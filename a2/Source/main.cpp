// COMP 371 Assignment 1
// Spiral Staircase (Section DD Team 3)
// 
// Badele, Theodor (40129466)
// Fourneaux, Alexander (40022711)
// Reda, Antonio (40155615)
// Zhang, Chi (29783539)
// 
// Based on Labs Framework created by Nicolas Bergeron
//
// CONTRIBUTIONS
//
//  Theodor
// 
//  Alexander
//
//  Antonio
//
//  Chi
//

#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // cross-platform interface for creating a graphical context,
						// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>

#include "Constants.h"
#include "Shape.h"
#include "ShaderManager.h"
#include "Coordinates.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace glm;
using namespace std;

GLuint loadTexture(const char* filename);
const char* getVertexShaderSource();

const char* getFragmentShaderSource();

const char* getTexturedVertexShaderSource();

const char* getTexturedFragmentShaderSource();

int compileAndLinkShaders(const char* vertexShaderSource, const char* fragmentShaderSource);

struct TexturedColoredVertex
{
	TexturedColoredVertex(vec3 _position, vec3 _color, vec2 _uv)
		: position(_position), color(_color), uv(_uv) {}

	vec3 position;
	vec3 color;
	vec2 uv;
};
// Textured Cube model
const TexturedColoredVertex texturedCubeVertexArray[] = {  // position,                            color
	TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)), //left - red
	TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)),

	TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f)),

	TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)), // far - blue
	TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f)),

	TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)),
	TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),

	TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)), // bottom - turquoise
	TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),

	TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
	TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),
	TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),

	TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)), // near - green
	TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)),

	TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)),
	TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)),

	TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)), // right - purple
	TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f)),

	TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)),
	TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f)),

	TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)), // top - yellow
	TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)),

	TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f))
};

int createTexturedCubeVertexArrayObject();

