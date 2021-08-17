#include "Shape.h"

Shape::Shape(vec3 position, vector<ivec3> description, vec3 colour, int texture) : _position(position), _colour(colour), _texture(texture) {
	if (description.empty()) {
		description = { { 0, 0, 0 } };
	}
	init(description);
}

Shape::Shape(vec3 position, int voxelCount, vec3 colour, int texture) : _position(position), _colour(colour), _texture(texture) {
	if (voxelCount <= 0) { voxelCount = 1; };

	ivec3 offset((int)(ceil((float)voxelCount / 2.0f)));
	vector<vector<vector<bool>>> map(voxelCount, vector<vector<bool>>(voxelCount, vector<bool>(voxelCount, false)));

	vector<ivec3> description;
	vector<ivec3> candidates = { {0, 0, 0} };
	map[offset.x][offset.y][offset.z] = true;

	int minX = 0;
	int maxX = map.size() - 1;
	int minY = 0;
	int maxY = map[0].size() - 1;
	int minZ = 0;
	int maxZ = map[0][0].size() - 1;

	for (int i = 0; i < voxelCount; i++) {
		int randomIndex = rand() % candidates.size();
		ivec3 added = candidates[randomIndex];
		candidates.erase(candidates.begin() + randomIndex);
		description.push_back(added);
		if (added.x + offset.x + 1 <= maxX && !map[added.x + offset.x + 1][added.y + offset.y][added.z + offset.z]) {
			candidates.push_back({ added.x + 1, added.y, added.z });
			map[added.x + offset.x + 1][added.y + offset.y][added.z + offset.z] = true;
		}
		if (added.x + offset.x - 1 >= minX && !map[added.x + offset.x - 1][added.y + offset.y][added.z + offset.z]) {
			candidates.push_back({ added.x - 1, added.y, added.z });
			map[added.x + offset.x - 1][added.y + offset.y][added.z + offset.z] = true;
		}
		if (added.y + offset.y + 1 <= maxY && !map[added.x + offset.x][added.y + offset.y + 1][added.z + offset.z]) {
			candidates.push_back({ added.x, added.y + 1, added.z });
			map[added.x + offset.x][added.y + offset.y + 1][added.z + offset.z] = true;
		}
		if (added.y + offset.y - 1 >= minY && !map[added.x + offset.x][added.y + offset.y - 1][added.z + offset.z]) {
			candidates.push_back({ added.x, added.y - 1, added.z });
			map[added.x + offset.x][added.y + offset.y - 1][added.z + offset.z] = true;
		}
		if (added.z + offset.z + 1 <= maxZ && !map[added.x + offset.x][added.y + offset.y][added.z + offset.z + 1]) {
			candidates.push_back({ added.x, added.y, added.z + 1 });
			map[added.x + offset.x][added.y + offset.y][added.z + offset.z + 1] = true;
		}
		if (added.z + offset.z - 1 >= minZ && !map[added.x + offset.x][added.y + offset.y][added.z + offset.z - 1]) {
			candidates.push_back({ added.x, added.y, added.z - 1 });
			map[added.x + offset.x][added.y + offset.y][added.z + offset.z - 1] = true;
		}
	}

	init(description);
}

void Shape::draw(GLenum* renderingMode, ShaderManager* shaderProgram) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture);
	shaderProgram->setVec3("colour", _colour);
	shaderProgram->setInt("textureSampler", 0);
	mat4 worldMatrix = translate(mat4(1.0f), _position) * toMat4(displayOrientation) * scale(mat4(1.0f), _scale);
	for (auto it = begin(voxels); it != end(voxels); ++it) {
		it->anchorMatrix = worldMatrix;
		it->draw(renderingMode, shaderProgram);
	}
}

