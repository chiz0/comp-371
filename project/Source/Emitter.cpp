#include "Emitter.h"

void Emitter::EmitBurst(vec3 position, int particleCount, float force, int texture)
{
	for (int i = 0; i < particleCount; i++) {
		float dirX = rand() % 10 / 10.0f - 0.5f;
		float dirY = rand() % 10 / 10.0f - 0.5f;
		float dirZ = rand() % 10 / 10.0f - 0.5f;

		float colourR = rand() % 100 / 100.0f;
		float colourG = rand() % 100 / 100.0f;
		float colourB = rand() % 100 / 100.0f;
		burstParticles.push_back(BurstParticle(
			position,							// Starting location
			vec3(dirX, dirY, dirZ) * force,		// Direction
			vec3(colourR, colourG, colourB),	// Colour
			rand() % 30 / 10.0f + 1.0f,			// Lifespan
			texture								// Texture
		));
	}
}

void Emitter::EmitFlame(vec3 position, int particleCount, float force, int texture)
{
	for (int i = 0; i < particleCount; i++) {
		float dirX = rand() % 20 / 10.0f - 1.0f;
		float dirY = rand() % 5 / 5.0f;
		float dirZ = rand() % 20 / 10.0f - 1.0f;

		float colourR = rand() % 100 / 500.0f + 0.5;
		float colourG = rand() % 100 / 500.0f;
		float colourB = 0.0f;
		flameParticles.push_back(FlameParticle(
			position,							// Starting location
			vec3(dirX, dirY, dirZ) * force,		// Direction
			vec3(colourR, colourG, colourB),	// Colour
			rand() % 30 / 10.0f + 1.0f,			// Lifespan
			texture								// Texture
		));
	}
}

Emitter::Emitter(int VAO) : particleVAO(VAO)
{
}

void Emitter::Update(float dt)
{
	// Burst Particles
	vector<int> toDelete;
	for (auto it = burstParticles.begin(); it != burstParticles.end(); it++) {
		it->Update(dt);
	}
	// Delete all dead particles
	burstParticles.erase(
		remove_if(
			burstParticles.begin(),
			burstParticles.end(),
			[](BurstParticle& b) { return b.isDead(); }
		),
		burstParticles.end()
	);

	toDelete.clear();
	for (auto it = flameParticles.begin(); it != flameParticles.end(); it++) {
		it->Update(dt);
	}
	// Delete all dead particles
	flameParticles.erase(
		remove_if(
			flameParticles.begin(),
			flameParticles.end(),
			[](FlameParticle& f) { return f.isDead(); }
		),
		flameParticles.end()
	);
}

void Emitter::Draw(ShaderManager shaderManager) {
	shaderManager.use();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBindVertexArray(particleVAO);
	for (auto it = burstParticles.begin(); it != burstParticles.end(); it++) {
		it->Draw(shaderManager);
	}
	for (auto it = flameParticles.begin(); it != flameParticles.end(); it++) {
		it->Draw(shaderManager);
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
