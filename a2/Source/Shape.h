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

using namespace glm;
using namespace std;

class Shape {
public:
	// Functions
	Shape(vec3 position, vector<coordinates> description, int vao, int glowVao, GLuint worldMatrixLocation, bool hasWall, float scalarScale);
	void BuildGlow(vector<coordinates> description, GLuint worldMatrixLocation);
	void Draw(GLenum renderingMode);
	void DrawGlow();
	void Reshuffle();
	void ResetPosition();

	// Properties
	bool showWall;
	int voxelCount = 0;
	bool projection[WALL_SIZE][WALL_SIZE] = { 0 };
	vector<struct coordinates> mDescription;
	vector<Voxel> voxels;
	vector<Voxel> wallVoxels;
	vector<Voxel> glowVoxels;
	vec3 mPosition;
	vec3 mOrientation = vec3(0.0f, 0.0f, 0.0f);
	float mScale = 1.0f;
	int mvao;
	int mGlowVao;
	GLuint mWorldMatrixLocation;
	vec3 defaultOrientation = vec3(0.0f, 0.0f, 0.0f);
	vec3 defaultPosition;
	float defaultScale = 1.0f;

private:
	bool isEdge(bool shapeMap[WALL_SIZE][WALL_SIZE][WALL_SIZE], coordinates fromShape, int dx, int dy, int dz);
};