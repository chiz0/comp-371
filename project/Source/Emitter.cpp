#include "Emitter.h"

void Emitter::Emit(vec3 position, int particleCount, float force, int texture)
{
	for (int i = 0; i < particleCount; i++) {
		float dirX = rand() % 10 / 10.0f - 0.5f;
		float dirY = rand() % 10 / 10.0f - 0.5f;
		float dirZ = rand() % 10 / 10.0f - 0.5f;
		particles.push_back(BurstParticle(
			position,
			vec3(dirX, dirY, dirZ) * force,
			vec3(rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f),
			rand() % 30 / 10.0f + 1.0f,
			texture
		));
	}
}

Emitter::Emitter(int VAO) : particleVAO(VAO)
{
}

void Emitter::Update(float dt)
{
	vector<int> toDelete;
	for (auto it = particles.begin(); it != particles.end(); it++) {
		it->Update(dt);
	}
	particles.erase(
		remove_if(
			particles.begin(),
			particles.end(),
			[](BurstParticle& b) { return b.isDead(); }
		),
		particles.end()
	);
}

void Emitter::Draw(ShaderManager shaderManager) {
	shaderManager.use();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBindVertexArray(particleVAO);
	for (auto it = particles.begin(); it != particles.end(); it++) {
		it->Draw(shaderManager);
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
