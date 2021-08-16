#pragma once
#include <string>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

constexpr int GRID_SIZE = 100;							// The world exists in an nxn grid
constexpr int WALL_SIZE = 12;                           // How many unit cubes in nxn should the wall be
constexpr int FLOOR_SCALE = 25;							// How large a single floor tile is
constexpr float WALL_THICKNESS = 0.1f;                  // How thick is the wall
constexpr int WALL_DISTANCE = 20;						// How far from the model should its wall be
constexpr int MODEL_COUNT = 4;                          // How many models are present in the world
constexpr float STAGE_WIDTH = 50.0f;                    // How far in either direction each model will be placed
constexpr float SCALE_RATE = 0.2f;                      // The rate at which models grow and shrink
constexpr float ROTATE_RATE = 20;                       // The rate at which models rotate
constexpr float TRANSLATE_RATE = 2.0f;                  // The rate at which models move left, right, up, and down
constexpr float CAMERA_JUMP_SPEED = 8.0f;               // The speed at which a camera moves from model to model
constexpr float CAMERA_ANGULAR_SPEED = 60.0f;           // The speed at which the camera rotates
constexpr float CAMERA_SPEED = 10.0f;					// Speed of camera zooming
constexpr vec3 CAMERA_OFFSET = vec3(0.0f, 0.0f, 0.0f);// The default position of the camera relative to a model
constexpr float FIELD_OF_VIEW = 70.0f;					// The starting zoom level of the camera
constexpr float LIGHT_AMBIENT_STRENGTH = 0.3f;			// Intensity of the ambient light
constexpr float LIGHT_DIFFUSE_STRENGTH = 0.6f;			// Intensity of the diffuse light
constexpr float LIGHT_SPECULAR_STRENGTH = 0.9f;			// Intensity of the specular light
constexpr float SHININESS = 4.0f;						// Shininess coefficient of specular light
constexpr vec3 LIGHT_OFFSET = vec3(0.0f, 10.0f, 0.0f);	// Where, relative to the focused object, to place the light source
constexpr float VIEW_WIDTH = 1024.0f;
constexpr float VIEW_HEIGHT = 768.0f;
constexpr float SHADOW_WIDTH = 1024.0f;
constexpr float SHADOW_HEIGHT = 1024.0f;
constexpr float NEAR_PLANE = 0.01f;
constexpr float FAR_PLANE = 300.0f;
constexpr float GRAVITY = 4.0f;
constexpr float BURST_SCALE = 0.3f;
constexpr float BURST_DISTANCE_FROM_CAMERA = 1.0f;
constexpr float FLAME_RISE = 1.0f;
constexpr float FLAME_SCALE = 0.2f;

constexpr char VERTEX_SHADER_FILEPATH[] = "../Source/TexturedVert.glsl";
constexpr char FRAGMENT_SHADER_FILEPATH[] = "../Source/TexturedFrag.glsl";

constexpr char SHADOW_VERTEX_SHADER_FILEPATH[] = "../Source/ShadowVert.glsl";
constexpr char SHADOW_FRAGMENT_SHADER_FILEPATH[] = "../Source/ShadowFrag.glsl";
constexpr char SHADOW_DEPTH_SHADER_FILEPATH[] = "../Source/ShadowDepth.glsl";

constexpr char PARTICLE_VERTEX_SHADER_FILEPATH[] = "../Source/ParticleVertex.glsl";
constexpr char PARTICLE_FRAGMENT_SHADER_FILEPATH[] = "../Source/ParticleFragment.glsl";

constexpr char* TEXTURE_PATH_TILE = "../Assets/Textures/tile.jpg";
constexpr char* TEXTURE_PATH_METAL = "../Assets/Textures/metal.jpg";
constexpr char* TEXTURE_PATH_BRICK = "../Assets/Textures/brick.jpg";
constexpr char* TEXTURE_PATH_FIRE = "../Assets/Textures/fire.jpg";
constexpr char* TEXTURE_PATH_PARTICLE = "../Assets/Textures/particle.jpg";

constexpr char* AUDIO_PATH_MUSIC = "../Assets/Audio/mii.ogg";
constexpr char* AUDIO_PATH_WOW = "../Assets/Audio/wow.ogg";
constexpr char* AUDIO_PATH_BRUH = "../Assets/Audio/bruh.ogg";