void setProjectionMatrix(int shaderProgram, mat4 projectionMatrix)
{
	glUseProgram(shaderProgram);
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void setViewMatrix(int shaderProgram, mat4 viewMatrix)
{
	glUseProgram(shaderProgram);
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
}

void setWorldMatrix(int shaderProgram, mat4 worldMatrix)
{
	glUseProgram(shaderProgram);
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
}




int createVertexArrayObject();
int createVertexArrayObjectColoured(vec3 frontBackColour, vec3 topBottomColour, vec3 leftRightColour);

bool initContext();

GLFWwindow* window = NULL;
/////////////////////// MAIN ///////////////////////

int main(int argc, char* argv[])
{

	// Initialize GLFW and OpenGL version
	glfwInit();

#if defined(PLATFORM_OSX)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
	// On windows, we set OpenGL version to 2.1, to support more hardware
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif

	// Create Window and rendering context using GLFW, resolution is 800x600
	GLFWwindow* window = glfwCreateWindow(800, 600, "Comp371 - Lab 03", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to create GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Load Textures
#if defined(PLATFORM_OSX)
	GLuint brickTextureID = loadTexture("Textures/brick.jpg");
	GLuint cementTextureID = loadTexture("Textures/cement.jpg");
#else
	GLuint brickTextureID = loadTexture("../Assets/Textures/brick.jpg");
	GLuint cementTextureID = loadTexture("../Assets/Textures/cement.jpg");
#endif


	if (!initContext()) return -1;

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Compile and link shaders here ...
	int colorShaderProgram = compileAndLinkShaders(getVertexShaderSource(), getFragmentShaderSource());
	int texturedShaderProgram = compileAndLinkShaders(getTexturedVertexShaderSource(), getTexturedFragmentShaderSource());
	
	// We can set the shader once, since we have only one
	ShaderManager shaderManager = ShaderManager(VERTEX_SHADER_FILEPATH, FRAGMENT_SHADER_FILEPATH);
	shaderManager.use();

	// Other camera parameters
	float cameraHorizontalAngle = 90.0f;
	float cameraVerticalAngle = 0.0f;

	// Set projection matrix for shader, this won't change
	mat4 projectionMatrix = glm::perspective(70.0f,            // field of view in degrees
		800.0f / 600.0f,  // aspect ratio
		0.01f, 100.0f);   // near and far (near > 0)

	GLuint projectionMatrixLocation = shaderManager.getUniformLocation("projectionMatrix");
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

	// Get the world matrix
	GLuint worldMatrixLocation = shaderManager.getUniformLocation("worldMatrix");
	
	// Camera parameters for view transform
	vec3 cameraPosition = vec3(0.0f, 1.0f, 20.0f);
	vec3 cameraLookAt(0.0f, -1.0f, 0.0f);
	vec3 cameraUp(0.0f, 1.0f, 0.0f);
	vec3 cameraDestination = cameraPosition;
	bool  cameraFirstPerson = true; // press 1 or 2 to toggle this variable

	// Set initial view matrix
	mat4 viewMatrix = lookAt(cameraPosition,  // eye
		cameraPosition + cameraLookAt,  // center
		cameraUp); // up

	// Set View and Projection matrices on both shaders
	setViewMatrix(colorShaderProgram, viewMatrix);
	setViewMatrix(texturedShaderProgram, viewMatrix);

	setProjectionMatrix(colorShaderProgram, projectionMatrix);
	setProjectionMatrix(texturedShaderProgram, projectionMatrix);

	// Define and upload geometry to the GPU here ...
	int texturedCubeVAO = createTexturedCubeVertexArrayObject();

	// For frame time
	float lastFrameTime = glfwGetTime();
	int lastMouseLeftState = GLFW_RELEASE;
	double lastMousePosX, lastMousePosY;
	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

	// Other OpenGL states to set once before the Game Loop
	// Enable Backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Enable Depth Test
	glEnable(GL_DEPTH_TEST);

	GLenum renderingMode = GL_TRIANGLES;
	glBindVertexArray(texturedCubeVAO);

	// Track models
	vector<Shape> shapes;               // Set of all shapes in the world

	///////// DESIGN MODELS HERE /////////
	// Chi shape
	vector<struct coordinates> chiShape{
		{ 0, 0, 0 },
		{ 0, 0, 1 },
		{ 0, 0, -1 },
		{ 0, 0, 2 },
		{ 0, 0, -2 },
		{ 0, 0, 3 },
		{ 0, 0, -3 },
		{ 1, 0, 0 },
		{ 1, 0, 1 },
		{ 1, 0, -1 },
		{ 1, 0, 2 },
		{ 1, 0, -2 },
		{ -1, 0, 0 },
		{ -1, 0, 1 },
		{ -1, 0, -1 },
		{ -1, 0, 2 },
		{ -1, 0, -2 },
		{ 2, 0, 0 },
		{ 2, 0, 1 },
		{ 2, 0, -1 },
		{ -2, 0, 0 },
		{ -2, 0, 1 },
		{ -2, 0, -1 },
		{ 3, 0, 0 },
		{ -3, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 1 },
		{ 0, 1, -1 },
		{ 0, 1, 2 },
		{ 0, 1, -2 },
		{ 1, 1, 0 },
		{ 1, 1, 1 },
		{ 1, 1, -1 },
		{ -1, 1, 0 },
		{ -1, 1, 1 },
		{ -1, 1, -1 },
		{ 2, 1, 0 },
		{ -2, 1, 0 },
		{ 0, 2, 0 },
		{ 0, 2, 1 },
		{ 0, 2, -1 },
		{ 1, 2, 0 },
		{ -1, 2, 0 },
		{ 0, 3, 0 }
	};

	vector<struct coordinates> alexShape{
		{ 1, 0, 0 },
		{ 1, -1, 0 },
		{ 1, -1, -1 },
		{ 0, -2, -1 },
		{ 1, -2, -1 },
		{ 1, 0, 1 },
		{ 0, 2, 1 },
		{ 0, 1, 1 },
		{ -1, 3, 1 },
		{ -1, 2, 1 },
		{ -1, 3, 0 },
		{ 1, 1, 1 }
	};

	vector<struct coordinates> theoShape{

		{ 0, 0, 0 },
		{ -1, 3, 2 },
		{ -1, 2, 2 },
		{ -1, 1, 2 },
		{ -1, 0, 2 },
		{ -1, -1, 2 },
		{ -1, -2, 2 },
		{ -1, -3, 2 },

		{ 0, 3, 0 },
		{ 1, 3, 0 },
		{ 0, 3, 2 },
		{ 2, 2, 2 },
		{ 2, 1, 2 },
		{ 1, 0, 0 },
		{ 0, -1, 2 },
		{ 1, -2, 2 },
		{ 2, -3, 1 },

		{ -1, 3, 1 },
		{ -1, 3, -1 },
		{ 2, 0, 1 },
		{ 2, -3, -1 },
		{ 1, -3, 0 }
	};

	vector<struct coordinates> antoShape{
		{ 0, 0, 0 },
		{ 0, 0, -1 },
		{ 0, 0, -2 },

		{ 1, 0, 0 },
		{ 1, 0, -1 },
		{ 1, 0, -2 },

		{ 1, 1, 0 },
		{ 1, 1, -1 },
		{ 1, 1, -2 },

		{ 1, -1, 0 },
		{ 1, -1, -1 },
		{ 1, -1, -2 },

		{ -1, 0, 0 },
		{ -1, 0, -1 },
		{ -1, 0, -2 },

		{ -1, 1, -1 },
		{ -1, 1, -2 },
		{ -1, 1, 0 },

		{ -1, -1, 0 },
		{ -1, -1, -1 },
		{ -1, -1, -2 },

		{ 0, -2, -1 },
		{ 0, -2, 0 },
		{ 0, -2, -2 },

		{ 0, 2, 0 },
		{ 0, 2, -1 },
		{ 0, 2, -2 }
	};

	// Colour of the shapes
	// Chi colour
	int chiColour = createVertexArrayObjectColoured(vec3(0.429f, 0.808f, 0.922f), vec3(0.248f, 0.511f, 0.804f), vec3(0.292f, 0.584f, 0.929f));
	// Alex colour
	int alexColour = createVertexArrayObjectColoured(vec3(0.984f, 0.761f, 0.016f), vec3(0.898f, 0.22f, 0), vec3(0.871f, 0.318f, 0.22f));
	// Theo colour
	int theoColour = createVertexArrayObjectColoured(vec3(1.0f, 0.15f, 0.0f), vec3(0.75f, 0.15f, 0.0f), vec3(0.75f, 0.15f, 0.15f));
	// Anto colour
	int antoColour = createVertexArrayObjectColoured(vec3(0.5f, 0.5f, 0.3f), vec3(0.7, 0.22f, 0), vec3(0.871f, 0.318f, 0.22f));

	shapes.push_back(Shape(vec3(STAGE_WIDTH, 10.0f, STAGE_WIDTH), chiShape, chiColour, worldMatrixLocation, true));
	shapes.push_back(Shape(vec3(-STAGE_WIDTH, 10.0f, STAGE_WIDTH), alexShape, alexColour, worldMatrixLocation, true));
	shapes.push_back(Shape(vec3(STAGE_WIDTH, 10.0f, -STAGE_WIDTH), theoShape, theoColour, worldMatrixLocation, true));
	shapes.push_back(Shape(vec3(-STAGE_WIDTH, 10.0f, -STAGE_WIDTH), antoShape, antoColour, worldMatrixLocation, true));

	int focusedShape = 0;                   // The shape currently being viewed and manipulated
	bool moveCameraToDestination = false;   // Tracks whether the camera is currently moving to a point

	const vector<vec3> cameraPositions{
		CAMERA_OFFSET + shapes[0].mPosition,
		CAMERA_OFFSET + shapes[1].mPosition,
		CAMERA_OFFSET + shapes[2].mPosition,
		CAMERA_OFFSET + shapes[3].mPosition
	};

	

	GLuint viewMatrixLocation = shaderManager.getUniformLocation("viewMatrix");
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

	// Position light source
	vec3 lightPosition = LIGHT_OFFSET;

	// Grid and coordinate axis colours
	int groundColour = createVertexArrayObjectColoured(vec3(1.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f));
	int xLineColour = createVertexArrayObjectColoured(vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f));
	int yLineColour = createVertexArrayObjectColoured(vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	int zLineColour = createVertexArrayObjectColoured(vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f));

	// Entering Game Loop
	while (!glfwWindowShouldClose(window))
	{
		// Frame time calculation
		float dt = glfwGetTime() - lastFrameTime;
		lastFrameTime += dt;

		// Each frame, reset color of each pixel to glClearColor
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw textured geometry
		glUseProgram(texturedShaderProgram);

		glActiveTexture(GL_TEXTURE0);
		GLuint textureLocation = glGetUniformLocation(texturedShaderProgram, "textureSampler");
		glBindTexture(GL_TEXTURE_2D, brickTextureID);
		glUniform1i(textureLocation, 0);           // Set our Texture sampler to user Texture Unit 0
		
		// Clear Depth Buffer Bit
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw ground
		glBindVertexArray(groundColour);
		mat4 groundWorldMatrix = translate(mat4(1.0f), vec3(0.0f, -0.01f, 0.0f)) * scale(mat4(1.0f), vec3(1000.0f, 0.02f, 1000.0f));
		setWorldMatrix(texturedShaderProgram, groundWorldMatrix);
		
		for (int i = -50; i <= 50; i++) {
			glBindTexture(GL_TEXTURE_2D, cementTextureID);
			mat4 gridMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, i)) * scale(mat4(1.0f), vec3(100.0f, 0.02f, 0.02f));
			setWorldMatrix(texturedShaderProgram, groundWorldMatrix);
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &gridMatrix[0][0]);
			glDrawArrays(renderingMode, 0, 36);
		}
		for (int i = -50; i <= 50; i++) {
			glBindTexture(GL_TEXTURE_2D, cementTextureID);
			mat4 gridMatrix = translate(mat4(1.0f), vec3(i, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.02f, 0.02f, 100.0f));
			setWorldMatrix(texturedShaderProgram, groundWorldMatrix);
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &gridMatrix[0][0]);
			glDrawArrays(renderingMode, 0, 36);
		}

		// Draw coordinate axes
		glBindVertexArray(xLineColour);
		mat4 xLine = translate(mat4(1.0f), vec3(2.5f, 0.1f, 0.0f)) * scale(mat4(1.0f), vec3(5.0f, 0.1f, 0.1f));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &xLine[0][0]);
		glDrawArrays(renderingMode, 0, 36);
		glBindVertexArray(yLineColour);
		mat4 yLine = translate(mat4(1.0f), vec3(0.0f, 2.6f, 0.0f)) * scale(mat4(1.0f), vec3(0.1f, 5.0f, 0.1f));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &yLine[0][0]);
		glDrawArrays(renderingMode, 0, 36);
		glBindVertexArray(zLineColour);
		mat4 zLine = translate(mat4(1.0f), vec3(0.0f, 0.1f, 2.5f)) * scale(mat4(1.0f), vec3(0.1f, 0.1f, 5.0f));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &zLine[0][0]);
		glDrawArrays(renderingMode, 0, 36);

		// Draw shape
		for (Shape shape : shapes) {
			shape.Draw(renderingMode);
		}

		glBindVertexArray(0);

		// End Frame
		glfwSwapBuffers(window);
		glfwPollEvents();

		// Handle inputs
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		// If shift is held, double camera speed
		bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
		float currentCameraSpeed = (fastCam) ? CAMERA_SPEED * 2 : CAMERA_SPEED;

		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) // FPview
		{
			cameraFirstPerson = true;
		}

		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) //TPview
		{
			cameraFirstPerson = false;
		}

		double mousePosX, mousePosY;
		glfwGetCursorPos(window, &mousePosX, &mousePosY);

		double dx = mousePosX - lastMousePosX;
		double dy = mousePosY - lastMousePosY;

		lastMousePosX = mousePosX;
		lastMousePosY = mousePosY;

		// Lock the camera rotation to be only when the middle and right button are pressed
		if (lastMouseLeftState == GLFW_RELEASE && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			cameraHorizontalAngle -= dx * CAMERA_ANGULAR_SPEED * dt;
		}
		if (lastMouseLeftState == GLFW_RELEASE && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
			cameraVerticalAngle -= dy * CAMERA_ANGULAR_SPEED * dt;
		}

		if (lastMouseLeftState == GLFW_RELEASE && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			if (!moveCameraToDestination) {
				if (dy < 0) {
					cameraPosition += currentCameraSpeed * cameraLookAt;
				}
				if (dy > 0) {
					cameraPosition -= currentCameraSpeed * cameraLookAt;
				}
			}
		}

		// Change orientation with the arrow keys
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			cameraFirstPerson = false;
			cameraHorizontalAngle -= CAMERA_ANGULAR_SPEED * dt;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			cameraFirstPerson = false;
			cameraHorizontalAngle += CAMERA_ANGULAR_SPEED * dt;
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			cameraFirstPerson = false;
			cameraVerticalAngle -= CAMERA_ANGULAR_SPEED * dt;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			cameraFirstPerson = false;
			cameraVerticalAngle += CAMERA_ANGULAR_SPEED * dt;
		}

		//Go Back to initial position and orientation
		if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) {
			cameraLookAt.x = 0.0f; cameraLookAt.y = 0.0f; cameraLookAt.z = -1.0f;
			cameraUp.x = 0.0f; cameraUp.y = 1.0f; cameraUp.z = 0.0f;

			//initial orientation
			cameraHorizontalAngle = 90.0f;
			cameraVerticalAngle = 0.0f;

			cameraDestination = vec3(0.0f, 1.0f, 20.0f);
			moveCameraToDestination = true;
		}

		// Clamp vertical angle to [-85, 85] degrees
		cameraVerticalAngle = std::max(-85.0f, std::min(85.0f, cameraVerticalAngle));
		if (cameraHorizontalAngle > 360)
		{
			cameraHorizontalAngle -= 360;
		}
		else if (cameraHorizontalAngle < -360)
		{
			cameraHorizontalAngle += 360;
		}

		float theta = radians(cameraHorizontalAngle);
		float phi = radians(cameraVerticalAngle);

		cameraLookAt = vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));
		vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));
		glm::normalize(cameraSideVector);

		// Select shapes via 1-4 keys
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			focusedShape = 0;
			cameraDestination = cameraPositions[0];
			moveCameraToDestination = true;
		}
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		{
			focusedShape = 1;
			cameraDestination = cameraPositions[1];
			moveCameraToDestination = true;
		}
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		{
			focusedShape = 2;
			cameraDestination = cameraPositions[2];
			moveCameraToDestination = true;
		}
		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		{
			focusedShape = 3;
			cameraDestination = cameraPositions[3];
			moveCameraToDestination = true;
		}

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // Ry
		{
			shapes[focusedShape].mOrientation.y += ROTATE_RATE * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) // R-y
		{
			shapes[focusedShape].mOrientation.y -= ROTATE_RATE * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) // Rz
		{
			shapes[focusedShape].mOrientation.z += ROTATE_RATE * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) // R-z
		{
			shapes[focusedShape].mOrientation.z -= ROTATE_RATE * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) // Rx
		{
			shapes[focusedShape].mOrientation.x += ROTATE_RATE * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) // R-x
		{
			shapes[focusedShape].mOrientation.x -= ROTATE_RATE * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) // grow object
		{
			shapes[focusedShape].mScale += SCALE_RATE * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) // shrink object
		{
			shapes[focusedShape].mScale -= SCALE_RATE * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // move object forward
		{
			shapes[focusedShape].mPosition.z -= TRANSLATE_RATE * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // move object backward
		{
			shapes[focusedShape].mPosition.z += TRANSLATE_RATE * dt;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // move object left
		{
			shapes[focusedShape].mPosition.x -= TRANSLATE_RATE * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // move object right
		{
			shapes[focusedShape].mPosition.x += TRANSLATE_RATE * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) // Set triangle rendering mode
		{
			renderingMode = GL_TRIANGLES;
		}

		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) // Set line rendering mode
		{
			renderingMode = GL_LINE_LOOP;
		}

		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) // Set point rendering mode
		{
			renderingMode = GL_POINTS;
		}

		// Reshuffle shape
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		{
			shapes[focusedShape].Reshuffle();
		}
		// Reset shape position and orientation
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			shapes[focusedShape].ResetPosition();
		}

		if (moveCameraToDestination) {
			// Slide the camera towards its destination
			vec3 cameraDelta = cameraDestination - cameraPosition;
			if (abs(cameraDelta.x) < 0.1 && abs(cameraDelta.y) < 0.1 && abs(cameraDelta.z) < 0.1) {
				// We have arrived at the destination
				cameraPosition = cameraDestination;
				moveCameraToDestination = false;
			}
			cameraPosition += cameraDelta * CAMERA_JUMP_SPEED * dt;

			// Reset camera orientation
			cameraLookAt.x = 0.0f; cameraLookAt.y = 0.0f; cameraLookAt.z = -1.0f;
			cameraUp.x = 0.0f; cameraUp.y = 1.0f; cameraUp.z = 0.0f;
			cameraHorizontalAngle = 90.0f;
			cameraVerticalAngle = 0.0f;
		}

		//Antonio's part
		mat4 viewMatrix = mat4(1.0);

		glm::vec3 position = cameraPosition;
		viewMatrix = lookAt(position, position + cameraLookAt, cameraUp);
		//if camera is third person, approximate the radius and give the world orientation perspective aimed at the origin {0,0,0}, Press N to normalize view to first person
		if (cameraFirstPerson) {
			viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
		}
		else {
			int newx;
			int newy;
			int newz;
			if (cameraPosition.x < 0) {
				newx = cameraPosition.x * -1;
			}
			else
				newx = cameraPosition.x;
			if (cameraPosition.y < 0)
			{
				newy = cameraPosition.y * -1;
			}
			else
				newy = cameraPosition.y;
			if (cameraPosition.z < 0)
			{
				newz = cameraPosition.z * -1;
			}
			else
				newz = cameraPosition.z;
			float radius = sqrt(pow(newx, 2) + pow(newy, 2) + pow(newz, 2));
			glm::vec3 position = vec3{ 0,1,0 } -radius * cameraLookAt;
			viewMatrix = lookAt(position, position + cameraLookAt, cameraUp);
		}
		GLuint viewMatrixLocation = shaderManager.getUniformLocation("viewMatrix");
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

	}

	// Shutdown GLFW
	glfwTerminate();

	return 0;
}
const char* getVertexShaderSource()
{
	// For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
	return
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;"
		"layout (location = 1) in vec3 aColor;"
		""
		"uniform mat4 worldMatrix;"
		"uniform mat4 viewMatrix = mat4(1.0);"  // default value for view matrix (identity)
		"uniform mat4 projectionMatrix = mat4(1.0);"
		""
		"out vec3 vertexColor;"
		"void main()"
		"{"
		"   vertexColor = aColor;"
		"   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
		"   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
		"}";
}