void Shape::update(vector<ScheduledEvent>* eventQueue, double dt) {
	switch (state) {
	case INITIALIZED: {
		randomRightAngleRotations();
		userInputResponse = false;
		for (Voxel& voxel : voxels) {
			voxel.displayPosition.x += (rand() % ANIMATE_CREATION_VOXEL_SPREAD) - (ANIMATE_CREATION_VOXEL_SPREAD / 2);
			voxel.displayPosition.y += (rand() % ANIMATE_CREATION_VOXEL_SPREAD) - (ANIMATE_CREATION_VOXEL_SPREAD / 2);
		}
		state = ANIMATE_CREATION;
		break;
	}
	case ANIMATE_CREATION: {
		userInputResponse = false;
		bool finished = true;
		for (Voxel& voxel : voxels) {
			if (voxel.displayPosition.x < voxel._position.x - ANIMATE_CREATION_MOVE_SPEED * dt) {
				voxel.displayPosition.x += ANIMATE_CREATION_MOVE_SPEED * dt;
				finished = false;
			}
			else if (voxel.displayPosition.x > voxel._position.x + ANIMATE_CREATION_MOVE_SPEED * dt) {
				voxel.displayPosition.x -= ANIMATE_CREATION_MOVE_SPEED * dt;
				finished = false;
			}
			else {
				voxel.displayPosition.x = voxel._position.x;
			}
			if (voxel.displayPosition.y < voxel._position.y - ANIMATE_CREATION_MOVE_SPEED * dt) {
				voxel.displayPosition.y += ANIMATE_CREATION_MOVE_SPEED * dt;
				finished = false;
			}
			else if (voxel.displayPosition.y > voxel._position.y + ANIMATE_CREATION_MOVE_SPEED * dt) {
				voxel.displayPosition.y -= ANIMATE_CREATION_MOVE_SPEED * dt;
				finished = false;
			}
			else {
				voxel.displayPosition.y = voxel._position.y;
			}
		}
		if (finished) {
			state = IDLE;
		}
		break;
	}
	case ANIMATE_ROTATE: {
		userInputResponse = false;
		bool finished = true;
		if (displayOrientation != _orientation) {
			finished = false;
			slerpProgress += ANIMATE_ROTATE_SPEED * dt;
			slerpProgress = slerpProgress > 1 ? 1 : slerpProgress;
			displayOrientation = slerp(previousOrientation, _orientation, slerpProgress);
			if (slerpProgress >= 1 || slerpProgress < 0) {
				displayOrientation = _orientation;
				finished = true;
			}
		}
		if (finished) {
			state = IDLE;
		}
		break;
	}
	case IDLE: {
		userInputResponse = true;
		break;
	}
	}
}

void Shape::processEvent(Event event) {
	if (userInputResponse) {
		switch (event) {

		case INPUT_UP: {
			previousOrientation = _orientation;
			slerpProgress = 0.0f;
			_orientation = angleAxis(radians(-90.0f), vec3(1, 0, 0)) * _orientation;
			state = ANIMATE_ROTATE;
			break;
		}
		case INPUT_DOWN: {
			previousOrientation = _orientation;
			slerpProgress = 0.0f;
			_orientation = angleAxis(radians(90.0f), vec3(1, 0, 0)) * _orientation;
			state = ANIMATE_ROTATE;
			break;
		}
		case INPUT_LEFT: {
			previousOrientation = _orientation;
			slerpProgress = 0.0f;
			_orientation = angleAxis(radians(-90.0f), vec3(0, 1, 0)) * _orientation;
			state = ANIMATE_ROTATE;
			break;
		}
		case INPUT_RIGHT: {
			previousOrientation = _orientation;
			slerpProgress = 0.0f;
			_orientation = angleAxis(radians(90.0f), vec3(0, 1, 0)) * _orientation;
			state = ANIMATE_ROTATE;
			break;
		}
		}
	}
}

