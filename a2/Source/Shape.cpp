#include "Shape.h"

Shape::Shape(vec3 position, vector<coordinates> description, int vao, bool hasWall) : mPosition(position), mvao(vao), voxelCount(description.size()), defaultPosition(position), showWall(hasWall)
{
	for (int i = 0; i < WALL_SIZE; i++) {
		for (int j = 0; j < WALL_SIZE; j++) {
			projection[i][j] = false;
		}
	}
	int originX = description.front().x;
	int originY = description.front().y;
	int originZ = description.front().z;
	for (auto it = begin(description); it != end(description); ++it) {
		struct coordinates remappedCoordinates = { it->x - originX, it->y - originY, it->z - originZ };
		mDescription.push_back(remappedCoordinates);
		voxels.push_back(Voxel(vec3(remappedCoordinates.x, remappedCoordinates.y, remappedCoordinates.z), vao));
		if (remappedCoordinates.x + WALL_SIZE / 2 >= 0 && remappedCoordinates.x < WALL_SIZE / 2
			&& remappedCoordinates.y + WALL_SIZE / 2 >= 0 && remappedCoordinates.y < WALL_SIZE / 2)
		{
			projection[remappedCoordinates.x + WALL_SIZE / 2][remappedCoordinates.y + WALL_SIZE / 2] = true;
		}
	}

	if (hasWall) {
		// Create wall voxels
		for (int i = 0; i < WALL_SIZE; i++) {
			for (int j = 0; j < WALL_SIZE; j++) {
				if (projection[i][j] == false) {    // If the projection does not cover this area, create a wall section here
					coordinates placedWall = {  };
					wallVoxels.push_back(Voxel(
						vec3(
							i - WALL_SIZE / 2 - originX,    // Wall segment x
							j - WALL_SIZE / 2 - originY,    // Wall segment y
							originZ - WALL_DISTANCE         // Wall segment z
						), vao, vec3(1.0f, 1.0f, WALL_THICKNESS)));
				}
			}
		}
	}
}

void Shape::Draw(GLenum renderingMode, GLuint worldMatrixLocation) {
	mat4 worldMatrix = GetModelMatrix();
	for (auto it = begin(voxels); it != end(voxels); ++it) {
		it->mAnchor = worldMatrix;
		it->Draw(renderingMode, worldMatrixLocation);
	}
	if (showWall) {
		for (auto it = begin(wallVoxels); it != end(wallVoxels); ++it) {
			it->mAnchor = worldMatrix;
			it->Draw(renderingMode, worldMatrixLocation);
		}
	}
}

void Shape::Reshuffle() {
	vector<struct coordinates> newCoordinates;
	int minX = -WALL_SIZE + 1;
	int maxX = WALL_SIZE - 1;
	int minY = -WALL_SIZE + 1;
	int maxY = WALL_SIZE - 1;
	int minZ = -WALL_DISTANCE + 1;
	int maxZ = voxelCount;

	vector<vector<vector<bool>>> map(maxX - minX + 1, vector<vector<bool> >(maxY - minY + 1, vector <bool>(maxZ - minZ + 1, false)));
	vector<coordinates> candidates;

	candidates.push_back({ 0, 0, 0 });

	for (int i = 0; i < voxelCount; i++) {
		int randomIndex = rand() % candidates.size();
		coordinates added = candidates[randomIndex];
		candidates.erase(candidates.begin() + randomIndex);
		newCoordinates.push_back(added);
		map[added.x - minX][added.y - minY][added.z - minZ] = true;
		if (added.x + 1 <= maxX && !map[added.x + 1 - minX][added.y - minY][added.z - minZ] && projection[added.x + 1 + WALL_SIZE / 2][added.y + WALL_SIZE / 2]) {
			candidates.push_back({ added.x + 1, added.y, added.z });
		}
		if (added.x - 1 >= minX && !map[added.x - 1 - minX][added.y - minY][added.z - minZ] && projection[added.x - 1 + WALL_SIZE / 2][added.y + WALL_SIZE / 2]) {
			candidates.push_back({ added.x - 1, added.y, added.z });
		}
		if (added.y + 1 <= maxY && !map[added.x - minX][added.y + 1 - minY][added.z - minZ] && projection[added.x + WALL_SIZE / 2][added.y + 1 + WALL_SIZE / 2]) {
			candidates.push_back({ added.x, added.y + 1, added.z });
		}
		if (added.y - 1 >= minY && !map[added.x - minX][added.y - 1 - minY][added.z - minZ] && projection[added.x + WALL_SIZE / 2][added.y - 1 + WALL_SIZE / 2]) {
			candidates.push_back({ added.x, added.y - 1, added.z });
		}
		if (added.z + 1 <= maxZ && !map[added.x - minX][added.y - minY][added.z + 1 - minZ]) {
			candidates.push_back({ added.x, added.y, added.z + 1 });
		}
		if (added.z - 1 >= minZ && !map[added.x - minX][added.y - minY][added.z - 1 - minZ]) {
			candidates.push_back({ added.x, added.y, added.z - 1 });
		}
	}

	mDescription.clear();
	voxels.clear();

	for (auto it = begin(newCoordinates); it != end(newCoordinates); ++it) {
		mDescription.push_back(*it);
		voxels.push_back(Voxel(vec3(it->x, it->y, it->z), mvao));
	}
}

void Shape::ResetPosition() {
	mPosition = defaultPosition;
	mOrientation = defaultOrientation;
	mScale = defaultScale;
}

mat4 Shape::GetModelMatrix() {
	return translate(mat4(1.0f), mPosition)* rotate(mat4(1.0f), radians(mOrientation.x), vec3(1.0f, 0.0f, 0.0f))* rotate(mat4(1.0f), radians(mOrientation.y), vec3(0.0f, 1.0f, 0.0f))* rotate(mat4(1.0f), radians(mOrientation.z), vec3(0.0f, 0.0f, 1.0f))* scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f) * mScale);
}