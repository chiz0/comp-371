#include "Wall.h"

Wall::Wall(vec3 position, Shape* shape, vec3 colour, int texture) : _position(position), _colour(colour), _texture(texture), _shape(shape) {
    vector<vector<bool>> projection = shape->getWallProjection(false);
    hole = projection;

    vec2 offset(projection.size() / 2 + PADDING, projection[0].size() / 2 + PADDING);

    for (int i = 0; i < projection.size() + 2 * PADDING; i++) {
        vector<Voxel> newLine;
        for (int j = 0; j < projection[0].size() + 2 * PADDING; j++) {
            if (i < PADDING || j < PADDING || i >= PADDING + projection.size() || j >= PADDING + projection[0].size() || i - PADDING >= 0 && j - PADDING >= 0 && !projection[i - PADDING][j - PADDING]) {
                newLine.push_back(Voxel(vec3(j - offset.y, i - offset.x, 0)));
            }
        }
        voxels.push_back(newLine);
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
    for (auto row = begin(voxels); row != end(voxels); ++row) {
        for (auto it = begin(*row); it != end(*row); ++it) {
            it->anchorMatrix = worldMatrix;
            it->draw(renderingMode, shaderProgram);
        }
    }
}

void Wall::update(vector<ScheduledEvent>*eventQueue, double dt) {
    switch (state) {
    case INITIALIZED: {
        for (int i = 0; i < voxels.size(); i++) {
            for (int j = 0; j < voxels[i].size(); j++) {
                voxels[i][j].displayPosition.y += ANIMATE_CREATION_VOXEL_FALL_HEIGHT + ANIMATE_CREATION_VOXEL_INTERVAL * (j + (voxels.size() * i) / 3);
            }
        }
        state = ANIMATE_CREATION;
        break;
    }
    case ANIMATE_CREATION: {
        bool finished = true;
        for (int i = 0; i < voxels.size(); i++) {
            for (int j = 0; j < voxels[i].size(); j++) {
                if (voxels[i][j].displayPosition.y > voxels[i][j]._position.y + ANIMATE_CREATION_MOVE_SPEED * dt) {
                    voxels[i][j].displayPosition.y -= ANIMATE_CREATION_MOVE_SPEED * dt;
                    finished = false;
                }
                else {
                    voxels[i][j].displayPosition.y = voxels[i][j]._position.y;
                }
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
            }
            else {
                eventQueue->push_back({ LEVEL_FAILED, 0 });
                state = FAILED;
            }
        }
        break;
    }
    case SUCCESS: {
        _position.z += speed * dt;
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
