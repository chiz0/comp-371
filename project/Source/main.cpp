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

#include "Model.h"
#include "Constants.h"
#include "Shape.h"
#include "ShaderManager.h"
#include "ControlState.h"
#include "Wall.h"
#include "Emitter.h"
#include "Stage.h"
#include "TerrainComponent.h"


using namespace glm;
using namespace std;
using namespace irrklang;
#include "texture.h"
#include "OBJloader.h"    //For loading .obj files
#include "OBJloaderV2.h"  //For loading .obj files using a polygon list format

using namespace glm;
using namespace std;


// Function declarations
// Event handlers
void windowSizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

int createVertexArrayObjectTextured(vec3 colour);
int createVertexArrayObjectTextured();
int loadTexture(string name, char* path);
GLuint setupModelVBO(string path, int& vertexCount);
//void drawScene(ShaderManager shaderManager, GLenum renderingMode, vector<Shape> shapes, Shape lightbulb, int tileTexture, int cameraPosition, float cameraHorizontalAngle);
void drawScene(ShaderManager shaderManager, GLenum renderingMode, vector<GameObject*>* gameEntities);
void pushMobs(Stage* stage);

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

    // We can set the shader once, since we have only one
    ShaderManager shaderManager = ShaderManager(VERTEX_SHADER_FILEPATH, FRAGMENT_SHADER_FILEPATH);
    ShaderManager shadowShaderManager = ShaderManager(SHADOW_VERTEX_SHADER_FILEPATH, SHADOW_FRAGMENT_SHADER_FILEPATH, SHADOW_DEPTH_SHADER_FILEPATH);


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

    //load the textures
    int tileTexture = loadTexture("tileTexture", TEXTURE_PATH_TILE);
    int shapeTexture = loadTexture("metalTexture", TEXTURE_PATH_BLOCK);
    int brickTexture = loadTexture("brickTexture", TEXTURE_PATH_BRICK);
    int particleTexture = loadTexture("particleTexture", TEXTURE_PATH_PARTICLE);
    int sunTexture = loadTexture("sunTexture", TEXTURE_PATH_SUN);
    int moonTexture = loadTexture("moonTexture", TEXTURE_PATH_MOON);

    int grassTexture = loadTexture("grassTexture", TEXTURE_PATH_GRASS);
    int waterTexture = loadTexture("waterTexture", TEXTURE_PATH_WATER);
    int logTexture = loadTexture("logTexture", TEXTURE_PATH_LOG);
    int leavesTexture = loadTexture("leavesTexture", TEXTURE_PATH_LEAVES);
    int planksTexture = loadTexture("planksTexture", TEXTURE_PATH_PLANKS);
    int rockTexture = loadTexture("rockTexture", TEXTURE_PATH_ROCK);
    int ironTexture = loadTexture("ironTexture", TEXTURE_PATH_IRON);
    int diamondTexture = loadTexture("diamondTexture", TEXTURE_PATH_DIAMOND);
    int obsidianTexture = loadTexture("obsidianTexture", TEXTURE_PATH_OBSIDIAN);
    int netherrackTexture = loadTexture("netherrackTexture", TEXTURE_PATH_NETHERRACK);
    int lavaTexture = loadTexture("lavaTexture", TEXTURE_PATH_LAVA);
    int endStoneTexture = loadTexture("endStoneTexture", TEXTURE_PATH_ENDSTONE);


    // Other camera parameters
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;
    int windowWidth, windowHeight;

    float fieldOfView = FIELD_OF_VIEW;
    mat4 projectionMatrix = perspective(radians(fieldOfView),   // field of view in degrees
        VIEW_WIDTH / VIEW_HEIGHT,                                // aspect ratio
        NEAR_PLANE, FAR_PLANE);                                    // near and far (near > 0)

    shaderManager.setMat4("projectionMatrix", projectionMatrix);

    // Other OpenGL states to set once before the Game Loop
    // Enable Backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Enable Depth Test
    glEnable(GL_DEPTH_TEST);

    GLenum renderingMode = GL_TRIANGLES;
    glBindTexture(GL_TEXTURE_2D, shapeTexture);

    vec3 whiteColour = vec3(1.0f, 1.0f, 0.0f);

    ///////// DESIGN MODELS HERE /////////

    // Only use one VAO (set colours with uniform)
    int cubeVAO = createVertexArrayObjectTextured(vec3(1.0f));
    glBindVertexArray(cubeVAO);


    //Light
    vector<ivec3> lightbulbShape{
        {0, 0, 0}
    };

    Shape lightbulb = Shape(vec3(0.0f, 0.0f, 0.0f), lightbulbShape, whiteColour, shapeTexture);


    // Create stage
    Stage* stage = new Stage(STAGE_STARTING_LOCATION, cubeVAO);
    stage->_scale = vec3(STAGE_INITIAL_SCALE);

    // Landscape
    for (int chunk = 0; chunk < 30; chunk++) {
        if (chunk >= 20) {
            stage->attachTerrain(TerrainComponent(DESCRIPTION_GRASS_HILL, whiteColour, endStoneTexture, chunk), vec3(0, 0, chunk * 20));
        }
        else if (chunk >= 10) {
            stage->attachTerrain(TerrainComponent(DESCRIPTION_GRASS_HILL, whiteColour, netherrackTexture, chunk), vec3(0, 0, chunk * 20));
        }
        else {
            stage->attachTerrain(TerrainComponent(DESCRIPTION_GRASS_HILL, whiteColour, grassTexture, chunk), vec3(0, 0, chunk * 20));
        }
    }

    // Terrain features
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_TRUNK, whiteColour, logTexture, 0), vec3(13, 4, 0));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_TRUNK, whiteColour, logTexture, 0), vec3(-16, 4, 0));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_TRUNK, whiteColour, logTexture, 0), vec3(-7, 2, 1));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_TRUNK, whiteColour, logTexture, 0), vec3(19, 5, 4));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_TRUNK, whiteColour, logTexture, 0), vec3(-19, 5, 6));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_TRUNK, whiteColour, logTexture, 0), vec3(10, 2, 8));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_TRUNK, whiteColour, logTexture, 0), vec3(19, 5, 12));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_TRUNK, whiteColour, logTexture, 0), vec3(-9, 2, 12));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_TRUNK, whiteColour, logTexture, 0), vec3(8, 2, 17));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_TRUNK, whiteColour, logTexture, 0), vec3(-18, 4, 18));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_TRUNK, whiteColour, logTexture, 0), vec3(18, 5, 19));

    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_LEAVES, whiteColour, leavesTexture, 0), vec3(13, 9, 0));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_LEAVES, whiteColour, leavesTexture, 0), vec3(-16, 9, 0));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_LEAVES, whiteColour, leavesTexture, 0), vec3(-7, 7, 1));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_LEAVES, whiteColour, leavesTexture, 0), vec3(19, 10, 4));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_LEAVES, whiteColour, leavesTexture, 0), vec3(-19, 10, 6));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_LEAVES, whiteColour, leavesTexture, 0), vec3(10, 7, 8));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_LEAVES, whiteColour, leavesTexture, 0), vec3(19, 10, 12));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_LEAVES, whiteColour, leavesTexture, 0), vec3(-9, 7, 12));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_LEAVES, whiteColour, leavesTexture, 0), vec3(8, 7, 17));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_LEAVES, whiteColour, leavesTexture, 0), vec3(-18, 9, 18));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_LEAVES, whiteColour, leavesTexture, 0), vec3(18, 10, 19));

    //Cut down planks + trees for the 2nd chunk
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_TRUNK, whiteColour, logTexture, 1), vec3(-18, 5, 39));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_LEAVES, whiteColour, leavesTexture, 1), vec3(-18, 10, 39));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_TRUNK, whiteColour, logTexture, 1), vec3(19, 5, 26));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_LEAVES, whiteColour, leavesTexture, 1), vec3(19, 10, 26));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_TRUNK, whiteColour, logTexture, 1), vec3(14, 5, 36));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_LEAVES, whiteColour, leavesTexture, 1), vec3(14, 10, 36));

    stage->attachTerrain(TerrainComponent(DESCRIPTION_CUT_PLANKS, whiteColour, planksTexture, 1), vec3(0, 1, 36));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_CUT_PLANKS, whiteColour, planksTexture, 1), vec3(10, 3, 29));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_CUT_PLANKS, whiteColour, planksTexture, 1), vec3(-13, 4, 32));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_CUT_PLANKS, whiteColour, planksTexture, 1), vec3(-8, 2, 21));

    //Rock formation + trees
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_TRUNK, whiteColour, logTexture, 2), vec3(-18, 5, 59));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_LEAVES, whiteColour, leavesTexture, 2), vec3(-18, 10, 59));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_TRUNK, whiteColour, logTexture, 2), vec3(19, 5, 46));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_LEAVES, whiteColour, leavesTexture, 2), vec3(19, 10, 46));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_TRUNK, whiteColour, logTexture, 2), vec3(14, 5, 56));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_TREE_LEAVES, whiteColour, leavesTexture, 2), vec3(14, 10, 56));

    stage->attachTerrain(TerrainComponent(DESCRIPTION_ROCK_FORMATION, whiteColour, rockTexture, 2), vec3(0, 1, 56));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_ROCK_FORMATION, whiteColour, rockTexture, 2), vec3(10, 3, 49));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_ROCK_FORMATION, whiteColour, rockTexture, 2), vec3(-13, 4, 52));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_ROCK_FORMATION, whiteColour, rockTexture, 2), vec3(-8, 2, 41));

    // Cave and ores chunk 8
    stage->attachTerrain(TerrainComponent(DESCRIPTION_CAVE, whiteColour, rockTexture, 7), vec3(11.9f, 3.1, 152.1));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_ROCK_FORMATION, whiteColour, ironTexture, 7), vec3(19, 5, 156));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_ROCK_FORMATION, whiteColour, ironTexture, 7), vec3(8, 2, 149));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_ROCK_FORMATION, whiteColour, ironTexture, 7), vec3(-13, 4, 152));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_ROCK_FORMATION, whiteColour, ironTexture, 7), vec3(-8, 2, 141));

    //chunk 9;
    stage->attachTerrain(TerrainComponent(DESCRIPTION_ROCK_FORMATION, whiteColour, diamondTexture, 8), vec3(-19, 5, 176));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_ROCK_FORMATION, whiteColour, diamondTexture, 8), vec3(-8, 2, 169));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_ROCK_FORMATION, whiteColour, diamondTexture, 8), vec3(13, 4, 172));
    stage->attachTerrain(TerrainComponent(DESCRIPTION_ROCK_FORMATION, whiteColour, diamondTexture, 8), vec3(8, 2, 161));

    //chunk 10
    stage->attachTerrain(TerrainComponent(DESCRIPTION_PORTAL, whiteColour, obsidianTexture, 9), vec3(-4, 5, 199));

    // Chunk 20
    stage->attachTerrain(TerrainComponent(DESCRIPTION_PORTAL, whiteColour, obsidianTexture, 19), vec3(-4, 5, 399));

    // Add mobs

    pushMobs(stage);

    // Add sun
    Sun* sun = new Sun(sunTexture, moonTexture);
    stage->attachSun(sun);

    // Persistent game variables
    vector<Event> currentFrameEvents;
    vector<Shape*> shapes;
    vector<Wall*> walls;
    int selectedShape = -1;
    int currentDifficulty = STARTING_DIFFICULTY;
    float currentWallSpeed = INITIAL_WALL_SPEED;
    bool pauseShapeCreation = false;


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
    gameEntities.push_back(stage);

    // Initialize random seed
    srand(time(NULL));

    // Frame time and mouse input
    double lastFrameTime = glfwGetTime();
    int lastMouseLeftState = GLFW_RELEASE;
    double lastMousePosX, lastMousePosY;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

    // Camera parameters for view transform
    vec3 cameraPosition = CAMERA_OFFSET;
    vec3 cameraLookAt(0.0f, -1.0f, 0.0f);
    vec3 cameraUp(0.0f, 1.0f, 0.0f);
    shaderManager.setVec3("cameraPosition", cameraPosition);

    // Set initial view matrix
    mat4 viewMatrix = lookAt(cameraPosition,  // eye
        cameraPosition + cameraLookAt,  // center
        cameraUp); // up

    shaderManager.setMat4("viewMatrix", viewMatrix);

    // Set up lighting
    shaderManager.setVec3("lightColour", 1.0f, 1.0f, 1.0f);
    shaderManager.setFloat("ambientLight", LIGHT_AMBIENT_STRENGTH);
    shaderManager.setFloat("diffuseLight", LIGHT_DIFFUSE_STRENGTH);
    shaderManager.setFloat("specularLight", LIGHT_SPECULAR_STRENGTH);
    shaderManager.setFloat("shininess", SHININESS);
    shaderManager.setInt("depthMap", 1);


    mat4 shadowProjection = perspective(radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, NEAR_PLANE, FAR_PLANE);

    Emitter emitter = Emitter(cubeVAO, particleTexture, particleTexture);

    // Sound settings
    ISoundEngine* soundEngine = createIrrKlangDevice();

    if (!soundEngine)
    {
        cout << ("WARNING: Could not start sound engine") << endl;
    }

    ISound* bgMusic = soundEngine->play2D(AUDIO_PATH_OVERWORLD, true, false, true);

    int world = 0;

    // Entering Game Loop
    while (!glfwWindowShouldClose(window))
    {
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
                stage->speed = INITIAL_STAGE_SPEED;
                stage->currentWorld = 0;
                eventQueue.push_back({ CREATE_SHAPE_AND_WALL, 0 });
                break;
            }

            case CREATE_SHAPE_AND_WALL: {
                if (!pauseShapeCreation) {
                    vec3 shapeColour = vec3((float)(rand() % 500) / 1000.0f + 0.5f, (float)(rand() % 500) / 1000.0f + 0.5f, (float)(rand() % 500) / 1000.0f + 0.5f);
                    Shape* newShape = new Shape(vec3(0), currentDifficulty, shapeColour, shapeTexture);
                    shapes.push_back(newShape);
                    selectedShape = 0;
                    Wall* newWall = new Wall(vec3(0, 0, -WALL_DISTANCE), shapes[selectedShape], vec3(1), brickTexture);
                    walls.push_back(newWall);
                    newWall->speed = currentWallSpeed;
                    newWall->particleEmitter = &emitter;
                    gameEntities.push_back(newWall);
                }
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
                eventQueue.push_back({ DESTROY_SHAPE_AND_WALL, 0 });
                soundEngine->play2D(AUDIO_PATH_IMPACT, false);
                break;
            }

            case LEVEL_SUCCESS: {
                eventQueue.push_back({ DESTROY_SHAPE_AND_WALL, 3 });
                currentDifficulty += stage->currentWorld + 1;
                currentWallSpeed += currentWallSpeed >= DIFFICULTY_SPEED_MAX ? 0 : DIFFICULTY_SPEED_GROWTH;
                soundEngine->play2D(AUDIO_PATH_LAUNCH, false);
                soundEngine->play2D(AUDIO_PATH_BLAST, false);
                break;
            }

            case DESTROY_SHAPE_AND_WALL: {
                if (selectedShape >= 0) {
                    shapes[selectedShape]->processEvent(event);
                    selectedShape = -1;
                    shapes.clear();
                    walls.clear();
                }
                if (!pauseShapeCreation) {
                    eventQueue.push_back({ CREATE_SHAPE_AND_WALL, 2 });
                }
                break;
            }

            case DESTROY_SHAPE_AND_WALL_WORLD_TRANSITION: {
                if (selectedShape >= 0) {
                    shapes[selectedShape]->processEvent(event);
                    walls[selectedShape]->processEvent(event);
                    selectedShape = -1;
                    shapes.clear();
                    walls.clear();
                }
                break;
            }

            case WORLD_TRANSITION: {
                bgMusic->stop();
                if (bgMusic->isFinished()) {
                    bgMusic->drop();
                }
                // Move to next world
                soundEngine->play2D(AUDIO_PATH_CHIMES);
                world++;
                if (world >= WORLDS) {
                    // End the game, VICTORY!
                    eventQueue.push_back({ EXIT_PROGRAM, 2 });
                }
                else {
                    switch (world) {
                    case 0:
                        // Overworld
                        bgMusic = soundEngine->play2D(AUDIO_PATH_OVERWORLD, true, false, true);
                        break;
                    case 1:
                        // Nether
                        bgMusic = soundEngine->play2D(AUDIO_PATH_NETHER, true, false, true);
                        break;
                    case 2:
                        // Nether
                        bgMusic = soundEngine->play2D(AUDIO_PATH_END, true, false, true);
                        break;
                    default:
                        cout << "INVALID WORLD ID " + world;
                    }

                    glClearColor(worldSkyColours[world].x, worldSkyColours[world].y, worldSkyColours[world].z, worldSkyColours[world].a);
                    pauseShapeCreation = false;
                    eventQueue.push_back({ CREATE_SHAPE_AND_WALL, 0 });
                }
                break;
            }

            case PORTAL_ON_HORIZON: {
                pauseShapeCreation = true;
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
        emitter.Update(dt);


        // Clear Depth Buffer Bit
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 0. create depth cubemap transformation matrices
        // -----------------------------------------------
        mat4 shadowTransforms[6] = {
            shadowProjection * lookAt(sun->getLightPosition(), sun->getLightPosition() + vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)),
            shadowProjection * lookAt(sun->getLightPosition(), sun->getLightPosition() + vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)),
            shadowProjection * lookAt(sun->getLightPosition(), sun->getLightPosition() + vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),
            shadowProjection * lookAt(sun->getLightPosition(), sun->getLightPosition() + vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)),
            shadowProjection * lookAt(sun->getLightPosition(), sun->getLightPosition() + vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f)),
            shadowProjection * lookAt(sun->getLightPosition(), sun->getLightPosition() + vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f))
        };

        // 1. render scene to depth cubemap
        // --------------------------------
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        shadowShaderManager.use();

        for (unsigned int i = 0; i < 6; ++i)
            shadowShaderManager.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
        shadowShaderManager.setFloat("farPlane", FAR_PLANE);

        // TODO: Merge these DrawScenes
        if (cameraPosition.z >= 399.4) {
            //drawScene(shadowShaderManager, renderingMode, shapes, lightbulb, waterTexture, cameraPosition.z, cameraHorizontalAngle);
        }
        else if (cameraPosition.z >= 199.4) {
            //drawScene(shadowShaderManager, renderingMode, shapes, lightbulb, lavaTexture, cameraPosition.z, cameraHorizontalAngle);
        }
        else {
            //drawScene(shadowShaderManager, renderingMode, shapes, lightbulb, waterTexture, cameraPosition.z, cameraHorizontalAngle);
        }

        drawScene(shadowShaderManager, renderingMode, &gameEntities);

        // Update and draw particles
        emitter.Draw(shadowShaderManager);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 2. render scene as normal 
        // -------------------------
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderManager.use();


        // set lighting uniforms
        shaderManager.setFloat("farPlane", FAR_PLANE);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
        // TODO: Merge these DrawScenes
        if (cameraPosition.z >= 399.4) {
            //drawScene(shaderManager, renderingMode, shapes, lightbulb, waterTexture, cameraPosition.z, cameraHorizontalAngle);
        }
        else if (cameraPosition.z >= 199.4) {
            //drawScene(shaderManager, renderingMode, shapes, lightbulb, lavaTexture, cameraPosition.z, cameraHorizontalAngle);
        }
        else {
            //drawScene(shaderManager, renderingMode, shapes, lightbulb, waterTexture, cameraPosition.z, cameraHorizontalAngle);
        }
        drawScene(shaderManager, renderingMode, &gameEntities);

        // Update and draw particles
        emitter.Draw(shaderManager);

        // End Frame
        glfwSwapBuffers(window);
        glfwPollEvents();

        double mousePosX, mousePosY;
        glfwGetCursorPos(window, &mousePosX, &mousePosY);

        double dx = mousePosX - lastMousePosX;
        double dy = mousePosY - lastMousePosY;

        lastMousePosX = mousePosX;
        lastMousePosY = mousePosY;

        cameraHorizontalAngle -= dx * CAMERA_ANGULAR_SPEED * dt;
        cameraVerticalAngle -= dy * CAMERA_ANGULAR_SPEED * dt;

        // Clamp camera angles
        cameraHorizontalAngle = std::max(60.0f, std::min(cameraHorizontalAngle, 120.0f));
        cameraVerticalAngle = std::max(-30.0f, std::min(cameraVerticalAngle, 15.0f));

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

        float radius = sqrt(pow(cameraPosition.x, 2) + pow(cameraPosition.y, 2) + pow(cameraPosition.z, 2));
        vec3 position = vec3(0.0f, 1.0f, 0.0f) - radius * cameraLookAt;
        viewMatrix = lookAt(position, position + cameraLookAt, cameraUp);
        shaderManager.setVec3("cameraPosition", cameraPosition);
        shaderManager.setMat4("viewMatrix", viewMatrix);

    }

    // Shutdown GLFW
    glfwTerminate();

    soundEngine->drop();
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
        3,                        // size
        GL_FLOAT,                // type
        GL_FALSE,                // normalized?
        3 * sizeof(vec3),        // stride
        (void*)0                // array buffer offset
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,    // Location 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(vec3),
        (void*)(sizeof(vec3))
    );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,    // Location 2 matches aNormal in Vertex Shader
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

    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        eventQueue->push_back({ INPUT_UP, 0 });
    }
    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        eventQueue->push_back({ INPUT_LEFT, 0 });
    }
    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        eventQueue->push_back({ INPUT_DOWN, 0 });
    }
    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        eventQueue->push_back({ INPUT_RIGHT, 0 });
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        eventQueue->push_back({ EXIT_PROGRAM, 0 });
    }
}

