#pragma once

#include <list>
#include <vector>

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>

#include "Constants.h"
#include "ShaderManager.h"

using namespace glm;
using namespace std;

struct Vertex {
    vec3 Position;
    vec3 Colour;
    vec3 Normal;
    vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    string type;
    string path;  // we store the path of the texture to compare with other textures
};

class Mesh {
public:
    Mesh(vector<Vertex> mVertices, vector<unsigned int> mIndices, vector<Texture> mTextures);
    void Draw(ShaderManager* shader, mat4 position);

private:
    vector<Vertex> mVertices;
    vector<unsigned int> mIndices;
    vector<Texture> mTextures;
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};