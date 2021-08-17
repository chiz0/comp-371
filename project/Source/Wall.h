#pragma once

#include <vector>

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Constants.h"
#include "GameObject.h"
#include "ShaderManager.h"
#include "Voxel.h"
#include "Shape.h"
#include "Emitter.h"

using namespace glm;
using namespace std;


class Wall : public GameObject {
public:
    // Functions
    Wall(vec3 position, Shape* shape, vec3 colour, int texture);

    void draw(GLenum* renderingMode, ShaderManager* shaderProgram);
    void update(vector<ScheduledEvent>* eventQueue, double dt);
    void processEvent(Event event);
    bool testCollision();

    // Properties
    const int PADDING = 2;
    vec2 offset;
    double timer = 0;
    Shape* _shape;
    vector<vector<bool>> hole;
    vector<Voxel> voxels;
    enum {
        INITIALIZED,
        IDLE,
        ANIMATE_CREATION,
        ANIMATE_DESTRUCTION,
        FAILURE,
        SUCCESS
    } state = INITIALIZED;
    vec3 _position;
    vec3 _orientation = vec3(0);
    vec3 _scale = vec3(1.0f);
    vec3 _colour;
    int _texture;
    float speed = 0;
    Emitter* particleEmitter;
    const float ANIMATE_CREATION_VOXEL_FALL_HEIGHT = 30.0f;
    const float ANIMATE_CREATION_MOVE_SPEED = 50.0f;
    const float ANIMATE_CREATION_VOXEL_INTERVAL = 3.0f;
    const float ANIMATE_CREATION_SIMULTANEOUS_ROWS = 3.0f;
    const float ANIMATE_DESTRUCTION_MOVE_SPEED = 2.0f;
    const float ANIMATE_DESTRUCTION_VOXEL_INTERVAL = 15.0f;
    const float ANIMATE_DESTRUCTION_LINE_INTERVAL = 0.8f;
};
