// COMP 371 Final project
// Spiral Staircase (Section DD Team 3)
// 
// Badele, Theodor (40129466)
// Fourneaux, Alexander (40022711)
// Reda, Antonio (40155615)
// Zhang, Chi (29783539)
// 
// Based on Labs Framework created by Nicolas Bergeron
// Lighting implemented via a guide on https://learnopengl.com
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
#include <glm/gtx/string_cast.hpp> 
#include <time.h>
#include <irrKlang.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Constants.h"
#include "Shape.h"
#include "ShaderManager.h"
#include "Coordinates.h"
#include "ControlState.h"
#include "Wall.h"
#include "Emitter.h"

using namespace glm;
using namespace std;
using namespace irrklang;

/////////////////////// MAIN ///////////////////////

void windowSizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

int createVertexArrayObjectColoured(vec3 frontBackColour, vec3 topBottomColour, vec3 leftRightColour);
int createVertexArrayObjectSingleColoured(vec3 colour);
int createVertexArrayObjectTextured(vec3 colour);
int loadTexture(string name, char* path);
void drawScene(ShaderManager shaderManager, GLenum renderingMode, vector<Shape> shapes, vector<Wall> walls, Shape lightbulb, int tileTexture);

bool initContext();

GLFWwindow* window = NULL;

int xLineColour;
int yLineColour;
int zLineColour;


