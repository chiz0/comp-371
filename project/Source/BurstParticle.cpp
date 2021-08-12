#include "BurstParticle.h"

BurstParticle::BurstParticle(vec3 position, vec3 velocity, vec4 colour, float duration, int texture) : mPosition(position), mVelocity(velocity), mColour(colour), mDuration(duration), mTexture(texture) {

}

void BurstParticle::Draw(ShaderManager shaderManager)
{
	mat4 matrix = translate(mat4(1.0f), mPosition);
	shaderManager.setMat4("worldMatrix", matrix);
	shaderManager.setVec4("colour", mColour);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glDrawArrays(GL_TRIANGLES, 0, 32);
}

void BurstParticle::Update(float dt)
{
	mDuration -= dt;
	mVelocity.y -= dt * 4.0f;
	mPosition += mVelocity * dt;
}

bool BurstParticle::isDead()
{
	return mDuration <= 0;
}
