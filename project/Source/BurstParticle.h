#pragma once

#include <list>
#include <vector>

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>

#include "Constants.h"
#include "ShaderManager.h"

using namespace glm;
using namespace std;

class BurstParticle {
public:
	BurstParticle(vec3 position, vec3 velocity, vec4 colour, float duration, int texture);
	void Draw(ShaderManager shaderManager);
	void Update(float dt);
	bool isDead();

private:
	vec3 mPosition;
	vec3 mVelocity;
	vec4 mColour;
	float mDuration;
	int mTexture;
};