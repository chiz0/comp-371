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
    // Properties
    unsigned int ID;                                                    // the program ID

    // Methods
    ShaderManager(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);    // Default constructor
    void use();                                                         // Activate the shader
    void setBool(const string& name, bool value) const;                 // Pass a boolean value to the shader
    void setInt(const string& name, int value) const;                   // Pass an int to the shader
    void setFloat(const string& name, float value) const;               // Pass a float to the shader
    void setVec3(const string& name, float value1, float value2, float value3) const;   // Pass a three floats to the shader
    GLuint getUniformLocation(const GLchar* key);

    void  setVec2(const std::string& name, const glm::vec2& value) const;
    void  setVec2(const std::string& name, float x, float y) const;
    void  setVec3(const std::string& name, const glm::vec3& value) const;
    void  setVec4(const std::string& name, const glm::vec4& value) const;
    void  setVec4(const std::string& name, float x, float y, float z, float w);
    void  setMat2(const std::string& name, const glm::mat2& mat) const;
    void  setMat3(const std::string& name, const glm::mat3& mat) const;
    void  setMat4(const std::string& name, const glm::mat4& mat) const;


private:
    int CompileShaderFromSource(string filepath, GLenum shaderType);
};
