#pragma once

#include<vector>

#include <glm/glm.hpp>
#include "ShaderManager.h"
#include "ScheduledEvent.h"

using namespace glm;
using namespace std;

class GameObject {
public:
    virtual void draw(GLenum* renderingMode, ShaderManager* shaderProgram) = 0;
    virtual void update(vector<ScheduledEvent>* eventQueue, double dt) = 0;

    bool destroyFlag = false;
};