int main(int argc, char* argv[])
{
	if (!initContext()) return -1;

	srand(time(NULL));

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// We can set the shader once, since we have only one
	ShaderManager shaderManager = ShaderManager(VERTEX_SHADER_FILEPATH, FRAGMENT_SHADER_FILEPATH);
	ShaderManager shadowShaderManager = ShaderManager(SHADOW_VERTEX_SHADER_FILEPATH, SHADOW_FRAGMENT_SHADER_FILEPATH, SHADOW_DEPTH_SHADER_FILEPATH);
	ShaderManager particleShaderManager = ShaderManager(PARTICLE_VERTEX_SHADER_FILEPATH, PARTICLE_FRAGMENT_SHADER_FILEPATH);


	// configure depth map FBO
	// -----------------------
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth cubemap texture
	unsigned int depthCubemap;
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Shader Configuration
	shaderManager.use();
	shaderManager.setInt("depthMap", 1);

	//load the texture
	int tileTexture = loadTexture("tileTexture", TEXTURE_PATH_TILE);
	int metalTexture = loadTexture("metalTexture", TEXTURE_PATH_METAL);
	int brickTexture = loadTexture("brickTexture", TEXTURE_PATH_BRICK);
	int fireTexture = loadTexture("fireTexture", TEXTURE_PATH_FIRE);
	int particleTexture = loadTexture("particleTexture", TEXTURE_PATH_PARTICLE);

	// Other camera parameters
	float cameraHorizontalAngle = 90.0f;
	float cameraVerticalAngle = 0.0f;
	int windowWidth, windowHeight;

	float fieldOfView = FIELD_OF_VIEW;
	mat4 projectionMatrix = perspective(radians(fieldOfView),   // field of view in degrees
		VIEW_WIDTH / VIEW_HEIGHT,								// aspect ratio
		NEAR_PLANE, FAR_PLANE);									// near and far (near > 0)

	glfwSetWindowSizeCallback(window, &windowSizeCallback);

	shaderManager.setMat4("projectionMatrix", projectionMatrix);

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
	glBindTexture(GL_TEXTURE_2D, metalTexture);
	// Track models
	vector<Shape> shapes;               // Set of all shapes in the world
	vector<Wall> walls;					// Set of all walls

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
		{ 0, 0, 0 },
		{ 0, 0, 1 },

		{ 0, -1, 0 },
		{ 0, -1, 1 },

		{ 1, -1, 0 },
		{ 1, -1, 1 },

		{ 2, -1, 0 },
		{ 2, -1, 1 },

		{ 1, -2, 1 },
		{ 2, -2, 1 },

		{ 1, -2, 2 },
		{ 2, -2, 2 },

		{ 1, -3, 2 },
		{ 2, -3, 2 },

		{ 1, -3, 3 },
		{ 2, -3, 3 },

		{ -1, 0, 0 },
		{ -1, 0, 1 },

		{ -1, 1, 0 },
		{ -1, 1, 1 },

		{ -2, 1, 0 },
		{ -2, 1, 1 },

		{ -2, 2, 0 },
		{ -2, 2, 1 },

		{ -3, 2, 0 },
		{ -3, 2, 1 },

		{ -2, 2, 2 },
		{ -3, 2, 2 },

		{ -2, 3, 2 },
		{ -3, 3, 2 },

		{ -2, 3, 3 },
		{ -3, 3, 3 },

		{ -2, 4, 3 },
		{ -3, 4, 3 },
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

	vector<struct coordinates> lightbulbShape{
		{0, 0, 0}
	};


	int groundColour = createVertexArrayObjectSingleColoured(vec3(1.0f, 1.0f, 0.0f));
	// Colour of the shapes
	// Chi colour
	int chiColour = createVertexArrayObjectTextured(vec3(0.429f, 0.808f, 0.922f));
	// Alex colour
	int alexColour = createVertexArrayObjectTextured(vec3(1.0f, 0.58f, 0.25f));
	// Theo colour
	int theoColour = createVertexArrayObjectTextured(vec3(1.0f, 0.15f, 0.0f));
	// Anto colour
	int antoColour = createVertexArrayObjectTextured(vec3(0.5f, 0.5f, 0.3f));
	// Lightbulb colour
	int lightbulbColour = createVertexArrayObjectTextured(vec3(1.0f, 1.0f, 1.0f));
	int wallColour = createVertexArrayObjectTextured(vec3(0.8f, 0.2f, 0.2f));
	int tileColour = createVertexArrayObjectTextured(vec3(1.0f, 1.0f, 1.0f));
	int glowColour = createVertexArrayObjectTextured(vec3(1.0f, 1.0f, 1.0f));

	shapes.push_back(Shape(vec3(STAGE_WIDTH, 10.0f, STAGE_WIDTH), chiShape, chiColour, glowColour, false, 1.0f, metalTexture, fireTexture));
	shapes.push_back(Shape(vec3(-STAGE_WIDTH, 10.0f, STAGE_WIDTH), alexShape, alexColour, glowColour, false, 1.0f, metalTexture, fireTexture));
	shapes.push_back(Shape(vec3(STAGE_WIDTH, 10.0f, -STAGE_WIDTH), theoShape, theoColour, glowColour, false, 1.0f, metalTexture, fireTexture));
	shapes.push_back(Shape(vec3(-STAGE_WIDTH, 10.0f, -STAGE_WIDTH), antoShape, antoColour, glowColour, false, 1.0f, metalTexture, fireTexture));
	Shape lightbulb = Shape(vec3(0.0f, 0.0f, 0.0f), lightbulbShape, lightbulbColour, glowColour, false, 1.0f, metalTexture, fireTexture);

	for (int i = 0; i < MODEL_COUNT; i++) {
		walls.push_back(Wall(vec3(shapes[i].mPosition.x, shapes[i].mPosition.y, shapes[i].mPosition.z - WALL_DISTANCE), &(shapes[i]), wallColour, brickTexture));
	}

	int focusedShape = 0;                   // The shape currently being viewed and manipulated
	bool moveCameraToDestination = false;   // Tracks whether the camera is currently moving to a point
	bool showTexture = true;
	bool showShadow = true;
	ControlState controlState = { &shapes, &focusedShape, &showTexture, &showShadow, &fieldOfView };
	glfwSetWindowUserPointer(window, &controlState);

	const vector<vec3> cameraPositions{
		CAMERA_OFFSET + shapes[0].mPosition,
		CAMERA_OFFSET + shapes[1].mPosition,
		CAMERA_OFFSET + shapes[2].mPosition,
		CAMERA_OFFSET + shapes[3].mPosition
	};

	// Camera parameters for view transform
	vec3 cameraPosition = vec3(0.0f, 20.0f, 20.0f);
	vec3 cameraLookAt(0.0f, -1.0f, 0.0f);
	vec3 cameraUp(0.0f, 1.0f, 0.0f);
	vec3 cameraDestination = cameraPosition;
	bool  cameraFirstPerson = true; // press 1 or 2 to toggle this variable
	shaderManager.setVec3("cameraPosition", cameraPosition);

	// Set initial view matrix
	mat4 viewMatrix = lookAt(cameraPosition,  // eye
		cameraPosition + cameraLookAt,  // center
		cameraUp); // up

	shaderManager.setMat4("viewMatrix", viewMatrix);

	// Set up lighting
	vec3 lightPosition = LIGHT_OFFSET;
	shaderManager.setVec3("lightPosition", lightPosition);
	shaderManager.setVec3("lightColour", 1.0f, 1.0f, 1.0f);
	shaderManager.setFloat("ambientLight", LIGHT_AMBIENT_STRENGTH);
	shaderManager.setFloat("diffuseLight", LIGHT_DIFFUSE_STRENGTH);
	shaderManager.setFloat("specularLight", LIGHT_SPECULAR_STRENGTH);
	shaderManager.setFloat("shininess", SHININESS);
	shaderManager.setInt("depthMap", 1);

	// Grid and coordinate axis colours
	xLineColour = createVertexArrayObjectSingleColoured(vec3(1.0f, 0.0f, 0.0f));
	yLineColour = createVertexArrayObjectSingleColoured(vec3(0.0f, 1.0f, 0.0f));
	zLineColour = createVertexArrayObjectSingleColoured(vec3(0.0f, 0.0f, 1.0f));

	// Register keypress event callback
	glfwSetKeyCallback(window, &keyCallback);


	mat4 shadowProjection = perspective(radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, NEAR_PLANE, FAR_PLANE);

	int particleVAO = createVertexArrayObjectTextured(vec3(1.0f, 1.0f, 1.0f));
	Emitter emitter = Emitter(particleVAO);

	// Sound settings
	ISoundEngine* soundEngine = createIrrKlangDevice();

	if (!soundEngine)
	{
		cout << ("WARNING: Could not start sound engine") << endl;
	}

	soundEngine->play2D(AUDIO_PATH_MUSIC, true);

	// Entering Game Loop
	while (!glfwWindowShouldClose(window))
	{
		shaderManager.setBool("texToggle", showTexture);
		shaderManager.setBool("showShadows", showShadow);
		// Frame time calculation
		float dt = glfwGetTime() - lastFrameTime;
		lastFrameTime += dt;

		lightbulb.mPosition = lightPosition;

		// Clear Depth Buffer Bit
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 0. create depth cubemap transformation matrices
		// -----------------------------------------------
		mat4 shadowTransforms[6] = {
			shadowProjection * lookAt(lightPosition, lightPosition + vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)),
			shadowProjection * lookAt(lightPosition, lightPosition + vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)),
			shadowProjection * lookAt(lightPosition, lightPosition + vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),
			shadowProjection * lookAt(lightPosition, lightPosition + vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)),
			shadowProjection * lookAt(lightPosition, lightPosition + vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f)),
			shadowProjection * lookAt(lightPosition, lightPosition + vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f))
		};

		// 1. render scene to depth cubemap
		// --------------------------------
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		shadowShaderManager.use();
		for (unsigned int i = 0; i < 6; ++i)
			shadowShaderManager.setMat4("shadowMatrices[" + to_string(i) + "]", shadowTransforms[i]);
		shadowShaderManager.setFloat("farPlane", FAR_PLANE);
		shadowShaderManager.setVec3("lightPosition", lightPosition);
		drawScene(shadowShaderManager, renderingMode, shapes, walls, lightbulb, tileTexture);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 2. render scene as normal 
		// -------------------------
		glfwGetWindowSize(window, &windowWidth, &windowHeight);
		glViewport(0, 0, windowWidth, windowHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderManager.use();
		// set lighting uniforms
		shaderManager.setVec3("lightPosition", lightPosition);
		shaderManager.setVec3("viewPos", cameraPosition);
		shaderManager.setFloat("farPlane", FAR_PLANE);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		drawScene(shaderManager, renderingMode, shapes, walls, lightbulb, tileTexture);

		// Update and draw particles
		emitter.Update(dt);
		emitter.Draw(shaderManager);
		
		shaderManager.use();

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
				if (dy > 0) {
					fieldOfView += currentCameraSpeed;
					fieldOfView = clamp(fieldOfView, 10.0f, 130.0f);
					mat4 projectionMatrix = perspective(radians(fieldOfView),  // field of view in degrees
						(float)windowWidth / windowHeight,      // aspect ratio
						NEAR_PLANE, FAR_PLANE);
					shaderManager.setMat4("projectionMatrix", projectionMatrix);

					//cameraPosition += currentCameraSpeed * cameraLookAt;
				}
				if (dy < 0) {
					fieldOfView -= currentCameraSpeed;
					fieldOfView = clamp(fieldOfView, 10.0f, 130.0f);
					mat4 projectionMatrix = perspective(radians(fieldOfView),  // field of view in degrees
						(float)windowWidth / windowHeight,      // aspect ratio
						NEAR_PLANE, FAR_PLANE);
					shaderManager.setMat4("projectionMatrix", projectionMatrix);
					//cameraPosition -= currentCameraSpeed * cameraLookAt;
				}
				shaderManager.setVec3("cameraPosition", cameraPosition);
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
			lightPosition = cameraDestination + LIGHT_OFFSET;
			shaderManager.setVec3("lightPosition", lightPosition);
			moveCameraToDestination = true;

			fieldOfView = FIELD_OF_VIEW;
			mat4 projectionMatrix = perspective(radians(fieldOfView),  // field of view in degrees
				VIEW_WIDTH / VIEW_HEIGHT,      // aspect ratio
				NEAR_PLANE, FAR_PLANE);       // near and far (near > 0)

			shaderManager.setMat4("projectionMatrix", projectionMatrix);
		}

		// Clamp vertical angle to [-85, 85] degrees
		cameraVerticalAngle = clamp(cameraVerticalAngle, -85.0f, 85.0f);

		// Hacky modulus operation
		while (cameraHorizontalAngle > 360)
		{
			cameraHorizontalAngle -= 360;
		}
		while (cameraHorizontalAngle < -360)
		{
			cameraHorizontalAngle += 360;
		}

		float theta = radians(cameraHorizontalAngle);
		float phi = radians(cameraVerticalAngle);

		cameraLookAt = vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));

		// Select shapes via 1-4 keys
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			focusedShape = 0;
			cameraDestination = cameraPositions[0];
			lightPosition = cameraDestination + LIGHT_OFFSET;
			moveCameraToDestination = true;
			shaderManager.setVec3("lightPosition", lightPosition);
		}
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		{
			focusedShape = 1;
			cameraDestination = cameraPositions[1];
			lightPosition = cameraDestination + LIGHT_OFFSET;
			moveCameraToDestination = true;
			shaderManager.setVec3("lightPosition", lightPosition);
		}
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		{
			focusedShape = 2;
			cameraDestination = cameraPositions[2];
			lightPosition = cameraDestination + LIGHT_OFFSET;
			moveCameraToDestination = true;
			shaderManager.setVec3("lightPosition", lightPosition);
		}
		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		{
			focusedShape = 3;
			cameraDestination = cameraPositions[3];
			lightPosition = cameraDestination + LIGHT_OFFSET;
			moveCameraToDestination = true;
			shaderManager.setVec3("lightPosition", lightPosition);
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS) // Ry
		{
			shapes[focusedShape].mOrientation.y += ROTATE_RATE * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS) // R-y
		{
			shapes[focusedShape].mOrientation.y -= ROTATE_RATE * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) // Rz
		{
			shapes[focusedShape].mOrientation.z += ROTATE_RATE * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS) // R-z
		{
			shapes[focusedShape].mOrientation.z -= ROTATE_RATE * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_APOSTROPHE) == GLFW_PRESS) // Rx
		{
			shapes[focusedShape].mOrientation.x += ROTATE_RATE * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_PRESS) // R-x
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

		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
			emitter.Emit(vec3(0.0f, 20.0f, 0.0f), 50.0f, 10.0f, particleTexture);
			soundEngine->play2D(AUDIO_PATH_WOW);
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
			cameraPosition += cameraDelta * glm::min(CAMERA_JUMP_SPEED * dt, 1.0f);

			// Reset camera orientation
			cameraLookAt.x = 0.0f; cameraLookAt.y = 0.0f; cameraLookAt.z = -1.0f;
			cameraUp.x = 0.0f; cameraUp.y = 1.0f; cameraUp.z = 0.0f;
			cameraHorizontalAngle = 90.0f;
			cameraVerticalAngle = 0.0f;
			shaderManager.setVec3("cameraPosition", cameraPosition);
		}

		//Antonio's part
		vec3 position = cameraPosition;
		viewMatrix = lookAt(position, position + cameraLookAt, cameraUp);
		//if camera is third person, approximate the radius and give the world orientation perspective aimed at the origin {0,0,0}, Press N to normalize view to first person
		if (cameraFirstPerson) {
			viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
		}
		else {
			float radius = sqrt(pow(cameraPosition.x, 2) + pow(cameraPosition.y, 2) + pow(cameraPosition.z, 2));
			vec3 position = vec3(0.0f, 1.0f, 0.0f) - radius * cameraLookAt;
			viewMatrix = lookAt(position, position + cameraLookAt, cameraUp);
			shaderManager.setVec3("cameraPosition", cameraPosition);
		}
		shaderManager.setMat4("viewMatrix", viewMatrix);

	}

	// Shutdown GLFW
	glfwTerminate();

	return 0;
}

