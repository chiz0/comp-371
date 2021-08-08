#include "Wall.h"

Wall::Wall(vec3 position, Shape* shape, int vao) : mPosition(position), mvao(vao), defaultPosition(position), mScale(shape->defaultScale), defaultScale(shape->defaultScale)
{
	int originX = shape->mDescription.front().x;
	int originY = shape->mDescription.front().y;
	int originZ = shape->mDescription.front().z;

	for (int i = 0; i < WALL_SIZE; i++) {
		for (int j = 0; j < WALL_SIZE; j++) {
			if (shape->projection[i][j] == false) {    // If the projection does not cover this area, create a wall section here
				coordinates placedWall = { i - originX, j - originY, 0 };
				mDescription.push_back(placedWall);
				voxels.push_back(Voxel(
					vec3(
						i - WALL_SIZE / 2 - originX,    // Wall segment x
						j - WALL_SIZE / 2 - originY,    // Wall segment y
						originZ	                        // Wall segment z
					), vao, vec3(1.0f, 1.0f, WALL_THICKNESS)));
			}
		}
	}
}

void Wall::Draw(GLenum renderingMode, ShaderManager shader) {
	mat4 worldMatrix = translate(mat4(1.0f), mPosition) * rotate(mat4(1.0f), radians(mOrientation.x), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0f), radians(mOrientation.y), vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), radians(mOrientation.z), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f) * mScale);
	for (auto it = begin(voxels); it != end(voxels); ++it) {
		it->mAnchor = worldMatrix;
		it->Draw(renderingMode, shader);
	}
}

void Wall::ResetPosition() {
	mPosition = defaultPosition;
	mOrientation = defaultOrientation;
	mScale = defaultScale;
}
