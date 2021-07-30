#include "ShaderManager.h"

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

void ShaderManager::use()
{
    glUseProgram(ID);
}

void ShaderManager::setBool(const string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
}

void ShaderManager::setInt(const string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void ShaderManager::setFloat(const string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

GLuint ShaderManager::getUniformLocation(const GLchar* key)
{
    return glGetUniformLocation(ID, key);
}
