#include "ShaderManager.h"

ShaderManager::ShaderManager(const char* vertexPath, const char* fragmentPath)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	string VertexShaderCode;
	ifstream VertexShaderStream(vertexPath, ios::in);
	if (VertexShaderStream.is_open()) {
		stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertexPath);
		getchar();
		return;
	}

	// Read the Fragment Shader code from the file
	string FragmentShaderCode;
	ifstream FragmentShaderStream(fragmentPath, ios::in);
	if (FragmentShaderStream.is_open()) {
		stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	cout << "Compiling shader : " << vertexPath << endl;
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	cout << "Compiling shader : " << fragmentPath << endl;
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

    ID = ProgramID;
}

void ShaderManager::use()
{
    glUseProgram(ID);
}

void ShaderManager::setBool(const string& name, bool value) const
{
    glUseProgram(ID);
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
}

void ShaderManager::setInt(const string& name, int value) const
{
    glUseProgram(ID);
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void ShaderManager::setFloat(const string& name, float value) const
{
    glUseProgram(ID);
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void ShaderManager::setVec3(const string& name, float value1, float value2, float value3) const 
{
    glUseProgram(ID);
    glUniform3f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3);
}

void ShaderManager::setMat4(const string& name, mat4 value) const
{
    glUseProgram(ID);
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

GLuint ShaderManager::getUniformLocation(const GLchar* key)
{
    glUseProgram(ID);
    return glGetUniformLocation(ID, key);
}
