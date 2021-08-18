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
            voxel.displayPosition.z += (rand() % ANIMATE_CREATION_VOXEL_SPREAD) - (ANIMATE_CREATION_VOXEL_SPREAD / 2);
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
            if (voxel.displayPosition.z < voxel._position.z - ANIMATE_CREATION_MOVE_SPEED * dt) {
                voxel.displayPosition.z += ANIMATE_CREATION_MOVE_SPEED * dt;
                finished = false;
            }
            else if (voxel.displayPosition.z > voxel._position.z + ANIMATE_CREATION_MOVE_SPEED * dt) {
                voxel.displayPosition.z -= ANIMATE_CREATION_MOVE_SPEED * dt;
                finished = false;
            }
            else {
                voxel.displayPosition.z = voxel._position.z;
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
    case ANIMATE_DESTRUCTION: {
        userInputResponse = false;
        timer += dt;
        for (Voxel& voxel : voxels) {
            if (voxel._position.x < 0) {
                voxel.displayPosition.x -= ANIMATE_DESTRUCTION_MOVE_SPEED * timer;
            }
            else if (voxel._position.x >= 0) {
                voxel.displayPosition.x += ANIMATE_DESTRUCTION_MOVE_SPEED * timer;
            }
            if (voxel._position.y < 0) {
                voxel.displayPosition.y -= ANIMATE_DESTRUCTION_MOVE_SPEED * timer;
            }
            else if (voxel._position.y >= 0) {
                voxel.displayPosition.y += ANIMATE_DESTRUCTION_MOVE_SPEED * timer;
            }
            if (voxel._position.z < 0) {
                voxel.displayPosition.z -= ANIMATE_DESTRUCTION_MOVE_SPEED * timer;
            }
            else if (voxel._position.z >= 0) {
                voxel.displayPosition.z += ANIMATE_DESTRUCTION_MOVE_SPEED * timer;
            }
        }
        if (timer >= 2) {
            destroyFlag = true;
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
            _orientation = angleAxis(radians(-90.0f), vec3(1, 0, 0)) * _orientation; // transposeMap function must be changed if rotation is changed
            transposeMap(event);
            state = ANIMATE_ROTATE;
            break;
        }
        case INPUT_DOWN: {
            previousOrientation = _orientation;
            slerpProgress = 0.0f;
            _orientation = angleAxis(radians(90.0f), vec3(1, 0, 0)) * _orientation; // transposeMap function must be changed if rotation is changed
            transposeMap(event);
            state = ANIMATE_ROTATE;
            break;
        }
        case INPUT_LEFT: {
            previousOrientation = _orientation;
            slerpProgress = 0.0f;
            _orientation = angleAxis(radians(-90.0f), vec3(0, 1, 0)) * _orientation; // transposeMap function must be changed if rotation is changed
            transposeMap(event);
            state = ANIMATE_ROTATE;
            break;
        }
        case INPUT_RIGHT: {
            previousOrientation = _orientation;
            slerpProgress = 0.0f;
            _orientation = angleAxis(radians(90.0f), vec3(0, 1, 0)) * _orientation; // transposeMap function must be changed if rotation is changed
            transposeMap(event);
            state = ANIMATE_ROTATE;
            break;
        }
        }
    }
    switch (event) {
    case DESTROY_SHAPE_AND_WALL: {
        state = ANIMATE_DESTRUCTION;
        timer = 0;
    }
    }
}

