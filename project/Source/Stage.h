#pragma once

#include <vector>
#include <algorithm>

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Constants.h"
#include "GameObject.h"
#include "ShaderManager.h"
#include "TerrainComponent.h"
#include "Emitter.h"
#include "Model.h"
#include "Sun.h"

using namespace glm;
using namespace std;


class Stage : public GameObject {
public:
    // Functions
    Stage(vec3 position, int defaultVAO);

    void draw(GLenum* renderingMode, ShaderManager* shaderProgram);
    void update(vector<ScheduledEvent>* eventQueue, double dt);
    void processEvent(Event event);
    void setFlameParticle(vec3 position);
    void attachTerrain(TerrainComponent terrain, vec3 position);
    void attachModel(Model model);

    double timer = 0;
    int currentWorld = 0;
    vector<TerrainComponent> terrainList;
    vector<Model> modelList;
    Sun* sun;
    enum {
        INITIALIZED,
        IDLE,
        IDLE_WORLD_TRANSITION
    } state = INITIALIZED;
    int floorTextures[WORLDS];
    Voxel floor;
    vec3 _position;
    vec3 _orientation = vec3(0);
    vec3 _scale = vec3(2.0f);
    float speed = 0;
    float initialSpeed = 0;
    Emitter* particleEmitter;
    int _defaultVAO;
    vector<vec3> fires;
};