vector<vector<bool>> Shape::getWallProjection(bool currentRotation) {

	vector<vector<vector<bool>>> useMap = map3D;
	vec3 eulerOrientation = eulerAngles(_orientation);
	eulerOrientation.x = degrees(eulerOrientation.x);
	eulerOrientation.y = degrees(eulerOrientation.y);
	eulerOrientation.z = degrees(eulerOrientation.z);

	if (currentRotation) {

		vector<vector<vector<bool>>> tempMap;
		if (eulerOrientation.x > 89 && eulerOrientation.x < 91) {
			tempMap = vector<vector<vector<bool>>>(useMap.size(), vector<vector<bool>>(useMap[0][0].size(), vector<bool>(useMap[0].size(), false)));
			for (int i = 0; i < tempMap.size(); i++) {
				for (int j = 0; j < tempMap[i].size(); j++) {
					for (int k = 0; k < tempMap[i][j].size(); k++) {
						tempMap[i][j][k] = useMap[i][k][useMap[0][0].size() - 1 - j];
					}
				}
			}
			useMap = tempMap;
		}
		else if (eulerOrientation.x > 179 && eulerOrientation.x < 181) {
			tempMap = vector<vector<vector<bool>>>(useMap.size(), vector<vector<bool>>(useMap[0].size(), vector<bool>(useMap[0][0].size(), false)));
			for (int i = 0; i < tempMap.size(); i++) {
				for (int j = 0; j < tempMap[i].size(); j++) {
					for (int k = 0; k < tempMap[i][j].size(); k++) {
						tempMap[i][j][k] = useMap[i][useMap[0].size() - 1 - j][useMap[0][0].size() - 1 - k];
					}
				}
			}
			useMap = tempMap;
		}
		else if (eulerOrientation.x > 269 && eulerOrientation.x < 271) {
			tempMap = vector<vector<vector<bool>>>(useMap.size(), vector<vector<bool>>(useMap[0][0].size(), vector<bool>(useMap[0].size(), false)));
			for (int i = 0; i < tempMap.size(); i++) {
				for (int j = 0; j < tempMap[i].size(); j++) {
					for (int k = 0; k < tempMap[i][j].size(); k++) {
						tempMap[i][j][k] = useMap[i][useMap[0].size() - 1 - k][j];
					}
				}
			}
			useMap = tempMap;
		}

		if (eulerOrientation.y > 89 && eulerOrientation.y < 91) {
			tempMap = vector<vector<vector<bool>>>(useMap[0][0].size(), vector<vector<bool>>(useMap[0].size(), vector<bool>(useMap.size(), false)));
			for (int i = 0; i < tempMap.size(); i++) {
				for (int j = 0; j < tempMap[i].size(); j++) {
					for (int k = 0; k < tempMap[i][j].size(); k++) {
						tempMap[i][j][k] = useMap[useMap.size() - 1 - k][j][i];
					}
				}
			}
			useMap = tempMap;
		}
		else if (eulerOrientation.y > 179 && eulerOrientation.y < 181) {
			tempMap = vector<vector<vector<bool>>>(useMap.size(), vector<vector<bool>>(useMap[0].size(), vector<bool>(useMap[0][0].size(), false)));
			for (int i = 0; i < tempMap.size(); i++) {
				for (int j = 0; j < tempMap[i].size(); j++) {
					for (int k = 0; k < tempMap[i][j].size(); k++) {
						tempMap[i][j][k] = useMap[useMap.size() - 1 - i][j][useMap[0][0].size() - 1 - k];
					}
				}
			}
			useMap = tempMap;
		}
		else if (eulerOrientation.y > 269 && eulerOrientation.y < 271) {
			tempMap = vector<vector<vector<bool>>>(useMap[0][0].size(), vector<vector<bool>>(useMap[0].size(), vector<bool>(useMap.size(), false)));
			for (int i = 0; i < tempMap.size(); i++) {
				for (int j = 0; j < tempMap[i].size(); j++) {
					for (int k = 0; k < tempMap[i][j].size(); k++) {
						tempMap[i][j][k] = useMap[k][j][useMap[0][0].size() - 1 - i];
					}
				}
			}
			useMap = tempMap;
		}

		if (eulerOrientation.z > 89 && eulerOrientation.z < 91) {
			tempMap = vector<vector<vector<bool>>>(useMap[0].size(), vector<vector<bool>>(useMap.size(), vector<bool>(useMap[0][0].size(), false)));
			for (int i = 0; i < tempMap.size(); i++) {
				for (int j = 0; j < tempMap[i].size(); j++) {
					for (int k = 0; k < tempMap[i][j].size(); k++) {
						tempMap[i][j][k] = useMap[j][useMap[0].size() - 1 - i][k];
					}
				}
			}
			useMap = tempMap;
		}
		else if (eulerOrientation.z > 179 && eulerOrientation.z < 181) {
			tempMap = vector<vector<vector<bool>>>(useMap.size(), vector<vector<bool>>(useMap[0].size(), vector<bool>(useMap[0][0].size(), false)));
			for (int i = 0; i < tempMap.size(); i++) {
				for (int j = 0; j < tempMap[i].size(); j++) {
					for (int k = 0; k < tempMap[i][j].size(); k++) {
						tempMap[i][j][k] = useMap[useMap.size() - 1 - i][useMap[0].size() - 1 - j][k];
					}
				}
			}
			useMap = tempMap;
		}
		else if (eulerOrientation.z > 269 && eulerOrientation.z < 271) {
			tempMap = vector<vector<vector<bool>>>(useMap[0].size(), vector<vector<bool>>(useMap.size(), vector<bool>(useMap[0][0].size(), false)));
			for (int i = 0; i < tempMap.size(); i++) {
				for (int j = 0; j < tempMap[i].size(); j++) {
					for (int k = 0; k < tempMap[i][j].size(); k++) {
						tempMap[i][j][k] = useMap[useMap.size() - 1 - j][i][k];
					}
				}
			}
			useMap = tempMap;
		}
	}

	vector<vector<bool>> map2D(useMap.size(), vector<bool>(useMap[0].size(), false));

	for (int i = 0; i < useMap.size(); i++) {
		for (int j = 0; j < useMap[i].size(); j++) {
			for (int k = 0; k < useMap[i][j].size(); k++) {
				if (useMap[i][j][k]) {
					map2D[i][j] = true;
				}
			}
		}
	}

	return map2D;
}

