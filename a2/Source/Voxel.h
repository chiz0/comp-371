#pragma once

#include <list>
#include <vector>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>

#include "ShaderManager.h"

using namespace glm;

class Voxel {
public:
	// Functions
	Voxel(vec3 position, int vao, vec3 localScale = vec3(1.0f, 1.0f, 1.0f));	// Default constructor
	void Draw(GLenum renderingMode, GLuint worldMatrixLocation);																		// Draw voxel to world

	// Properties
	mat4 mAnchor = mat4(1.0f);
	vec3 mPosition;
	vec3 mOrientation = vec3(0.0f, 0.0f, 0.0f);
	vec3 mScaleVector = vec3(1.0f, 1.0f, 1.0f);
	float mScale = 1.0f;
	int mvao;
};