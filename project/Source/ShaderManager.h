#pragma once

// Shader manager class taken from Learn Open GL
// https://learnopengl.com/Getting-started/Shaders

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

class ShaderManager
{
public:
    ShaderManager(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);    // Default constructor
    void use();                                                         // Activate the shader

    // Pass uniform values to the shader
    void setBool(const string& name, bool value) const;
    void setInt(const string& name, int value) const;
    void setFloat(const string& name, float value) const;
    void setVec2(const string& name, const glm::vec2& value) const;
    void setVec2(const string& name, float x, float y) const;
    void setVec3(const string& name, const glm::vec3& value) const;
    void setVec3(const string& name, float value1, float value2, float value3) const;
    void setVec4(const string& name, const glm::vec4& value) const;
    void setVec4(const string& name, float x, float y, float z, float w);
    void setMat2(const string& name, const glm::mat2& mat) const;
    void setMat3(const string& name, const glm::mat3& mat) const;
    void setMat4(const string& name, const glm::mat4& mat) const;

private:
    unsigned int ID;                                                    // Shader program ID
};