int createVertexArrayObjectColoured(vec3 frontBackColour, vec3 topBottomColour, vec3 leftRightColour)
{
	// Cube model
	// Vertex, colour, normal
	vec3 unitCube[] = {
		vec3(-0.5f,-0.5f,-0.5f), leftRightColour, vec3(-1.0f, 0.0f, 0.0f), // left
		vec3(-0.5f,-0.5f, 0.5f), leftRightColour, vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), leftRightColour, vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f,-0.5f,-0.5f), leftRightColour, vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), leftRightColour, vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f, 0.5f,-0.5f), leftRightColour, vec3(-1.0f, 0.0f, 0.0f),

		vec3(0.5f, 0.5f,-0.5f), frontBackColour, vec3(0.0f, 0.0f, -1.0f), // far
		vec3(-0.5f,-0.5f,-0.5f), frontBackColour, vec3(0.0f, 0.0f, -1.0f),
		vec3(-0.5f, 0.5f,-0.5f), frontBackColour, vec3(0.0f, 0.0f, -1.0f),
		vec3(0.5f, 0.5f,-0.5f), frontBackColour, vec3(0.0f, 0.0f, -1.0f),
		vec3(0.5f,-0.5f,-0.5f), frontBackColour, vec3(0.0f, 0.0f, -1.0f),
		vec3(-0.5f,-0.5f,-0.5f), frontBackColour, vec3(0.0f, 0.0f, -1.0f),

		vec3(0.5f,-0.5f, 0.5f), topBottomColour, vec3(0.0f, -1.0f, 0.0f), // bottom
		vec3(-0.5f,-0.5f,-0.5f), topBottomColour, vec3(0.0f, -1.0f, 0.0f),
		vec3(0.5f,-0.5f,-0.5f), topBottomColour, vec3(0.0f, -1.0f, 0.0f),
		vec3(0.5f,-0.5f, 0.5f), topBottomColour, vec3(0.0f, -1.0f, 0.0f),
		vec3(-0.5f,-0.5f, 0.5f), topBottomColour, vec3(0.0f, -1.0f, 0.0f),
		vec3(-0.5f,-0.5f,-0.5f), topBottomColour, vec3(0.0f, -1.0f, 0.0f),

		vec3(-0.5f, 0.5f, 0.5f), frontBackColour, vec3(0.0f, 0.0f, 1.0f), // near
		vec3(-0.5f,-0.5f, 0.5f), frontBackColour, vec3(0.0f, 0.0f, 1.0f),
		vec3(0.5f,-0.5f, 0.5f), frontBackColour, vec3(0.0f, 0.0f, 1.0f),
		vec3(0.5f, 0.5f, 0.5f), frontBackColour, vec3(0.0f, 0.0f, 1.0f),
		vec3(-0.5f, 0.5f, 0.5f), frontBackColour, vec3(0.0f, 0.0f, 1.0f),
		vec3(0.5f,-0.5f, 0.5f), frontBackColour, vec3(0.0f, 0.0f, 1.0f),

		vec3(0.5f, 0.5f, 0.5f), leftRightColour, vec3(1.0f, 0.0f, 0.0f), // right
		vec3(0.5f,-0.5f,-0.5f), leftRightColour, vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f, 0.5f,-0.5f), leftRightColour, vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f,-0.5f,-0.5f), leftRightColour, vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f, 0.5f, 0.5f), leftRightColour, vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f,-0.5f, 0.5f), leftRightColour, vec3(1.0f, 0.0f, 0.0f),

		vec3(0.5f, 0.5f, 0.5f), topBottomColour, vec3(0.0f, 1.0f, 0.0f), // top
		vec3(0.5f, 0.5f,-0.5f), topBottomColour, vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f,-0.5f), topBottomColour, vec3(0.0f, 1.0f, 0.0f),
		vec3(0.5f, 0.5f, 0.5f), topBottomColour, vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f,-0.5f), topBottomColour, vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), topBottomColour, vec3(0.0f, 1.0f, 0.0f)
	};

	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(unitCube), unitCube, GL_STATIC_DRAW);

	glVertexAttribPointer(0,    // Location 0 matches aPos in Vertex Shader
		3,						// size
		GL_FLOAT,				// type
		GL_FALSE,				// normalized?
		3 * sizeof(vec3),		// stride
		(void*)0				// array buffer offset
	);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1,	// Location 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(vec3),
		(void*)(sizeof(vec3))
	);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2,	// Location 2 matches aNormal in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(vec3),
		(void*)(sizeof(vec3) * 2)
	);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	return vertexArrayObject;
}

