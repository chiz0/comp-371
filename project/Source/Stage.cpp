#include "Stage.h"

Stage::Stage(vec3 position) : _position(position) {
    _orientation = vec3(0, 180, 0);
}

void Stage::draw(GLenum* renderingMode, ShaderManager* shaderProgram) {
    // Calculate anchor matrix
    //mat4 worldMatrix = translate(mat4(1.0f), _position) * rotate(mat4(1.0f), radians(_orientation.y), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), _scale);
    mat4 worldMatrix = translate(mat4(1.0f), _position) * rotate(mat4(1.0f), radians(_orientation.y), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), _scale);

    // Draw all TerrainComponents and bind texture only if different from previous
    int prevTexture = -1;
    glActiveTexture(GL_TEXTURE0);
    shaderProgram->setInt("textureSampler", 0);
    for (auto it = begin(terrainList); it != end(terrainList); ++it) {
        if (it->_chunk >= _position.z / (20 * _scale.z) - 1 && it->_chunk <= _position.z / (20 * _scale.z) + 3) {
            if (it->_texture != prevTexture) {
                prevTexture = it->_texture;
                glBindTexture(GL_TEXTURE_2D, it->_texture);
            }
            it->anchorMatrix = worldMatrix;
            it->draw(renderingMode, shaderProgram);
        }
    }

    // Draw models
    //for (auto it = begin(modelList); it != end(modelList); ++it) {
    //    it->anchorMatrix = worldMatrix;
    //    it->draw(renderingMode, shaderProgram);
    //}
}

void Stage::update(vector<ScheduledEvent>* eventQueue, double dt) {
    switch (state) {
    case INITIALIZED: {
        state = IDLE;
        break;
    }
    case IDLE: {
        _position.z += speed * dt;
        break;
    }
    }
}

void Stage::processEvent(Event event) {
    switch (event) {
    case WORLD_TRANSITION:
        destroyFlag = true;
        break;
    }
}

void Stage::attachTerrain(TerrainComponent terrain, vec3 position) {
    // Add terrain to list
    terrain._position = position;
    terrainList.push_back(terrain);

    // Sort terrain components by texture
    sort(terrainList.begin(), terrainList.end(),
        [](const TerrainComponent& a, const TerrainComponent& b) {
            return a._texture < b._texture;
        }
    );
}

void Stage::attachModel(Model model, vec3 position) {
    //model._position = position;
    modelList.push_back(model);
}
