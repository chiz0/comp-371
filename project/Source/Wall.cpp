#include "Wall.h"

Wall::Wall(vec3 position, Shape* shape, vec3 colour, int texture) : _position(position), _colour(colour), _texture(texture), _shape(shape) {
    vector<vector<bool>> projection = shape->getWallProjection(false);
    hole = projection;

    offset = vec2(projection.size() / 2 + PADDING, projection[0].size() / 2 + PADDING);
    for (int i = 0; i < projection.size() + 2 * PADDING; i++) {
        for (int j = 0; j < projection[0].size() + 2 * PADDING; j++) {
            if (i < PADDING || j < PADDING || i >= PADDING + projection.size() || j >= PADDING + projection[0].size() || i - PADDING >= 0 && j - PADDING >= 0 && !projection[i - PADDING][j - PADDING]) {
                voxels.push_back(Voxel(vec3(i - offset.x, j - offset.y, 0)));
            }
        }
    }

    state = INITIALIZED;
}

void Wall::draw(GLenum* renderingMode, ShaderManager* shaderProgram) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture);
    shaderProgram->setVec3("colour", _colour);
    shaderProgram->setInt("textureSampler", 0);
    shaderProgram->setFloat("ambientBoost", AMBIENT_BOOST);
    //quat orientationQuat(_orientation);
    //mat4 worldMatrix = translate(mat4(1.0f), displayPosition) * toMat4(orientationQuat) * scale(mat4(1.0f), _scale);
    mat4 worldMatrix = translate(mat4(1.0f), displayPosition);
    for (auto it = begin(voxels); it != end(voxels); ++it) {
        it->anchorMatrix = worldMatrix;
        it->draw(renderingMode, shaderProgram);
    }
    shaderProgram->setFloat("ambientBoost", 1.0f);
}

void Wall::update(vector<ScheduledEvent>*eventQueue, double dt) {
    switch (state) {
    case INITIALIZED: {
        for (auto it = begin(voxels); it != end(voxels); ++it) {
            it->displayPosition.y += ANIMATE_CREATION_VOXEL_FALL_HEIGHT + ANIMATE_CREATION_VOXEL_INTERVAL * (it->_position.x + offset.x + (offset.x * 2 * (it->_position.y + offset.y)) / ANIMATE_CREATION_SIMULTANEOUS_ROWS);
        }
        displayPosition = _position;
        state = ANIMATE_CREATION;
        break;
    }
    case ANIMATE_CREATION: {
        bool finished = true;
        for (auto it = begin(voxels); it != end(voxels); ++it) {
            if (it->displayPosition.y > it->_position.y + ANIMATE_CREATION_MOVE_SPEED * dt) {
                it->displayPosition.y -= ANIMATE_CREATION_MOVE_SPEED * dt;
                finished = false;
            }
            else {
                it->displayPosition.y = it->_position.y;
            }
        }
        if (finished) {
            eventQueue->push_back({DISPLAY_SHAPE, 0});
            state = IDLE;
        }
        displayPosition = _position;
        break;
    }
    case IDLE: {
        _position.z += speed * dt;
        if (_position.z >= _shape->_position.z - 1.2f) {
            if (testCollision()) {
                eventQueue->push_back({ LEVEL_SUCCESS, 0 });
                state = SUCCESS;
                timer = -1;
                particleEmitter->EmitBurst(_position + vec3(hole.size() / 2 * PADDING, hole[0].size() + 2 * PADDING, 0.0f), DEFAULT_BURST_AMOUNT, DEFAULT_BURST_FORCE);
                particleEmitter->EmitBurst(_position + vec3(-((int)hole.size() / 2 * PADDING), hole[0].size() + 2 * PADDING, 0.0f), DEFAULT_BURST_AMOUNT, DEFAULT_BURST_FORCE);
            }
            else {
                eventQueue->push_back({ LEVEL_FAILED, 0 });
                state = FAILURE;
                timer = 0;
            }
        }
        else if (_position.z >= _shape->_position.z - 1.5f) {
            eventQueue->push_back({ COLLISION_IMMINENT, 0 });
        }
        displayPosition = _position;
        break;
    }
    case SUCCESS: {
        speed = INITIAL_WALL_SPEED;
        _position.z += speed * dt;
        timer += dt;
        if (timer >= 0) {
            for (auto it = begin(voxels); it != end(voxels); ++it) {
                if (it->_position.x + offset.x + (offset.x * (it->_position.y + offset.y) * ANIMATE_DESTRUCTION_LINE_INTERVAL) <= timer * ANIMATE_DESTRUCTION_VOXEL_INTERVAL || abs(it->_position.x - offset.x) + (offset.x * (it->_position.y + offset.y) * ANIMATE_DESTRUCTION_LINE_INTERVAL) <= timer * ANIMATE_DESTRUCTION_VOXEL_INTERVAL) {
                    if (it->displayPosition.x > 0) {
                        it->displayPosition.x += sqrt(abs(it->_position.x)) * ANIMATE_DESTRUCTION_MOVE_SPEED * dt;
                    }
                    else if (it->displayPosition.x < 0) {
                        it->displayPosition.x -= sqrt(abs(it->_position.x)) * ANIMATE_DESTRUCTION_MOVE_SPEED * dt;
                    }
                    it->displayPosition.y -= powf((20 - it->_position.y), 2) * 0.03 * ANIMATE_DESTRUCTION_MOVE_SPEED * dt;
                }
            }
        }
        if (timer >= 5) {
            destroyFlag = true;
        }
        displayPosition = _position;
        break;
    }
    case FAILURE: {
        timer += dt;
        speed += 1 * dt;
        displayPosition = _position;
        if (timer >= 0 && timer < 1) {
            if (timer < 0.2 && (int)(timer * 10) % 1 == 0) {
                displayPosition.x += ((rand() % 10) - 4.5f) / 40 * sqrt(speed);
                displayPosition.y += ((rand() % 10) - 4.5f) / 40 * sqrt(speed);
            }
        }
        else {
            _position.z += speed * dt;
        }
        if (timer >= 10) {
            destroyFlag = true;
        }
        break;
    }
    }
}

void Wall::processEvent(Event event) {
    switch (event) {
    case DESTROY_SHAPE_AND_WALL_WORLD_TRANSITION: {
        state = SUCCESS;
        timer = 0;
        break;
    }
    }
}

bool Wall::testCollision() {
    vector<vector<bool>> projection = _shape->getWallProjection(true);
    bool pass = false;
    
    if (hole.size() == projection.size() && hole[0].size() == projection[0].size()) {
        pass = true;
        for (int i = 0; i < projection.size(); i++) {
            for (int j = 0; j < projection[i].size(); j++) {
                if (projection[i][j] && !hole[i][j]) {
                    pass = false;
                    break;
                }
            }
        }
    }

    return pass;
}
