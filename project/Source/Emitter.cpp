#include "Emitter.h"

Emitter::Emitter(int VAO, int burstTexture, int flameTexture) : particleVAO(VAO), defaultBurstTexture(burstTexture), defaultFlameTexture(flameTexture)
{
}

void Emitter::EmitBurst(vec3 position, int particleCount, float force, int texture)
{
	int tex = texture == -1 ? defaultBurstTexture : texture;

	for (int i = 0; i < particleCount; i++) {
		float dirX = rand() % 10 / 10.0f - 0.5f;
		float dirY = rand() % 10 / 10.0f - 0.5f;
		float dirZ = rand() % 10 / 10.0f - 0.5f;

		float colourR = rand() % 50 / 100.0f + 0.5f;
		float colourG = rand() % 50 / 100.0f + 0.5f;
		float colourB = rand() % 50 / 100.0f + 0.5f;

		float rotX = rand() % 3600 / 10.0f;
		float rotY = rand() % 3600 / 10.0f;
		float rotZ = rand() % 3600 / 10.0f;

		quat rotation = normalize(quat());
		rotation *= angleAxis(radians(rotX), vec3(1.0f, 0.0f, 0.0f));
		rotation *= angleAxis(radians(rotY), vec3(0.0f, 1.0f, 0.0f));
		rotation *= angleAxis(radians(rotZ), vec3(0.0f, 0.0f, 1.0f));

		burstParticles.push_back(BurstParticle(
			position,							// Starting location
			vec3(dirX, dirY, dirZ) * force,		// Direction
			vec3(colourR, colourG, colourB),	// Colour
			rand() % 30 / 10.0f + 1.0f,			// Lifespan
			tex,								// Texture
			rotation
		));
	}
}

void Emitter::EmitFlame(vec3 position, int particleCount, float force, int texture)
{
	int tex = texture == -1 ? defaultFlameTexture : texture;

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
			tex									// Texture
		));
	}
}

void Emitter::draw(GLenum* renderingMode, ShaderManager* shaderProgram)
{
	shaderProgram->use();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBindVertexArray(particleVAO);
	for (BurstParticle particle : burstParticles) {
		particle.draw(renderingMode, shaderProgram);
	}
	for (FlameParticle particle : flameParticles) {
		particle.worldAnchor = worldAnchor;
		particle.draw(renderingMode, shaderProgram);
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Emitter::update(vector<ScheduledEvent>* eventQueue, double dt)
{
	for (BurstParticle particle : burstParticles) {
		particle.update(eventQueue, dt);
	}
	for (FlameParticle particle : flameParticles) {
		particle.update(eventQueue, dt);
	}
	// Delete all dead particles
	burstParticles.erase(
		remove_if(
			burstParticles.begin(),
			burstParticles.end(),
			[](BurstParticle p) { return p.isDead(); }
		),
		burstParticles.end()
	);
	flameParticles.erase(
		remove_if(
			flameParticles.begin(),
			flameParticles.end(),
			[](FlameParticle p) { return p.isDead(); }
		),
		flameParticles.end()
	);
}