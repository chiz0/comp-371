#pragma once

#include <list>
#include <vector>

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>

#include "Constants.h"
#include "BurstParticle.h"

using namespace glm;
using namespace std;

class Emitter {
public:
	Emitter(int VAO);
	void Emit(vec3 position, int particleCount, float force, int texture);
	void Update(float dt);
	void Draw(ShaderManager shaderManager);

private:
	vector<BurstParticle> particles;
	int particleVAO;
};