int createVertexArrayObjectSingleColoured(vec3 colour)
{
	// Cube model
	// Vertex, colour, normal
	vec3 unitCube[] = {
		vec3(-0.5f,-0.5f,-0.5f), colour, vec3(-1.0f, 0.0f, 0.0f), // left
		vec3(-0.5f,-0.5f, 0.5f), colour, vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), colour, vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f,-0.5f,-0.5f), colour, vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), colour, vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f, 0.5f,-0.5f), colour, vec3(-1.0f, 0.0f, 0.0f),

		vec3(0.5f, 0.5f,-0.5f), colour, vec3(0.0f, 0.0f, -1.0f), // far
		vec3(-0.5f,-0.5f,-0.5f), colour, vec3(0.0f, 0.0f, -1.0f),
		vec3(-0.5f, 0.5f,-0.5f), colour, vec3(0.0f, 0.0f, -1.0f),
		vec3(0.5f, 0.5f,-0.5f), colour, vec3(0.0f, 0.0f, -1.0f),
		vec3(0.5f,-0.5f,-0.5f), colour, vec3(0.0f, 0.0f, -1.0f),
		vec3(-0.5f,-0.5f,-0.5f), colour, vec3(0.0f, 0.0f, -1.0f),

		vec3(0.5f,-0.5f, 0.5f), colour, vec3(0.0f, -1.0f, 0.0f), // bottom
		vec3(-0.5f,-0.5f,-0.5f), colour, vec3(0.0f, -1.0f, 0.0f),
		vec3(0.5f,-0.5f,-0.5f), colour, vec3(0.0f, -1.0f, 0.0f),
		vec3(0.5f,-0.5f, 0.5f), colour, vec3(0.0f, -1.0f, 0.0f),
		vec3(-0.5f,-0.5f, 0.5f), colour, vec3(0.0f, -1.0f, 0.0f),
		vec3(-0.5f,-0.5f,-0.5f), colour, vec3(0.0f, -1.0f, 0.0f),

		vec3(-0.5f, 0.5f, 0.5f), colour, vec3(0.0f, 0.0f, 1.0f), // near
		vec3(-0.5f,-0.5f, 0.5f), colour, vec3(0.0f, 0.0f, 1.0f),
		vec3(0.5f,-0.5f, 0.5f), colour, vec3(0.0f, 0.0f, 1.0f),
		vec3(0.5f, 0.5f, 0.5f), colour, vec3(0.0f, 0.0f, 1.0f),
		vec3(-0.5f, 0.5f, 0.5f), colour, vec3(0.0f, 0.0f, 1.0f),
		vec3(0.5f,-0.5f, 0.5f), colour, vec3(0.0f, 0.0f, 1.0f),

		vec3(0.5f, 0.5f, 0.5f), colour, vec3(1.0f, 0.0f, 0.0f), // right
		vec3(0.5f,-0.5f,-0.5f), colour, vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f, 0.5f,-0.5f), colour, vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f,-0.5f,-0.5f), colour, vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f, 0.5f, 0.5f), colour, vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f,-0.5f, 0.5f), colour, vec3(1.0f, 0.0f, 0.0f),

		vec3(0.5f, 0.5f, 0.5f), colour, vec3(0.0f, 1.0f, 0.0f), // top
		vec3(0.5f, 0.5f,-0.5f), colour, vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f,-0.5f), colour, vec3(0.0f, 1.0f, 0.0f),
		vec3(0.5f, 0.5f, 0.5f), colour, vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f,-0.5f), colour, vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), colour, vec3(0.0f, 1.0f, 0.0f)
	};

	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(unitCube), unitCube, GL_STATIC_DRAW);

	glVertexAttribPointer(0,    // Location 0 matches aPos in Vertex Shader
		3,						// size
		GL_FLOAT,				// type
		GL_FALSE,				// normalized?
		3 * sizeof(vec3),		// stride
		(void*)0				// array buffer offset
	);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1,	// Location 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(vec3),
		(void*)(sizeof(vec3))
	);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2,	// Location 2 matches aNormal in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(vec3),
		(void*)(sizeof(vec3) * 2)
	);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	return vertexArrayObject;
}

