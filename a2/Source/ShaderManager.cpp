#include "ShaderManager.h"
#include <GL/glew.h>

ShaderManager::ShaderManager(const char* vertexPath, const char* fragmentPath)
{
    int vertexShader = CompileShaderFromSource(vertexPath, GL_VERTEX_SHADER);
    int fragmentShader = CompileShaderFromSource(fragmentPath, GL_FRAGMENT_SHADER);

    // Link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    ID = shaderProgram;
}

int ShaderManager::CompileShaderFromSource(string filepath, GLenum shaderType) {
    // Read from file
    ifstream sourceFile(filepath);
    string shaderSource = "";
    string sourceLine;
    if (sourceFile.is_open()) {
        while (getline(sourceFile, sourceLine)) {
            shaderSource += sourceLine + "\n";
        }
        sourceFile.close();
    }
    else {
        cout << "ERROR: Unable to open file.\n";
        return -1;
    }

    // Create and compile shader
    const GLchar* shaderSourcePointer = shaderSource.c_str();
    int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSourcePointer, NULL);
    glCompileShader(shader);

    // Check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Error: Shader compilation failed.\n" << infoLog << std::endl;
    }

    return shader;
}


void ShaderManager::setVec3(const string& name, float value1, float value2, float value3) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3);
}

GLuint ShaderManager::getUniformLocation(const GLchar* key)
{
    return glGetUniformLocation(ID, key);
}

// activate the shader
    // ------------------------------------------------------------------------
void ShaderManager::use()
{
    glUseProgram(ID);
}
// utility uniform functions
// ------------------------------------------------------------------------
void ShaderManager::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void ShaderManager::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void ShaderManager::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void ShaderManager::setVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void ShaderManager::setVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void ShaderManager::setVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
// ------------------------------------------------------------------------
void ShaderManager::setVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void ShaderManager::setVec4(const std::string& name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void ShaderManager::setMat2(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void ShaderManager::setMat3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void ShaderManager::setMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
