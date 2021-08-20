#pragma once
#include <list>
#include <vector>

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Constants.h"
#include "ShaderManager.h"
#include "Mesh.h"

using namespace Assimp;
using namespace glm;
using namespace std;

class Model
{
public:
    Model(char* path, mat4 position, int chunk);
    void Draw(ShaderManager* shader);

    int _chunk = 0;
    mat4 anchorMatrix=mat4(1.0f);;
    
private:
    // model data
    vector<Mesh> meshes;
    vector<Texture> textures_loaded;
    string directory;
    mat4 position;
    
    void loadModel(string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
    unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);
};