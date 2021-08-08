#include "Voxel.h"

Voxel::Voxel(vec3 position, vec3 localScale) : mPosition(position), mScaleVector(localScale) {}

void Voxel::Draw(GLenum renderingMode, ShaderManager shader) {
	mat4 worldMatrix = mAnchor * translate(mat4(1.0f), mPosition) * rotate(mat4(1.0f), radians(mOrientation.x), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0f), radians(mOrientation.y), vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), radians(mOrientation.z), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), mScale * mScaleVector);
	shader.setMat4("worldMatrix", worldMatrix);
	glDrawArrays(renderingMode, 0, 36);
}