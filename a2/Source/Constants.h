#pragma once
#include <string>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

constexpr int WALL_SIZE = 12;                           // How many unit cubes in nxn should the wall be
constexpr float WALL_THICKNESS = 0.1f;                  // How thick is the wall
constexpr int WALL_DISTANCE = 10;						// How far from the model should its wall be
constexpr int MODEL_COUNT = 4;                          // How many models are present in the world
constexpr float STAGE_WIDTH = 50.0f;                    // How far in either direction each model will be placed
constexpr float SCALE_RATE = 0.2f;                      // The rate at which models grow and shrink
constexpr float ROTATE_RATE = 20;                       // The rate at which models rotate
constexpr float TRANSLATE_RATE = 2.0f;                  // The rate at which models move left, right, up, and down
constexpr float CAMERA_JUMP_SPEED = 8.0f;               // The speed at which a camera moves from model to model
constexpr float CAMERA_ANGULAR_SPEED = 60.0f;           // The speed at which the camera rotates
constexpr float CAMERA_SPEED = 0.75f;                   // Speed of camera zooming
constexpr vec3 CAMERA_OFFSET = vec3(-2.0f, 2.0f, 10.0f);// The default position of the camera relative to a model
constexpr float LIGHT_AMBIENT_STRENGTH = 0.1f;			// Intensity of the ambient light
constexpr float LIGHT_DIFFUSE_STRENGTH = 0.4f;			// Intensity of the diffuse light
constexpr float LIGHT_SPECULAR_STRENGTH = 0.5f;			// Intensity of the specular light
constexpr float SHININESS = 4.0f;						// Shininess coefficient of specular light
constexpr vec3 LIGHT_OFFSET = vec3(0.0f, 30.0f, 0.0f);	// Where, relative to the focused object, to place the light source

constexpr char VERTEX_SHADER_FILEPATH[] = "../Source/TexturedVert.glsl";
constexpr char FRAGMENT_SHADER_FILEPATH[] = "../Source/TexturedFrag.glsl";