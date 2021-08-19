#include "TerrainComponent.h"

TerrainComponent::TerrainComponent(vector<ivec3> description, vec3 colour, int texture, int chunk): _colour(colour), _texture(texture), _chunk(chunk) {
    if (description.empty()) {
        description = { { 0, 0, 0 } };
    }

	for (ivec3 coords : description) {
		voxels.push_back(Voxel(vec3(coords.x, coords.y, coords.z)));
	}
}

void TerrainComponent::draw(GLenum* renderingMode, ShaderManager* shaderProgram) {
	//quat orientationQuat(_orientation);
	//mat4 worldMatrix = anchorMatrix * translate(mat4(1.0f), _position) * toMat4(orientationQuat) * scale(mat4(1.0f), _scale);
	mat4 worldMatrix = anchorMatrix * translate(mat4(1.0f), _position);
	shaderProgram->setVec3("colour", _colour);
	for (auto it = begin(voxels); it != end(voxels); ++it) {
		it->anchorMatrix = worldMatrix;
		it->draw(renderingMode, shaderProgram);
	}
}
