#include "Stage.h"

Stage::Stage(vec3 position, int defaultVAO) : _position(position), _defaultVAO(defaultVAO) {
    _orientation = vec3(0, 180, 0);
}

void Stage::draw(GLenum* renderingMode, ShaderManager* shaderProgram) {
    // Draw sun/moon
    sun->draw(renderingMode, shaderProgram);

    // Calculate anchor matrix
    mat4 worldMatrix = translate(mat4(1.0f), _position) * rotate(mat4(1.0f), radians(_orientation.y), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), _scale);

    // Draw particles
    particleEmitter->worldAnchor = worldMatrix;
    particleEmitter->draw(renderingMode, shaderProgram);

    // Draw all TerrainComponents and bind texture only if different from previous
    int prevTexture = -1;
    glActiveTexture(GL_TEXTURE0);
    shaderProgram->setInt("textureSampler", 0);
    for (auto it = begin(terrainList); it != end(terrainList); ++it) {
        if (it->_chunk >= (_position.z - 10) / (20 * _scale.z) - 1 && it->_chunk <= (_position.z - 10) / (20 * _scale.z) + 4 && (int)it->_chunk / 10 == currentWorld) {
            if (it->_texture != prevTexture) {
                prevTexture = it->_texture;
                glBindTexture(GL_TEXTURE_2D, it->_texture);
            }
            it->anchorMatrix = worldMatrix;
            it->draw(renderingMode, shaderProgram);
        }
    }

    // Draw models
    for (auto it = begin(modelList); it != end(modelList); ++it) {
        if (it->_chunk >= (_position.z - 10) / (20 * _scale.z) - 1 && it->_chunk <= (_position.z - 10) / (20 * _scale.z) + 4 && (int)it->_chunk / 10 == currentWorld) {
            it->anchorMatrix = worldMatrix;
            it->Draw(shaderProgram);
        }
    }

    // Rebind default VAO after all models are drawn
    glBindVertexArray(_defaultVAO);
    // Maybe draw a large coloured cube below stage with water/lava texture here
}

void Stage::update(vector<ScheduledEvent>* eventQueue, double dt) {
    switch (state) {
    case INITIALIZED: {
        state = IDLE;
        break;
    }
    case IDLE: {
        _position.z += speed * dt;
        if ((int) (_position.z / _scale.z) % 200 >= 160) {
            eventQueue->push_back({ PORTAL_ON_HORIZON, 0 });
            initialSpeed = speed;
            state = IDLE_WORLD_TRANSITION;
        }
        sun->progress = _position.z;
        sun->update(eventQueue, dt);
        for (int i = fires.size() - 1; i >= 0; i--) {
            if (fires[i].z + CAMERA_OFFSET.z < _position.z) {
                fires.erase(fires.begin() + i);
            }
        }

        for (int i = fires.size() - 1; i >= 0; i--) {
            if (fires[i].z < _position.z + FAR_PLANE) {
                particleEmitter->EmitFlame(fires[i], DEFAULT_FLAME_AMOUNT, DEFAULT_FLAME_FORCE);
            }
            if (fires[i].z + CAMERA_OFFSET.z < _position.z / _scale.z) {
                fires.erase(fires.begin() + i);
            }
        }
        particleEmitter->update(eventQueue, dt);
        break;
    }
    case IDLE_WORLD_TRANSITION: {
        _position.z += speed * dt;
        if ((int)(_position.z / _scale.z) % 200 >= 180) {
            eventQueue->push_back({ DESTROY_SHAPE_AND_WALL_WORLD_TRANSITION, 0 });
            speed = 2 * initialSpeed;
        }
        else if ((int)(_position.z / _scale.z) % 200 < 160) {
            state = IDLE;
            eventQueue->push_back({ WORLD_TRANSITION, 0 });
            currentWorld++;
            speed = initialSpeed;
            state = IDLE;
        }
        sun->progress = _position.z;
        sun->update(eventQueue, dt);

        for (int i = fires.size() - 1; i >= 0; i--) {
            if (fires[i].z < _position.z + FAR_PLANE) {
                particleEmitter->EmitFlame(fires[i], DEFAULT_FLAME_AMOUNT, DEFAULT_FLAME_FORCE);
            }
            if (fires[i].z + CAMERA_OFFSET.z < _position.z / _scale.z) {
                fires.erase(fires.begin() + i);
            }
        }
        particleEmitter->update(eventQueue, dt);
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

void Stage::setFlameParticle(vec3 position) {
    fires.push_back(position);
}

void Stage::attachModel(Model model) {
    modelList.push_back(model);
}