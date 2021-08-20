#include "FlameParticle.h"

FlameParticle::FlameParticle(vec3 position, vec3 velocity, vec3 colour, float duration, int texture) : mPosition(position), mVelocity(velocity), mColour(colour), mDuration(duration), mTexture(texture) {

}

void FlameParticle::draw(GLenum* renderingMode, ShaderManager* shaderProgram)
{
	mat4 matrix = translate(worldAnchor, mPosition) * scale(mat4(1.0f), vec3(FLAME_SCALE));
	shaderProgram->setMat4("worldMatrix", matrix);
	shaderProgram->setVec3("colour", mColour);
	shaderProgram->setBool("ignoreLighting", true);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	shaderProgram->setBool("ignoreLighting", false);
}

void FlameParticle::update(vector<ScheduledEvent>* eventQueue, double dt)
{
	mDuration -= dt;
	mVelocity.y += dt * FLAME_RISE;
	mPosition += mVelocity * (float)dt;
}

bool FlameParticle::isDead()
{
	return mDuration <= 0;
}