GLuint setupModelVBO(string path, int& vertexCount) {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> UVs;

    // read the vertex data from the model's OBJ file
    loadOBJ(path.c_str(), vertices, normals, UVs);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);  // Becomes active VAO
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and
    // attribute pointer(s).

    // Vertex VBO setup
    GLuint vertices_VBO;
    glGenBuffers(1, &vertices_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
        &vertices.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
        (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Normals VBO setup
    GLuint normals_VBO;
    glGenBuffers(1, &normals_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3),
        &normals.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
        (GLvoid*)0);
    glEnableVertexAttribArray(1);

    // UVs VBO setup
    GLuint uvs_VBO;
    glGenBuffers(1, &uvs_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, uvs_VBO);
    glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs.front(),
        GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
        (GLvoid*)0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent
    // strange bugs, as we are using multiple VAOs)
    vertexCount = vertices.size();
    return VAO;
}

/*
void drawScene(ShaderManager shaderManager, GLenum renderingMode, vector<Shape> shapes, Shape lightbulb, int tileTexture, int cameraPosition, float cameraHorizontalAngle) {

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tileTexture);
    //Draw Tiles
    glBindVertexArray(createVertexArrayObjectTextured(vec3(1.0f, 1.0f, 1.0f)));
    for (int i = -20 / 2 / FLOOR_SCALE; i <= 20 / 2 / FLOOR_SCALE; i++) {
        for (int j = 0 / 2 / FLOOR_SCALE; j <= GRID_SIZE / 2 / FLOOR_SCALE; j++) {
            mat4 tileMatrix = translate(mat4(1.0f), vec3(i * FLOOR_SCALE, 1.0f, j * FLOOR_SCALE + (int)(cameraPosition / 20 - 2) * 20)) * scale(mat4(1.0f), vec3(FLOOR_SCALE, 0.1f, FLOOR_SCALE));
            shaderManager.setMat4("worldMatrix", tileMatrix);
            glDrawArrays(renderingMode, 0, 36);
        }
    }

    //Water Wall
    glBindVertexArray(createVertexArrayObjectTextured(vec3(1.0f, 1.0f, 1.0f)));
    for (int i = -GRID_SIZE / 2 / FLOOR_SCALE; i <= GRID_SIZE / 2 / FLOOR_SCALE; i++) {
        for (int j = -GRID_SIZE / 2 / FLOOR_SCALE; j <= GRID_SIZE / 2 / FLOOR_SCALE; j++) {
            mat4 tileMatrix = translate(mat4(1.0f), vec3(i * FLOOR_SCALE, j * FLOOR_SCALE, 199.4f)) * scale(mat4(1.0f), vec3(FLOOR_SCALE, FLOOR_SCALE, 0.1f));
            shaderManager.setMat4("worldMatrix", tileMatrix);
            glDrawArrays(renderingMode, 0, 36);
        }
    }

    //Lava Wall
    glBindVertexArray(createVertexArrayObjectTextured(vec3(1.0f, 1.0f, 1.0f)));
    for (int i = -GRID_SIZE / 2 / FLOOR_SCALE; i <= GRID_SIZE / 2 / FLOOR_SCALE; i++) {
        for (int j = -GRID_SIZE / 2 / FLOOR_SCALE; j <= GRID_SIZE / 2 / FLOOR_SCALE; j++) {
            mat4 tileMatrix = translate(mat4(1.0f), vec3(i * FLOOR_SCALE, j * FLOOR_SCALE, 399.4f)) * scale(mat4(1.0f), vec3(FLOOR_SCALE, FLOOR_SCALE, 0.1f));
            shaderManager.setMat4("worldMatrix", tileMatrix);
            glDrawArrays(renderingMode, 0, 36);
        }
    }

    // Draw shapes
    for (Shape shape : shapes) {
        shape.Draw(renderingMode, shaderManager);
    }

    if (((int)(cameraHorizontalAngle / 180.0f) % 2) == 1 || ((int)(cameraHorizontalAngle / 180.0f) % 2) == -1) {
        for (int i = (int)(cameraPosition / 20); i < (int)(cameraPosition / 20) + 3; i++) {
            if (i< 30 && i>-1) {
                for (Shape shape : owChunks[i]) {
                    shape.Draw(renderingMode, shaderManager);
                }

                for (Model shape : mobs[i]) {
                    shape.Draw(shaderManager);
                }
            }
        }
    }
    else
    {
        for (int i = (int)(cameraPosition / 20); i > (int)(cameraPosition / 20) - 3; i--) {
            if (i >= 0 && i < 30) {
                for (Shape shape : owChunks[i]) {
                    shape.Draw(renderingMode, shaderManager);
                }
                for (Model shape : mobs[i]) {
                    shape.Draw(shaderManager);
                }
            }
        }
    }

    shaderManager.setBool("ignoreLighting", true);
    lightbulb.Draw(renderingMode, shaderManager);
    shaderManager.setBool("ignoreLighting", false);

    glBindVertexArray(0);
}
*/