const char* getFragmentShaderSource()
{
	return
		"#version 330 core\n"
		"in vec3 vertexColor;"
		"out vec4 FragColor;"
		"void main()"
		"{"
		"   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);"
		"}";
}

const char* getTexturedVertexShaderSource()
{
	// For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
	return
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;"
		"layout (location = 1) in vec3 aColor;"
		"layout (location = 2) in vec2 aUV;"
		""
		"uniform mat4 worldMatrix;"
		"uniform mat4 viewMatrix = mat4(1.0);"  // default value for view matrix (identity)
		"uniform mat4 projectionMatrix = mat4(1.0);"
		""
		"out vec3 vertexColor;"
		"out vec2 vertexUV;"
		""
		"void main()"
		"{"
		"   vertexColor = aColor;"
		"   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
		"   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
		"   vertexUV = aUV;"
		"}";
}

const char* getTexturedFragmentShaderSource()
{
	return
		"#version 330 core\n"
		"in vec3 vertexColor;"
		"in vec2 vertexUV;"
		"uniform sampler2D textureSampler;"
		""
		"out vec4 FragColor;"
		"void main()"
		"{"
		"   vec4 textureColor = texture( textureSampler, vertexUV );"
		"   FragColor = textureColor * vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);"
		"}";
}

int compileAndLinkShaders(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	// compile and link shader program
	// return shader program id
	// ------------------------------------

	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

GLuint loadTexture(const char* filename)
{
	// Step1 Create and bind textures
	GLuint textureId = 0;
	glGenTextures(1, &textureId);
	assert(textureId != 0);


	glBindTexture(GL_TEXTURE_2D, textureId);

	// Step2 Set filter parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Step3 Load Textures with dimension data
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (!data)
	{
		std::cerr << "Error::Texture could not load texture file:" << filename << std::endl;
		return 0;
	}

	// Step4 Upload the texture to the PU
	GLenum format = 0;
	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 3)
		format = GL_RGB;
	else if (nrChannels == 4)
		format = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
		0, format, GL_UNSIGNED_BYTE, data);

	// Step5 Free resources
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureId;
}

