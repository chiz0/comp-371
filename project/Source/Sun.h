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


class Sun : public GameObject {
    enum SunState {
        NONE,
        DAWN,
        DAY,
        DUSK,
        NIGHT,
        NETHER,
        END
    };

public:
    Sun(int sunTexture, int moonTexture);
    void draw(GLenum* renderingMode, ShaderManager* shaderProgram);
    void update(vector<ScheduledEvent>* eventQueue, double dt);
    vec3 getLightPosition();
    vec3 getSunPosition();

    bool destroyFlag = false;
    float progress;

private:
    vec3 _position;
    Voxel _voxel;
    int _sunTexture;
    int _moonTexture;
    SunState state;
};