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

// Preprocessor macros
// Standard library
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Third-party
#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // cross-platform interface for creating a graphical context,
						// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>
#include <glm/gtx/string_cast.hpp> 
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <time.h>
#include <irrKlang.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Constants.h"
#include "Shape.h"
#include "ShaderManager.h"
#include "ControlState.h"
#include "Wall.h"
#include "Emitter.h"


using namespace glm;
using namespace std;
using namespace irrklang;


// Function declarations
// Event handlers
void windowSizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

int createVertexArrayObjectTextured(vec3 colour);
int createVertexArrayObjectTextured();
int loadTexture(string name, char* path);
void drawScene(ShaderManager shaderManager, GLenum renderingMode, vector<GameObject*>* gameEntities);

bool initContext();

GLFWwindow* window = NULL;

int main(int argc, char* argv[])
{
	if (!initContext()) return -1;

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

	shaderManager.setMat4("projectionMatrix", projectionMatrix);

	// Other OpenGL states to set once before the Game Loop
	// Enable Backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Enable Depth Test
	glEnable(GL_DEPTH_TEST);

	GLenum renderingMode = GL_TRIANGLES;
	glBindTexture(GL_TEXTURE_2D, metalTexture);

	// Persistent game variables
	vector<Event> currentFrameEvents;
	vector<Shape*> shapes;
	vector<Wall*> walls;
	int selectedShape = -1;
	int currentDifficulty = 6;
	float currentWallSpeed = 1.5f;

	// Only use one VAO (set colours with uniform)
	int cubeVAO = createVertexArrayObjectTextured(vec3(1.0f));
	glBindVertexArray(cubeVAO);

	int focusedShape = 0;                   // The shape currently being viewed and manipulated
	bool moveCameraToDestination = false;   // Tracks whether the camera is currently moving to a point
	bool showTexture = true;
	bool showShadow = true;

	// Create event queue
	vector<ScheduledEvent> eventQueue{
		{GAME_START, 0}
	};

	// Set event callbacks
	glfwSetWindowSizeCallback(window, &windowSizeCallback);
	glfwSetKeyCallback(window, &keyCallback);
	glfwSetWindowUserPointer(window, &eventQueue);

	// Create entity pointer container
	vector<GameObject*> gameEntities;

	// Initialize random seed
	srand(time(NULL));

	// Frame time and mouse input
	double lastFrameTime = glfwGetTime();
	int lastMouseLeftState = GLFW_RELEASE;
	double lastMousePosX, lastMousePosY;
	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

	// Camera parameters for view transform
	vec3 cameraPosition = vec3(0.0f, 0.0f, 10.0f);
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


	mat4 shadowProjection = perspective(radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, NEAR_PLANE, FAR_PLANE);

	Emitter emitter = Emitter(cubeVAO);

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

		// Frame time
		double now = glfwGetTime();
		double dt = now - lastFrameTime;
		lastFrameTime = now;


		// Process event queue timers
		for (int i = eventQueue.size() - 1; i >= 0; i--) {
			ScheduledEvent& element = eventQueue.at(i);
			element.timeRemaining -= dt;
			if (element.timeRemaining <= 0) {
				currentFrameEvents.push_back(element.type);
				eventQueue.erase(eventQueue.begin() + i);
			}
		}

		// Delete flagged entities
		for (int i = gameEntities.size() - 1; i >= 0; i--) {
			if (gameEntities.at(i)->destroyFlag) {
				delete gameEntities.at(i);
				gameEntities.erase(gameEntities.begin() + i);
			}
		}

		// Process current frame events
		for (Event event : currentFrameEvents) {
			switch (event) {
			case GAME_START: {
				eventQueue.push_back({ CREATE_SHAPE_AND_WALL, 0 });
				break;
			}

			case CREATE_SHAPE_AND_WALL: {
				vec3 shapeColour = vec3((float)(rand() % 500) / 1000.0f + 0.5f, (float)(rand() % 500) / 1000.0f + 0.5f, (float)(rand() % 500) / 1000.0f + 0.5f);
				Shape* newShape = new Shape(vec3(0), currentDifficulty, shapeColour, metalTexture);
				shapes.push_back(newShape);
				selectedShape = 0;
				Wall* newWall = new Wall(vec3(0, 0, -WALL_DISTANCE), shapes[selectedShape], vec3(1), brickTexture);
				walls.push_back(newWall);
				newWall->speed = currentWallSpeed;
				gameEntities.push_back(newWall);
				break;
			}

			case DISPLAY_SHAPE: {
				Shape* newShape = shapes.at(selectedShape);
				gameEntities.push_back(newShape);
				break;
			}

			case INPUT_UP:
			case INPUT_DOWN:
			case INPUT_LEFT:
			case INPUT_RIGHT: {
				if (selectedShape >= 0) {
					shapes.at(selectedShape)->processEvent(event);
				}
				break;
			}

			case LEVEL_FAILED: {
				cout << "Failure...\n";
				eventQueue.push_back({ DESTROY_SHAPE_AND_WALL, 0 });
				soundEngine->play2D(AUDIO_PATH_BRUH, false);
				break;
			}

			case LEVEL_SUCCESS: {
				cout << "Success!\n";
				eventQueue.push_back({ DESTROY_SHAPE_AND_WALL, 2 });
				currentDifficulty++;
				currentWallSpeed += currentWallSpeed >= 4 ? 0 : 0.1f;
				soundEngine->play2D(AUDIO_PATH_WOW, false);
				break;
			}

			case DESTROY_SHAPE_AND_WALL: {
				for (Shape* shape : shapes) {
					shape->destroyFlag = true;
				}
				selectedShape = -1;
				shapes.clear();
				for (Wall* wall : walls) {
					wall->destroyFlag = true;
				}
				walls.clear();
				eventQueue.push_back({ CREATE_SHAPE_AND_WALL, 2 });
				break;
			}

			case EXIT_PROGRAM: {
				// Exit program
				glfwSetWindowShouldClose(window, true);
			}
			}
		}

		currentFrameEvents.clear();

		// Update entities
		for (GameObject*& entity : gameEntities) {
			entity->update(&eventQueue, dt);
		}


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
		drawScene(shadowShaderManager, renderingMode, &gameEntities);
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
		drawScene(shaderManager, renderingMode, &gameEntities);

		// Update and draw particles
		emitter.Update(dt);
		emitter.Draw(shaderManager);

		shaderManager.use();

		// End Frame
		glfwSwapBuffers(window);
		glfwPollEvents();


		cameraFirstPerson = false;

		double mousePosX, mousePosY;
		glfwGetCursorPos(window, &mousePosX, &mousePosY);

		double dx = mousePosX - lastMousePosX;
		double dy = mousePosY - lastMousePosY;

		lastMousePosX = mousePosX;
		lastMousePosY = mousePosY;

		cameraHorizontalAngle -= dx * CAMERA_ANGULAR_SPEED * dt;
		cameraVerticalAngle -= dy * CAMERA_ANGULAR_SPEED * dt;

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


		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
			emitter.EmitBurst(cameraPosition + cameraLookAt * BURST_DISTANCE_FROM_CAMERA, 100, 5.0f, particleTexture);
			soundEngine->play2D(AUDIO_PATH_WOW);
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


int createVertexArrayObjectTextured()
{
	// Cube model
	// Vertex, normal
	vec3 unitCube[] = {
		vec3(-0.5f,-0.5f,-0.5f), vec3(-1.0f, 0.0f, 0.0f), // left
		vec3(-0.5f,-0.5f, 0.5f), vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f,-0.5f,-0.5f), vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f, 0.5f,-0.5f), vec3(-1.0f, 0.0f, 0.0f),

		vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f), // far
		vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f),
		vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f),
		vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f),
		vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f),
		vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f),

		vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, -1.0f, 0.0f), // bottom
		vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, -1.0f, 0.0f),
		vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, -1.0f, 0.0f),
		vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, -1.0f, 0.0f),
		vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, -1.0f, 0.0f),
		vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, -1.0f, 0.0f),

		vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f), // near
		vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),

		vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), // right
		vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),

		vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), // top
		vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f)
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
		3,                        // size
		GL_FLOAT,                // type
		GL_FALSE,                // normalized?
		2 * sizeof(vec3),        // stride
		(void*)0                // array buffer offset
	);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1,    // Location 1 matches aNormal in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(vec3),
		(void*)(sizeof(vec3))
	);
	glEnableVertexAttribArray(1);

	GLuint uvMapVertexBufferObject;
	glGenBuffers(1, &uvMapVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, uvMapVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvMap), uvMap, GL_STATIC_DRAW);

	glVertexAttribPointer(2,    // attribute 2 matches aUV in Vertex Shader
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vec2),
		(void*)0
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
	window = glfwCreateWindow(VIEW_WIDTH, VIEW_HEIGHT, "COMP 371 - Final project by Spiral Staircase", NULL, NULL);
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

	glViewport(0, 0, width, height);

	GLint shaderProgram = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgram);

	mat4 projectionMatrix = glm::perspective(radians(FIELD_OF_VIEW),            // field of view in degrees
		(float)width / (float)height,  // aspect ratio
		NEAR_PLANE, FAR_PLANE);   // near and far (near > 0)

	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	vector<ScheduledEvent>* eventQueue = (vector<ScheduledEvent>*)glfwGetWindowUserPointer(window);

	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		eventQueue->push_back({ INPUT_UP, 0 });
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		eventQueue->push_back({ INPUT_LEFT, 0 });
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		eventQueue->push_back({ INPUT_DOWN, 0 });
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		eventQueue->push_back({ INPUT_RIGHT, 0 });
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		eventQueue->push_back({ EXIT_PROGRAM, 0 });
	}
}

void drawScene(ShaderManager shaderManager, GLenum renderingMode, vector<GameObject*>* gameEntities) {

	glActiveTexture(GL_TEXTURE0);

	for (GameObject*& entity : *gameEntities) {
		entity->draw(&renderingMode, &shaderManager);
	}

}
