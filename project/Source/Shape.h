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

using namespace glm;
using namespace std;


class Shape : public GameObject {
public:
    // Functions
    Shape(vec3 position, vector<ivec3> description, vec3 colour, int texture);
    Shape(vec3 position, int voxelCount, vec3 colour, int texture);

    void draw(GLenum* renderingMode, ShaderManager* shaderProgram);
    void update(vector<ScheduledEvent>* eventQueue, double dt);
    void processEvent(Event event);
    vector<vector<bool>> getWallProjection(bool currentRotation);

    // Properties
    double timer = 0;
    vector<Voxel> voxels;
    vector<vector<vector<bool>>> map3D;
    vec3 centerOffset;
    bool userInputResponse = false;
    enum {
        INITIALIZED,
        IDLE,
        ANIMATE_CREATION,
        ANIMATE_DESTRUCTION,
        ANIMATE_ROTATE
    } state = INITIALIZED;
    vec3 _position;
    quat _orientation = quat();
    vec3 _scale = vec3(1.0f);
    vec3 _colour;
    int _texture;
    quat displayOrientation = quat();
    const float ANIMATE_CREATION_MOVE_SPEED = 20.0f;
    const int ANIMATE_CREATION_VOXEL_SPREAD = 10;
    const float ANIMATE_ROTATE_SPEED = 10.0f;

private:
    // Functions
    void init(vector<ivec3> description);
    void randomRightAngleRotations();

    // Properties
    quat previousOrientation = quat();
    float slerpProgress = 0.0f;
};