int createVertexArrayObjectTextured(vec3 colour)
{
	// Cube model
	// Vertex, colour, normal
	vec3 unitCube[] = {
		vec3(-0.5f,-0.5f,-0.5f), colour, vec3(-1.0f, 0.0f, 0.0f), // left
		vec3(-0.5f,-0.5f, 0.5f), colour, vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), colour, vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f,-0.5f,-0.5f), colour, vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), colour, vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f, 0.5f,-0.5f), colour, vec3(-1.0f, 0.0f, 0.0f),

		vec3(0.5f, 0.5f,-0.5f), colour, vec3(0.0f, 0.0f, -1.0f), // far
		vec3(-0.5f,-0.5f,-0.5f), colour, vec3(0.0f, 0.0f, -1.0f),
		vec3(-0.5f, 0.5f,-0.5f), colour, vec3(0.0f, 0.0f, -1.0f),
		vec3(0.5f, 0.5f,-0.5f), colour, vec3(0.0f, 0.0f, -1.0f),
		vec3(0.5f,-0.5f,-0.5f), colour, vec3(0.0f, 0.0f, -1.0f),
		vec3(-0.5f,-0.5f,-0.5f), colour, vec3(0.0f, 0.0f, -1.0f),

		vec3(0.5f,-0.5f, 0.5f), colour, vec3(0.0f, -1.0f, 0.0f), // bottom
		vec3(-0.5f,-0.5f,-0.5f), colour, vec3(0.0f, -1.0f, 0.0f),
		vec3(0.5f,-0.5f,-0.5f), colour, vec3(0.0f, -1.0f, 0.0f),
		vec3(0.5f,-0.5f, 0.5f), colour, vec3(0.0f, -1.0f, 0.0f),
		vec3(-0.5f,-0.5f, 0.5f), colour, vec3(0.0f, -1.0f, 0.0f),
		vec3(-0.5f,-0.5f,-0.5f), colour, vec3(0.0f, -1.0f, 0.0f),

		vec3(-0.5f, 0.5f, 0.5f), colour, vec3(0.0f, 0.0f, 1.0f), // near
		vec3(-0.5f,-0.5f, 0.5f), colour, vec3(0.0f, 0.0f, 1.0f),
		vec3(0.5f,-0.5f, 0.5f), colour, vec3(0.0f, 0.0f, 1.0f),
		vec3(0.5f, 0.5f, 0.5f), colour, vec3(0.0f, 0.0f, 1.0f),
		vec3(-0.5f, 0.5f, 0.5f), colour, vec3(0.0f, 0.0f, 1.0f),
		vec3(0.5f,-0.5f, 0.5f), colour, vec3(0.0f, 0.0f, 1.0f),

		vec3(0.5f, 0.5f, 0.5f), colour, vec3(1.0f, 0.0f, 0.0f), // right
		vec3(0.5f,-0.5f,-0.5f), colour, vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f, 0.5f,-0.5f), colour, vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f,-0.5f,-0.5f), colour, vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f, 0.5f, 0.5f), colour, vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f,-0.5f, 0.5f), colour, vec3(1.0f, 0.0f, 0.0f),

		vec3(0.5f, 0.5f, 0.5f), colour, vec3(0.0f, 1.0f, 0.0f), // top
		vec3(0.5f, 0.5f,-0.5f), colour, vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f,-0.5f), colour, vec3(0.0f, 1.0f, 0.0f),
		vec3(0.5f, 0.5f, 0.5f), colour, vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f,-0.5f), colour, vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), colour, vec3(0.0f, 1.0f, 0.0f)
	};

	vec2 uvMap[] = {
		vec2(0.0f, 0.0f), // left
		vec2(1.0f, 0.0f),
		vec2(1.0f, 1.0f),
		vec2(0.0f, 0.0f),
		vec2(1.0f, 1.0f),
		vec2(0.0f, 1.0f),

		vec2(0.0f, 1.0f), // far
		vec2(1.0f, 0.0f),
		vec2(1.0f, 1.0f),
		vec2(0.0f, 1.0f),
		vec2(0.0f, 0.0f),
		vec2(1.0f, 0.0f),

		vec2(1.0f, 1.0f), // bottom
		vec2(0.0f, 0.0f),
		vec2(1.0f, 0.0f),
		vec2(1.0f, 1.0f),
		vec2(0.0f, 1.0f),
		vec2(0.0f, 0.0f),

		vec2(0.0f, 1.0f), // near
		vec2(0.0f, 0.0f),
		vec2(1.0f, 0.0f),
		vec2(1.0f, 1.0f),
		vec2(0.0f, 1.0f),
		vec2(1.0f, 0.0f),

		vec2(0.0f, 1.0f), // right
		vec2(1.0f, 0.0f),
		vec2(1.0f, 1.0f),
		vec2(1.0f, 0.0f),
		vec2(0.0f, 1.0f),
		vec2(0.0f, 0.0f),

		vec2(1.0f, 0.0f), // top
		vec2(1.0f, 1.0f),
		vec2(0.0f, 1.0f),
		vec2(1.0f, 0.0f),
		vec2(0.0f, 1.0f),
		vec2(0.0f, 0.0f)
	};

	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(unitCube), unitCube, GL_STATIC_DRAW);

	glVertexAttribPointer(0,    // Location 0 matches aPos in Vertex Shader
		3,						// size
		GL_FLOAT,				// type
		GL_FALSE,				// normalized?
		3 * sizeof(vec3),		// stride
		(void*)0				// array buffer offset
	);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1,	// Location 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(vec3),
		(void*)(sizeof(vec3))
	);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2,	// Location 2 matches aNormal in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(vec3),
		(void*)(sizeof(vec3) * 2)
	);
	glEnableVertexAttribArray(2);

	GLuint uvMapVertexBufferObject;
	glGenBuffers(1, &uvMapVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, uvMapVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvMap), uvMap, GL_STATIC_DRAW);

	glVertexAttribPointer(3,    // attribute 3 matches aUV in Vertex Shader
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vec2),
		(void*)0
	);
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);

	return vertexArrayObject;
}