vector<vector<bool>> Shape::getWallProjection(bool currentRotation) {

    vector<vector<vector<bool>>> useMap = currentRotation ? currentMap3D : origMap3D;

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

void Shape::transposeMap(Event event) {
    switch (event)
    {
    case INPUT_UP: {
        vector<vector<vector<bool>>> newMap = vector<vector<vector<bool>>>(currentMap3D.size(), vector<vector<bool>>(currentMap3D[0][0].size(), vector<bool>(currentMap3D[0].size(), false)));
        for (int i = 0; i < newMap.size(); i++) {
            for (int j = 0; j < newMap[i].size(); j++) {
                for (int k = 0; k < newMap[i][j].size(); k++) {
                    newMap[i][j][k] = currentMap3D[i][currentMap3D[0].size() - 1 - k][j];
                }
            }
        }
        currentMap3D = newMap;
        break;
    }
    case INPUT_DOWN: {
        vector<vector<vector<bool>>> newMap = vector<vector<vector<bool>>>(currentMap3D.size(), vector<vector<bool>>(currentMap3D[0][0].size(), vector<bool>(currentMap3D[0].size(), false)));
        for (int i = 0; i < newMap.size(); i++) {
            for (int j = 0; j < newMap[i].size(); j++) {
                for (int k = 0; k < newMap[i][j].size(); k++) {
                    newMap[i][j][k] = currentMap3D[i][k][currentMap3D[0][0].size() - 1 - j];
                }
            }
        }
        currentMap3D = newMap;
        break;
    }
    case INPUT_LEFT: {
        vector<vector<vector<bool>>> newMap = vector<vector<vector<bool>>>(currentMap3D[0][0].size(), vector<vector<bool>>(currentMap3D[0].size(), vector<bool>(currentMap3D.size(), false)));
        for (int i = 0; i < newMap.size(); i++) {
            for (int j = 0; j < newMap[i].size(); j++) {
                for (int k = 0; k < newMap[i][j].size(); k++) {
                    newMap[i][j][k] = currentMap3D[k][j][currentMap3D[0][0].size() - 1 - i];
                }
            }
        }
        currentMap3D = newMap;
        break;
    }
    case INPUT_RIGHT: {
        vector<vector<vector<bool>>> newMap = vector<vector<vector<bool>>>(currentMap3D[0][0].size(), vector<vector<bool>>(currentMap3D[0].size(), vector<bool>(currentMap3D.size(), false)));
        for (int i = 0; i < newMap.size(); i++) {
            for (int j = 0; j < newMap[i].size(); j++) {
                for (int k = 0; k < newMap[i][j].size(); k++) {
                    newMap[i][j][k] = currentMap3D[currentMap3D.size() - 1 - k][j][i];
                }
            }
        }
        currentMap3D = newMap;
        break;
    }
    }
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

    origMap3D = map;
    currentMap3D = origMap3D;

    // Recenter shape for rotations
    centerOffset.x = (maxX - minX + 1) / 2;
    centerOffset.y = (maxY - minY + 1) / 2;
    centerOffset.z = (maxZ - minZ + 1) / 2;

    // Create voxels from map (to avoid duplicate coordinates in description)
    for (int i = 0; i < origMap3D.size(); i++) {
        for (int j = 0; j < origMap3D[i].size(); j++) {
            for (int k = 0; k < origMap3D[i][j].size(); k++) {
                if (origMap3D[i][j][k]) {
                    voxels.push_back(Voxel(vec3(i - centerOffset.x, j - centerOffset.y, k - centerOffset.z)));
                }
            }
        }
    }

    state = INITIALIZED;
}

void Shape::randomRightAngleRotations() {
    int randX = (rand() % 4);
    int randY = (rand() % 4);
    int randZ = (rand() % 4);
    
    _orientation = normalize(_orientation);
    _orientation = angleAxis(radians(randX * 90.0f), vec3(1.0f, 0.0f, 0.0f)) * _orientation;
    _orientation = angleAxis(radians(randY * 90.0f), vec3(0.0f, 1.0f, 0.0f)) * _orientation;
    _orientation = angleAxis(radians(randZ * 90.0f), vec3(0.0f, 0.0f, 1.0f)) * _orientation;

    for (int incrementer = 0; incrementer < randX; incrementer++) {
        vector<vector<vector<bool>>> newMap = vector<vector<vector<bool>>>(currentMap3D.size(), vector<vector<bool>>(currentMap3D[0][0].size(), vector<bool>(currentMap3D[0].size(), false)));
        for (int i = 0; i < newMap.size(); i++) {
            for (int j = 0; j < newMap[i].size(); j++) {
                for (int k = 0; k < newMap[i][j].size(); k++) {
                    newMap[i][j][k] = currentMap3D[i][k][currentMap3D[0][0].size() - 1 - j];
                }
            }
        }
        currentMap3D = newMap;
    }

    for (int incrementer = 0; incrementer < randY; incrementer++) {
        vector<vector<vector<bool>>> newMap = vector<vector<vector<bool>>>(currentMap3D[0][0].size(), vector<vector<bool>>(currentMap3D[0].size(), vector<bool>(currentMap3D.size(), false)));
        for (int i = 0; i < newMap.size(); i++) {
            for (int j = 0; j < newMap[i].size(); j++) {
                for (int k = 0; k < newMap[i][j].size(); k++) {
                    newMap[i][j][k] = currentMap3D[currentMap3D.size() - 1 - k][j][i];
                }
            }
        }
        currentMap3D = newMap;
    }

    for (int incrementer = 0; incrementer < randZ; incrementer++) {
        vector<vector<vector<bool>>> newMap = vector<vector<vector<bool>>>(currentMap3D[0].size(), vector<vector<bool>>(currentMap3D.size(), vector<bool>(currentMap3D[0][0].size(), false)));
        for (int i = 0; i < newMap.size(); i++) {
            for (int j = 0; j < newMap[i].size(); j++) {
                for (int k = 0; k < newMap[i][j].size(); k++) {
                    newMap[i][j][k] = currentMap3D[j][currentMap3D[0].size() - 1 - i][k];
                }
            }
        }
        currentMap3D = newMap;
    }

    displayOrientation = _orientation;
}
