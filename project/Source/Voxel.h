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
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "ShaderManager.h"

using namespace glm;


class Voxel {
public:
	// Functions
    Voxel(vec3 position);	// Default constructor

    void draw(GLenum* renderingMode, ShaderManager* shaderProgram);			// Draw voxel to world

	// Properties
    mat4 anchorMatrix = mat4(1.0f);
    vec3 displayPosition;
    quat displayOrientation = vec3(0);
    vec3 displayScale = vec3(1.0f);
    vec3 _position; // Used by Shape class to determine final position during animation
    vec3 _orientation = vec3(0); // Used by Shape class to determine final orientation during animation
    vec3 _scale = vec3(1.0f); // Used by Shape class to determine final scale during animation
};