void Shape::init(vector<ivec3> description) {
	// Get min and max position values
	int minX = 0, maxX = 0, minY = 0, maxY = 0, minZ = 0, maxZ = 0;
	for (auto it = begin(description); it != end(description); ++it) {
		minX = minX > it->x ? it->x : minX;
		maxX = maxX < it->x ? it->x : maxX;
		minY = minY > it->y ? it->y : minY;
		maxY = maxY < it->y ? it->y : maxY;
		minZ = minZ > it->z ? it->z : minZ;
		maxZ = maxZ < it->z ? it->z : maxZ;
	}

	// Create boolean 3D map
	int sizeX = (maxX - minX + 1) % 2 == 0 ? maxX - minX + 2 : maxX - minX + 1;
	int sizeY = (maxY - minY + 1) % 2 == 0 ? maxY - minY + 2 : maxY - minY + 1;
	int sizeZ = (maxZ - minZ + 1) % 2 == 0 ? maxZ - minZ + 2 : maxZ - minZ + 1;
	vector<vector<vector<bool>>> map(sizeX, vector<vector<bool>>(sizeY, vector<bool>(sizeZ, false)));

	for (auto it = begin(description); it != end(description); ++it) {
		map[it->x - minX][it->y - minY][it->z - minZ] = true;
	}

	map3D = map;

	// Recenter shape for rotations
	centerOffset.x = (maxX - minX + 1) / 2;
	centerOffset.y = (maxY - minY + 1) / 2;
	centerOffset.z = (maxZ - minZ + 1) / 2;

	// Create voxels from map (to avoid duplicate coordinates in description)
	for (int i = 0; i < map3D.size(); i++) {
		for (int j = 0; j < map3D[i].size(); j++) {
			for (int k = 0; k < map3D[i][j].size(); k++) {
				if (map3D[i][j][k]) {
					voxels.push_back(Voxel(vec3(i - centerOffset.x, j - centerOffset.y, k - centerOffset.z)));
				}
			}
		}
	}

	state = INITIALIZED;
}

void Shape::randomRightAngleRotations() {
	_orientation = angleAxis(radians((rand() % 4) * 90.0f), vec3(1.0f, 0.0f, 0.0f)) * _orientation;
	_orientation = angleAxis(radians((rand() % 4) * 90.0f), vec3(0.0f, 1.0f, 0.0f)) * _orientation;
	_orientation = angleAxis(radians((rand() % 4) * 90.0f), vec3(0.0f, 0.0f, 1.0f)) * _orientation;
	_orientation = normalize(_orientation);
	displayOrientation = _orientation;
}
