#include "FlameParticle.h"

FlameParticle::FlameParticle(vec3 position, vec3 velocity, vec3 colour, float duration, int texture) : mPosition(position), mVelocity(velocity), mColour(colour), mDuration(duration), mTexture(texture) {

}

void FlameParticle::Draw(ShaderManager shaderManager)
{
	mat4 matrix = translate(mat4(1.0f), mPosition) * scale(mat4(1.0f), vec3(FLAME_SCALE));
	shaderManager.setMat4("worldMatrix", matrix);
	shaderManager.setVec3("colour", mColour);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	shaderManager.setVec3("colour", vec3(1.0f, 1.0f, 1.0f));
}

void FlameParticle::Update(float dt)
{
	mDuration -= dt;
	mVelocity.y += dt * FLAME_RISE;
	mPosition += mVelocity * dt;
}

bool FlameParticle::isDead()
{
	return mDuration <= 0;
}
