#pragma once

#include <list>
#include <vector>

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Constants.h"
#include "BurstParticle.h"
#include "FlameParticle.h"
#include "GameObject.h"

using namespace glm;
using namespace std;

class Emitter : public GameObject {
public:
	Emitter(int VAO, int burstTexture, int flameTexture);
	void EmitBurst(vec3 position, int particleCount, float force, int texture = -1);
	void EmitFlame(vec3 position, int particleCount, float force, int texture = -1);
	void draw(GLenum* renderingMode, ShaderManager* shaderProgram);
	void update(vector<ScheduledEvent>* eventQueue, double dt);

	mat4 worldAnchor = mat4(1.0f);

private:
	vector<BurstParticle> burstParticles;
	vector<FlameParticle> flameParticles;
	int particleVAO;
	int defaultBurstTexture;
	int defaultFlameTexture;
};