int loadTexture(string name, char* path) {
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* textureData = stbi_load(path, &width, &height, &nrChannels, 0);
	if (textureData)
	{
		cout << "Texture has been found: " << name << endl;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture: " << name << endl;
	}
	//frees data
	stbi_image_free(textureData);

	return texture;
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
	window = glfwCreateWindow(VIEW_WIDTH, VIEW_HEIGHT, "COMP 371 - Assignment 2 by Spiral Staircase", NULL, NULL);
	if (window == NULL)
	{
		cerr << "Failed to create GLFW window" << endl;
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
		cerr << "Failed to create GLEW" << endl;
		glfwTerminate();
		return false;
	}
	return true;
}

void windowSizeCallback(GLFWwindow* window, int width, int height) {

	ControlState controlState = *(ControlState*)glfwGetWindowUserPointer(window);

	glViewport(0, 0, width, height);

	GLint shaderProgram = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgram);

	mat4 projectionMatrix = perspective(radians(*controlState.fieldOfView),            // field of view in degrees
		(float)width / (float)height,  // aspect ratio
		NEAR_PLANE, FAR_PLANE);   // near and far (near > 0)

	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	ControlState controlState = *(ControlState*)glfwGetWindowUserPointer(window);

	if (key == GLFW_KEY_I && action == GLFW_PRESS) {
		controlState.shapes->at(*(controlState.focusedShape)).mPosition.z -= TRANSLATE_RATE * 0.2;
	}
	if (key == GLFW_KEY_K && action == GLFW_PRESS) {
		controlState.shapes->at(*(controlState.focusedShape)).mPosition.z += TRANSLATE_RATE * 0.2;
	}
	//Texture toggle
	if (key == GLFW_KEY_X && action == GLFW_PRESS) {
		*controlState.showTexture = !*controlState.showTexture;
	}
	if (key == GLFW_KEY_B && action == GLFW_PRESS) {
		*controlState.showShadow = !*controlState.showShadow;
	}
}

