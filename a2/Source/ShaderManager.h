#pragma once

// Shader manager class taken from Learn Open GL
// https://learnopengl.com/Getting-started/Shaders

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

class ShaderManager
{
public:
    // Properties
    unsigned int ID;                                                    // the program ID

    // Methods
    ShaderManager(const char* vertexPath, const char* fragmentPath);    // Default constructor
    void use();                                                         // Activate the shader
    void setBool(const string& name, bool value) const;                 // Pass a boolean value to the shader
    void setInt(const string& name, int value) const;                   // Pass an int to the shader
    void setFloat(const string& name, float value) const;               // Pass a float to the shader
    void setVec3(const string& name, float value1, float value2, float value3) const;   // Pass a three floats to the shader
    void ShaderManager::setMat4(const string& name, mat4 value) const;  // Pass a mat4 to the shader
    GLuint getUniformLocation(const GLchar* key);
};