int createTexturedCubeVertexArrayObject()
{
	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texturedCubeVertexArray), texturedCubeVertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		sizeof(TexturedColoredVertex), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TexturedColoredVertex),
		(void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2,                            // attribute 2 matches aUV in Vertex Shader
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TexturedColoredVertex),
		(void*)(2 * sizeof(vec3))      // uv is offseted by 2 vec3 (comes after position and color)
	);
	glEnableVertexAttribArray(2);

	return vertexArrayObject;
}

int createVertexArrayObjectColoured(vec3 frontBackColour, vec3 topBottomColour, vec3 leftRightColour)
{
	// Cube model

	// All arrays are in the following face order:
	// left
	// far
	// bottom
	// near
	// right
	// top

	// Verticies
	vec3 vertexArray[] = {
		vec3(-0.5f,-0.5f,-0.5f),
		vec3(-0.5f,-0.5f, 0.5f),
		vec3(-0.5f, 0.5f, 0.5f),
		vec3(-0.5f,-0.5f,-0.5f),
		vec3(-0.5f, 0.5f, 0.5f),
		vec3(-0.5f, 0.5f,-0.5f),

		vec3(0.5f, 0.5f,-0.5f),
		vec3(-0.5f,-0.5f,-0.5f),
		vec3(-0.5f, 0.5f,-0.5f),
		vec3(0.5f, 0.5f,-0.5f),
		vec3(0.5f,-0.5f,-0.5f),
		vec3(-0.5f,-0.5f,-0.5f),

		vec3(0.5f,-0.5f, 0.5f),
		vec3(-0.5f,-0.5f,-0.5f),
		vec3(0.5f,-0.5f,-0.5f),
		vec3(0.5f,-0.5f, 0.5f),
		vec3(-0.5f,-0.5f, 0.5f),
		vec3(-0.5f,-0.5f,-0.5f),

		vec3(-0.5f, 0.5f, 0.5f),
		vec3(-0.5f,-0.5f, 0.5f),
		vec3(0.5f,-0.5f, 0.5f),
		vec3(0.5f, 0.5f, 0.5f),
		vec3(-0.5f, 0.5f, 0.5f),
		vec3(0.5f,-0.5f, 0.5f),

		vec3(0.5f, 0.5f, 0.5f),
		vec3(0.5f,-0.5f,-0.5f),
		vec3(0.5f, 0.5f,-0.5f),
		vec3(0.5f,-0.5f,-0.5f),
		vec3(0.5f, 0.5f, 0.5f),
		vec3(0.5f,-0.5f, 0.5f),

		vec3(0.5f, 0.5f, 0.5f),
		vec3(0.5f, 0.5f,-0.5f),
		vec3(-0.5f, 0.5f,-0.5f),
		vec3(0.5f, 0.5f, 0.5f),
		vec3(-0.5f, 0.5f,-0.5f),
		vec3(-0.5f, 0.5f, 0.5f)
	};

	// Colours
	vec3 colourVertexArray[] = {
		leftRightColour,
		leftRightColour,
		leftRightColour,
		leftRightColour,
		leftRightColour,
		leftRightColour,

		frontBackColour,
		frontBackColour,
		frontBackColour,
		frontBackColour,
		frontBackColour,
		frontBackColour,

		topBottomColour,
		topBottomColour,
		topBottomColour,
		topBottomColour,
		topBottomColour,
		topBottomColour,

		frontBackColour,
		frontBackColour,
		frontBackColour,
		frontBackColour,
		frontBackColour,
		frontBackColour,

		leftRightColour,
		leftRightColour,
		leftRightColour,
		leftRightColour,
		leftRightColour,
		leftRightColour,

		topBottomColour,
		topBottomColour,
		topBottomColour,
		topBottomColour,
		topBottomColour,
		topBottomColour
	};

	// Normals
	vec3 normalArray[] = {
		vec3(0.0f, 0.0f, -1.0f),
		vec3(0.0f, 0.0f, -1.0f),
		vec3(0.0f, 0.0f, -1.0f),
		vec3(0.0f, 0.0f, -1.0f),
		vec3(0.0f, 0.0f, -1.0f),
		vec3(0.0f, 0.0f, -1.0f),

		vec3(-1.0f, 0.0f, 0.0f),
		vec3(-1.0f, 0.0f, 0.0f),
		vec3(-1.0f, 0.0f, 0.0f),
		vec3(-1.0f, 0.0f, 0.0f),
		vec3(-1.0f, 0.0f, 0.0f),
		vec3(-1.0f, 0.0f, 0.0f),

		vec3(0.0f, -1.0f, 0.0f),
		vec3(0.0f, -1.0f, 0.0f),
		vec3(0.0f, -1.0f, 0.0f),
		vec3(0.0f, -1.0f, 0.0f),
		vec3(0.0f, -1.0f, 0.0f),
		vec3(0.0f, -1.0f, 0.0f),

		vec3(1.0f, 0.0f, 0.0f),
		vec3(1.0f, 0.0f, 0.0f),
		vec3(1.0f, 0.0f, 0.0f),
		vec3(1.0f, 0.0f, 0.0f),
		vec3(1.0f, 0.0f, 0.0f),
		vec3(1.0f, 0.0f, 0.0f),

		vec3(0.0f, 0.0f, 1.0f),
		vec3(0.0f, 0.0f, 1.0f),
		vec3(0.0f, 0.0f, 1.0f),
		vec3(0.0f, 0.0f, 1.0f),
		vec3(0.0f, 0.0f, 1.0f),
		vec3(0.0f, 0.0f, 1.0f),

		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f)
	};

	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);


	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,    // attribute 0 matches aPos in Vertex Shader
		3,						// size
		GL_FLOAT,				// type
		GL_FALSE,				// normalized?
		sizeof(TexturedColoredVertex),			// stride
		(void*)0				// array buffer offset
	);
	glEnableVertexAttribArray(0);

	GLuint colourVertexBufferObject;
	glGenBuffers(1, &colourVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, colourVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colourVertexArray), colourVertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(1,	// attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TexturedColoredVertex),
		(void*)sizeof(vec3)
	);
	glEnableVertexAttribArray(1);


	GLuint normalBufferObject;
	glGenBuffers(1, &normalBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normalArray), normalArray, GL_STATIC_DRAW);

	glVertexAttribPointer(2,	// attribute 2 matches aNormal in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vec3),
		(void*)0
	);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	return vertexArrayObject;
}

int createVertexArrayObject()
{
	// Cube model
	vec3 vertexArray[] = {  // position,                            color
		vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), //left - red
		vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),

		vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),

		vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), // far - blue
		vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
		vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),

		vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
		vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
		vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),

		vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), // bottom - turquoise
		vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f),
		vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f),

		vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f),
		vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f),
		vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f),

		vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), // near - green
		vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),

		vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),

		vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), // right - purple
		vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),
		vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),

		vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),
		vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f),
		vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f),

		vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), // top - yellow
		vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f),

		vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f)
	};

	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);


	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		2 * sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(vec3),
		(void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBindVertexArray(0);

	return vertexArrayObject;
}


bool initContext() {     // Initialize GLFW and OpenGL version
	glfwInit();

#if defined(PLATFORM_OSX)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
	// On windows, we set OpenGL version to 2.1, to support more hardware
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif

	// Create Window and rendering context using GLFW, resolution is 1024x768
	window = glfwCreateWindow(1024, 768, "COMP 371 - Assignment 1 by Spiral Staircase", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	// The next line disables the mouse cursor
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// 
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to create GLEW" << std::endl;
		glfwTerminate();
		return false;
	}
	return true;
}