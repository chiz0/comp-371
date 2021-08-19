#include "Voxel.h"

Voxel::Voxel(vec3 position) : _position(position), displayPosition(position) {}

void Voxel::draw(GLenum* renderingMode, ShaderManager* shaderProgram) {
    //quat orientationQuat(displayOrientation);
    //mat4 worldMatrix = anchorMatrix * translate(mat4(1.0f), displayPosition) * toMat4(orientationQuat) * scale(mat4(1.0f), displayScale);
    mat4 worldMatrix = anchorMatrix * translate(mat4(1.0f), displayPosition);
    shaderProgram->setMat4("worldMatrix", worldMatrix);
    glDrawArrays(*renderingMode, 0, 36);
}