void drawScene(ShaderManager shaderManager, GLenum renderingMode, vector<Shape> shapes, vector<Wall> walls, Shape lightbulb, int tileTexture) {

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tileTexture);

	//Draw Tiles
	glBindVertexArray(createVertexArrayObjectTextured(vec3(1.0f, 1.0f, 1.0f)));
	for (int i = -GRID_SIZE / 2 / FLOOR_SCALE; i <= GRID_SIZE / 2 / FLOOR_SCALE; i++) {
		for (int j = -GRID_SIZE / 2 / FLOOR_SCALE; j <= GRID_SIZE / 2 / FLOOR_SCALE; j++) {
			mat4 tileMatrix = translate(mat4(1.0f), vec3(i * FLOOR_SCALE, -0.1f, j * FLOOR_SCALE)) * scale(mat4(1.0f), vec3(FLOOR_SCALE, 0.01f, FLOOR_SCALE));
			shaderManager.setMat4("worldMatrix", tileMatrix);
			glDrawArrays(renderingMode, 0, 36);
		}
	}
	// Draw grid
	glBindVertexArray(createVertexArrayObjectSingleColoured(vec3(1.0f, 1.0f, 0.0f)));

	for (int i = -GRID_SIZE / 2; i <= GRID_SIZE / 2; i++) {
		mat4 gridMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, i)) * scale(mat4(1.0f), vec3(100.0f, 0.02f, 0.02f));
		shaderManager.setMat4("worldMatrix", gridMatrix);
		glDrawArrays(renderingMode, 0, 36);
	}
	for (int i = -GRID_SIZE / 2; i <= GRID_SIZE / 2; i++) {
		mat4 gridMatrix = translate(mat4(1.0f), vec3(i, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.02f, 0.02f, 100.0f));
		shaderManager.setMat4("worldMatrix", gridMatrix);
		glDrawArrays(renderingMode, 0, 36);
	}

	// Draw coordinate axes
	glBindVertexArray(xLineColour);
	mat4 xLine = translate(mat4(1.0f), vec3(2.5f, 0.1f, 0.0f)) * scale(mat4(1.0f), vec3(5.0f, 0.1f, 0.1f));
	shaderManager.setMat4("worldMatrix", xLine);
	glDrawArrays(renderingMode, 0, 36);
	glBindVertexArray(yLineColour);
	mat4 yLine = translate(mat4(1.0f), vec3(0.0f, 2.6f, 0.0f)) * scale(mat4(1.0f), vec3(0.1f, 5.0f, 0.1f));
	shaderManager.setMat4("worldMatrix", yLine);
	glDrawArrays(renderingMode, 0, 36);
	glBindVertexArray(zLineColour);
	mat4 zLine = translate(mat4(1.0f), vec3(0.0f, 0.1f, 2.5f)) * scale(mat4(1.0f), vec3(0.1f, 0.1f, 5.0f));
	shaderManager.setMat4("worldMatrix", zLine);
	glDrawArrays(renderingMode, 0, 36);

	// Draw shapes and walls
	for (Shape shape : shapes) {
		shape.Draw(renderingMode, shaderManager);

		shaderManager.setBool("ignoreLighting", true);
		shape.DrawGlow(renderingMode, shaderManager);
		shaderManager.setBool("ignoreLighting", false);
	}

	for (Wall wall : walls) {
		wall.Draw(renderingMode, shaderManager);
	}

	shaderManager.setBool("ignoreLighting", true);
	lightbulb.Draw(renderingMode, shaderManager);
	shaderManager.setBool("ignoreLighting", false);

	glBindVertexArray(0);
}