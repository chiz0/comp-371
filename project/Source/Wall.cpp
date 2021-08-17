#include "Wall.h"

Wall::Wall(vec3 position, Shape* shape, vec3 colour, int texture) : _position(position), _colour(colour), _texture(texture), _shape(shape) {
    vector<vector<bool>> projection = shape->getWallProjection(false);
    hole = projection;

    offset = vec2(projection.size() / 2 + PADDING, projection[0].size() / 2 + PADDING);
    _width = projection.size() + 2 * PADDING;
    _height = projection[0].size() + 2 * PADDING;
    for (int i = 0; i < _width; i++) {
        for (int j = 0; j < _height; j++) {
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
    quat orientationQuat(_orientation);
    mat4 worldMatrix = translate(mat4(1.0f), _position) * toMat4(orientationQuat) * scale(mat4(1.0f), _scale);
    for (auto it = begin(voxels); it != end(voxels); ++it) {
        it->anchorMatrix = worldMatrix;
        it->draw(renderingMode, shaderProgram);
    }
}

void Wall::update(vector<ScheduledEvent>*eventQueue, double dt) {
    switch (state) {
    case INITIALIZED: {
        for (auto it = begin(voxels); it != end(voxels); ++it) {
            it->displayPosition.y += ANIMATE_CREATION_VOXEL_FALL_HEIGHT + ANIMATE_CREATION_VOXEL_INTERVAL * (it->_position.x + offset.x + (offset.x * 2 * (it->_position.y + offset.y)) / ANIMATE_CREATION_SIMULTANEOUS_ROWS);
        }
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
        break;
    }
    case IDLE: {
        _position.z += speed * dt;
        if (_position.z >= _shape->_position.z - 1) {
            if (testCollision()) {
                eventQueue->push_back({ LEVEL_SUCCESS, 0 });
                state = SUCCESS;
                timer = -1;
                particleEmitter->EmitBurst(_position + vec3(_width / 2, _height, 0.0f), 50, 10.0f);
                particleEmitter->EmitBurst(_position + vec3(-_width / 2, _height, 0.0f), 50, 10.0f);
            }
            else {
                eventQueue->push_back({ LEVEL_FAILED, 0 });
                state = FAILURE;
                timer = 0;
            }
        }
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
        break;
    }
    case FAILURE: {
        speed += 1 * dt;
        _position.z += speed * dt;
        timer += dt;
        if (timer >= 10) {
            destroyFlag = true;
        }
        break;
    }
    }
}

void Wall::processEvent(Event event) {

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
