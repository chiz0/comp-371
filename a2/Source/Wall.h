#pragma once

#include <list>
#include <vector>

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>

#include "Constants.h"
#include "Coordinates.h"
#include "ShaderManager.h"
#include "Voxel.h"
#include "Shape.h"

using namespace glm;
using namespace std;

class Wall {
public:
	// Functions
	Wall(vec3 position, Shape* shape, int vao);
	void Draw(GLenum renderingMode, ShaderManager shader);
	void ResetPosition();

	// Properties
	vector<struct coordinates> mDescription;
	vector<Voxel> voxels;
	vec3 mPosition;
	vec3 mOrientation = vec3(0.0f, 0.0f, 0.0f);
	float mScale = 1.0f;
	int mvao;
	vec3 defaultOrientation = vec3(0.0f, 0.0f, 0.0f);
	vec3 defaultPosition;
	float defaultScale = 1.0f;
};