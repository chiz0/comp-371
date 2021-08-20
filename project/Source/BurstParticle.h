#pragma once

#include <list>
#include <vector>

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Constants.h"
#include "ShaderManager.h"
#include "GameObject.h"

using namespace glm;
using namespace std;

class BurstParticle : public GameObject {
public:
	BurstParticle(vec3 position, vec3 velocity, vec3 colour, float duration, int texture, quat rotation);
	void draw(GLenum* renderingMode, ShaderManager* shaderProgram);
	void update(vector<ScheduledEvent>* eventQueue, double dt);
	bool isDead();

private:
	vec3 mPosition;
	vec3 mVelocity;
	vec3 mColour;
	float mDuration;
	int mTexture;
	quat mRotation;
};