void drawScene(ShaderManager shaderManager, GLenum renderingMode, vector<GameObject*>* gameEntities) {
    glActiveTexture(GL_TEXTURE0);

    for (GameObject*& entity : *gameEntities) {
        entity->draw(&renderingMode, &shaderManager);
    }
}

void pushMobs(Stage* stage) {

    //4TH chunk mobs
    stage->attachModel(Model(MODEL_PATH_COW,
        glm::translate(mat4(1.0f), vec3(10.0f, 2.5f, 60.0f)) *                    //Position
        glm::rotate(mat4(1.0f), radians(-120.0f), vec3(0.0f, 1.0f, 0.0f)) *       //Orientation
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
        glm::scale(mat4(1.0f), vec3(0.5f)), 3));								  //Scale
    stage->attachModel(Model(MODEL_PATH_COW,
        glm::translate(mat4(1.0f), vec3(-10.0f, 2.5f, 68.0f)) *                   //Position
        glm::rotate(mat4(1.0f), radians(120.0f), vec3(0.0f, 1.0f, 0.0f)) *        //Orientation
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
        glm::scale(mat4(1.0f), vec3(0.5f)), 3));								  //Scale
    stage->attachModel(Model(MODEL_PATH_COW,
        glm::translate(mat4(1.0f), vec3(-13.0f, 3.5f, 70.0f)) *                   //Position
        glm::rotate(mat4(1.0f), radians(30.0f), vec3(0.0f, 1.0f, 0.0f)) *         //Orientation
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
        glm::scale(mat4(1.0f), vec3(0.5f)), 3));
    stage->attachModel(Model(MODEL_PATH_COW,
        glm::translate(mat4(1.0f), vec3(18.0f, 4.5f, 76.0f)) *                    //Position
        glm::rotate(mat4(1.0f), radians(-45.0f), vec3(0.0f, 1.0f, 0.0f)) *        //Orientation
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
        glm::scale(mat4(1.0f), vec3(0.5f)), 3));

    stage->attachModel(Model(MODEL_PATH_CHICKEN,
        glm::translate(mat4(1.0f), vec3(8.0f, 2.5f, 63.0f)) *                     //Position
        glm::rotate(mat4(1.0f), radians(-90.0f), vec3(0.0f, 1.0f, 0.0f)) *        //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(0.5f)), 3));								  //Scale
    stage->attachModel(Model(MODEL_PATH_CHICKEN,
        glm::translate(mat4(1.0f), vec3(7.0f, 2.5f, 65.0f)) *                     //Position
        glm::rotate(mat4(1.0f), radians(-130.0f), vec3(0.0f, 1.0f, 0.0f)) *       //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(0.5f)), 3));								  //Scale
    stage->attachModel(Model(MODEL_PATH_CHICKEN,
        glm::translate(mat4(1.0f), vec3(-15.0f, 4.5f, 64.0f)) *                   //Position
        glm::rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f)) *         //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(0.5f)), 3));								  //Scale

    stage->attachModel(Model(MODEL_PATH_SHEEP,
        glm::translate(mat4(1.0f), vec3(12.0f, 3.5f, 63.0f)) *                    //Position
        glm::rotate(mat4(1.0f), radians(145.0f), vec3(0.0f, 1.0f, 0.0f)) *        //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(0.5f)), 3));								  //Scale
    stage->attachModel(Model(MODEL_PATH_SHEEP,
        glm::translate(mat4(1.0f), vec3(7.0f, 1.5f, 69.0f)) *                     //Position
        glm::rotate(mat4(1.0f), radians(145.0f), vec3(0.0f, 1.0f, 0.0f)) *        //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(0.5f)), 3));								  //Scale
    stage->attachModel(Model(MODEL_PATH_SHEEP,
        glm::translate(mat4(1.0f), vec3(-2.0f, 0.5f, 76.0f)) *                    //Position
        glm::rotate(mat4(1.0f), radians(145.0f), vec3(0.0f, 1.0f, 0.0f)) *        //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(0.5f)), 3));




    //5TH chunk mobs
    stage->attachModel(Model(MODEL_PATH_COW,
        glm::translate(mat4(1.0f), vec3(10.0f, 2.5f, 80.0f)) *                    //Position
        glm::rotate(mat4(1.0f), radians(-120.0f), vec3(0.0f, 1.0f, 0.0f)) *       //Orientation
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
        glm::scale(mat4(1.0f), vec3(0.5f)), 4));								  //Scale
    stage->attachModel(Model(MODEL_PATH_COW,
        glm::translate(mat4(1.0f), vec3(10.0f, 2.5f, 88.0f)) *                    //Position
        glm::rotate(mat4(1.0f), radians(-90.0f), vec3(0.0f, 1.0f, 0.0f)) *        //Orientation
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
        glm::scale(mat4(1.0f), vec3(0.5f)), 4));								  //Scale
    stage->attachModel(Model(MODEL_PATH_COW,
        glm::translate(mat4(1.0f), vec3(13.0f, 3.5f, 90.0f)) *                    //Position
        glm::rotate(mat4(1.0f), radians(-30.0f), vec3(0.0f, 1.0f, 0.0f)) *        //Orientation
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation
        glm::scale(mat4(1.0f), vec3(0.5f)), 4));

    stage->attachModel(Model(MODEL_PATH_CHICKEN,
        glm::translate(mat4(1.0f), vec3(-7.0f, 2.5f, 83.0f)) *                    //Position
        glm::rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f)) *         //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(0.5f)), 4));								  //Scale
    stage->attachModel(Model(MODEL_PATH_CHICKEN,
        glm::translate(mat4(1.0f), vec3(-6.0f, 2.5f, 85.0f)) *                    //Position
        glm::rotate(mat4(1.0f), radians(130.0f), vec3(0.0f, 1.0f, 0.0f)) *        //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(0.5f)), 4));								  //Scale
    stage->attachModel(Model(MODEL_PATH_CHICKEN,
        glm::translate(mat4(1.0f), vec3(15.0f, 4.5f, 84.0f)) *                    //Position
        glm::rotate(mat4(1.0f), radians(-90.0f), vec3(0.0f, 1.0f, 0.0f)) *        //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(0.5f)), 4));								  //Scale

    stage->attachModel(Model(MODEL_PATH_SHEEP,
        glm::translate(mat4(1.0f), vec3(-9.0f, 2.5f, 82.0f)) *                    //Position
        glm::rotate(mat4(1.0f), radians(145.0f), vec3(0.0f, 1.0f, 0.0f)) *        //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(0.5f)), 4));

    stage->attachModel(Model(MODEL_PATH_PIG,
        glm::translate(mat4(1.0f), vec3(-12.0f, 3.5f, 86.0f)) *                   //Position
        glm::rotate(mat4(1.0f), radians(120.0f), vec3(0.0f, 1.0f, 0.0f)) *        //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(0.25f)), 4));
    stage->attachModel(Model(MODEL_PATH_PIG,
        glm::translate(mat4(1.0f), vec3(-15.0f, 4.25f, 89.0f)) *                  //Position
        glm::rotate(mat4(1.0f), radians(80.0f), vec3(0.0f, 1.0f, 0.0f)) *         //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(0.25f)), 4));



    //6TH chunk mobs
    stage->attachModel(Model(MODEL_PATH_SKELETON,
        glm::translate(mat4(1.0f), vec3(18.0f, 4.5f, 100.0f)) *                   //Position
        glm::rotate(mat4(1.0f), radians(-80.0f), vec3(0.0f, 1.0f, 0.0f)) *        //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(1.0f)), 5));
    stage->attachModel(Model(MODEL_PATH_SKELETON,
        glm::translate(mat4(1.0f), vec3(-11.0f, 3.5f, 105.0f)) *                  //Position
        glm::rotate(mat4(1.0f), radians(80.0f), vec3(0.0f, 1.0f, 0.0f)) *         //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(1.0f)), 5));
    stage->attachModel(Model(MODEL_PATH_SKELETON,
        glm::translate(mat4(1.0f), vec3(11.0f, 3.5f, 110.0f)) *                   //Position
        glm::rotate(mat4(1.0f), radians(-80.0f), vec3(0.0f, 1.0f, 0.0f)) *        //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(1.0f)), 5));
    stage->attachModel(Model(MODEL_PATH_SKELETON,
        glm::translate(mat4(1.0f), vec3(-18.0f, 4.5f, 115.0f)) *                  //Position
        glm::rotate(mat4(1.0f), radians(-80.0f), vec3(0.0f, 1.0f, 0.0f)) *        //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(1.0f)), 5));

    stage->attachModel(Model(MODEL_PATH_ZOMBIE,
        glm::translate(mat4(1.0f), vec3(14.0f, 4.5f, 102.0f)) *                   //Position
        glm::rotate(mat4(1.0f), radians(-80.0f), vec3(0.0f, 1.0f, 0.0f)) *        //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(1.0f)), 5));
    stage->attachModel(Model(MODEL_PATH_ZOMBIE,
        glm::translate(mat4(1.0f), vec3(12.0f, 3.5f, 105.0f)) *                   //Position
        glm::rotate(mat4(1.0f), radians(-110.0f), vec3(0.0f, 1.0f, 0.0f)) *       //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(1.0f)), 5));
    stage->attachModel(Model(MODEL_PATH_ZOMBIE,
        glm::translate(mat4(1.0f), vec3(-11.0f, 3.5f, 110.0f)) *                  //Position
        glm::rotate(mat4(1.0f), radians(145.0f), vec3(0.0f, 1.0f, 0.0f)) *        //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(1.0f)), 5));

    stage->attachModel(Model(MODEL_PATH_SPIDER,
        glm::translate(mat4(1.0f), vec3(17.0f, 4.5f, 115.0f)) *                   //Position
        glm::rotate(mat4(1.0f), radians(145.0f), vec3(0.0f, 1.0f, 0.0f)) *        //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(1.0f)), 5));
    stage->attachModel(Model(MODEL_PATH_SPIDER,
        glm::translate(mat4(1.0f), vec3(-16.0f, 3.5f, 107.0f)) *                  //Position
        glm::rotate(mat4(1.0f), radians(60.0f), vec3(0.0f, 1.0f, 0.0f)) *         //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(1.0f)), 5));
    stage->attachModel(Model(MODEL_PATH_SPIDER,
        glm::translate(mat4(1.0f), vec3(-9.0f, 1.5f, 117.0f)) *                   //Position
        glm::rotate(mat4(1.0f), radians(20.0f), vec3(0.0f, 1.0f, 0.0f)) *         //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(1.0f)), 5));



    //7TH chunk mobs
    stage->attachModel(Model(MODEL_PATH_SKELETON,
        glm::translate(mat4(1.0f), vec3(18.0f, 4.5f, 120.0f)) *                   //Position
        glm::rotate(mat4(1.0f), radians(-120.0f), vec3(0.0f, 1.0f, 0.0f)) *       //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(1.0f)), 6));
    stage->attachModel(Model(MODEL_PATH_SKELETON,
        glm::translate(mat4(1.0f), vec3(-11.0f, 3.5f, 125.0f)) *                  //Position
        glm::rotate(mat4(1.0f), radians(80.0f), vec3(0.0f, 1.0f, 0.0f)) *         //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(1.0f)), 6));
    stage->attachModel(Model(MODEL_PATH_SKELETON,
        glm::translate(mat4(1.0f), vec3(11.0f, 3.5f, 130.0f)) *                   //Position
        glm::rotate(mat4(1.0f), radians(-120.0f), vec3(0.0f, 1.0f, 0.0f)) *       //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(1.0f)), 6));
    stage->attachModel(Model(MODEL_PATH_SKELETON,
        glm::translate(mat4(1.0f), vec3(-18.0f, 4.5f, 135.0f)) *                  //Position
        glm::rotate(mat4(1.0f), radians(80.0f), vec3(0.0f, 1.0f, 0.0f)) *         //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(1.0f)), 6));

    stage->attachModel(Model(MODEL_PATH_ZOMBIE,
        glm::translate(mat4(1.0f), vec3(-14.0f, 4.5f, 132.0f)) *                  //Position
        glm::rotate(mat4(1.0f), radians(80.0f), vec3(0.0f, 1.0f, 0.0f)) *         //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(1.0f)), 6));
    stage->attachModel(Model(MODEL_PATH_ZOMBIE,
        glm::translate(mat4(1.0f), vec3(-12.0f, 3.5f, 135.0f)) *                  //Position
        glm::rotate(mat4(1.0f), radians(110.0f), vec3(0.0f, 1.0f, 0.0f)) *        //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(1.0f)), 6));
    stage->attachModel(Model(MODEL_PATH_ZOMBIE,
        glm::translate(mat4(1.0f), vec3(11.0f, 3.5f, 139.0f)) *                   //Position
        glm::rotate(mat4(1.0f), radians(145.0f), vec3(0.0f, 1.0f, 0.0f)) *        //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(1.0f)), 6));

    stage->attachModel(Model(MODEL_PATH_SPIDER,
        glm::translate(mat4(1.0f), vec3(-17.0f, 4.5f, 125.0f)) *                  //Position
        glm::rotate(mat4(1.0f), radians(-145.0f), vec3(0.0f, 1.0f, 0.0f)) *       //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(1.0f)), 6));
    stage->attachModel(Model(MODEL_PATH_SPIDER,
        glm::translate(mat4(1.0f), vec3(16.0f, 3.5f, 127.0f)) *                   //Position
        glm::rotate(mat4(1.0f), radians(-60.0f), vec3(0.0f, 1.0f, 0.0f)) *        //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(1.0f)), 6));
    stage->attachModel(Model(MODEL_PATH_SPIDER,
        glm::translate(mat4(1.0f), vec3(9.0f, 1.5f, 137.0f)) *                    //Position
        glm::rotate(mat4(1.0f), radians(-20.0f), vec3(0.0f, 1.0f, 0.0f)) *        //Orientation around y
        glm::rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)) *		  //Orientation around x
        glm::scale(mat4(1.0f), vec3(1.0f)), 6));

}