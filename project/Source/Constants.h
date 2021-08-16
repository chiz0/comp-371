#pragma once
#include <string>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

constexpr int GRID_SIZE = 200;							// The world exists in an nxn grid
constexpr int WALL_SIZE = 12;                           // How many unit cubes in nxn should the wall be
constexpr int FLOOR_SCALE = 5;							// How large a single floor tile is
constexpr float WALL_THICKNESS = 0.1f;                  // How thick is the wall
constexpr int WALL_DISTANCE = 10;						// How far from the model should its wall be
constexpr int MODEL_COUNT = 4;                          // How many models are present in the world
constexpr float STAGE_WIDTH = 50.0f;                    // How far in either direction each model will be placed
constexpr float SCALE_RATE = 0.2f;                      // The rate at which models grow and shrink
constexpr float ROTATE_RATE = 20;                       // The rate at which models rotate
constexpr float TRANSLATE_RATE = 2.0f;                  // The rate at which models move left, right, up, and down
constexpr float CAMERA_JUMP_SPEED = 8.0f;               // The speed at which a camera moves from model to model
constexpr float CAMERA_ANGULAR_SPEED = 5.0f;           // The speed at which the camera rotates
constexpr float CAMERA_SPEED = 2.0f;					// Speed of camera zooming
constexpr vec3 CAMERA_OFFSET = vec3(-2.0f, 2.0f, 10.0f);// The default position of the camera relative to a model
constexpr float FIELD_OF_VIEW = 70.0f;					// The starting zoom level of the camera
constexpr float LIGHT_AMBIENT_STRENGTH = 0.75f;			// Intensity of the ambient light
constexpr float LIGHT_DIFFUSE_STRENGTH = 0.4f;			// Intensity of the diffuse light
constexpr float LIGHT_SPECULAR_STRENGTH = 0.2f;			// Intensity of the specular light
constexpr float SHININESS = 4.0f;						// Shininess coefficient of specular light
constexpr vec3 LIGHT_OFFSET = vec3(0.0f, 50.0f, 10.0f);	// Where, relative to the focused object, to place the light source
constexpr float VIEW_WIDTH = 1024.0f;
constexpr float VIEW_HEIGHT = 768.0f;
constexpr float SHADOW_WIDTH = 1024.0f;
constexpr float SHADOW_HEIGHT = 1024.0f;

constexpr char VERTEX_SHADER_FILEPATH[] = "../Source/TexturedVert.glsl";
constexpr char FRAGMENT_SHADER_FILEPATH[] = "../Source/TexturedFrag.glsl";

constexpr char SHADOW_VERTEX_SHADER_FILEPATH[] = "../Source/ShadowVert.glsl";
constexpr char SHADOW_FRAGMENT_SHADER_FILEPATH[] = "../Source/ShadowFrag.glsl";
constexpr char SHADOW_DEPTH_SHADER_FILEPATH[] = "../Source/ShadowDepth.glsl";

constexpr char* TEXTURE_PATH_TILE = "../Assets/Textures/tile.jpg";
constexpr char* TEXTURE_PATH_METAL = "../Assets/Textures/metal.jpg";
constexpr char* TEXTURE_PATH_BRICK = "../Assets/Textures/brick.jpg";
constexpr char* TEXTURE_PATH_FIRE = "../Assets/Textures/fire.jpg";
constexpr char* TEXTURE_PATH_GRASS = "../Assets/Textures/block/grass_block_top.png";
constexpr char* TEXTURE_PATH_WATER = "../Assets/Textures/block/light_blue_concrete.png";
constexpr char* TEXTURE_PATH_LOG = "../Assets/Textures/block/oak_log.png";
constexpr char* TEXTURE_PATH_LEAVES = "../Assets/Textures/block/green_concrete.png";
constexpr char* TEXTURE_PATH_PLANKS = "../Assets/Textures/block/oak_planks - copia.png";
constexpr char* TEXTURE_PATH_ROCK = "../Assets/Textures/block/cobblestone.png";
constexpr char* TEXTURE_PATH_IRON = "../Assets/Textures/block/iron_ore.png";
constexpr char* TEXTURE_PATH_DIAMOND = "../Assets/Textures/block/diamond_ore.png";
constexpr char* TEXTURE_PATH_OBSIDIAN = "../Assets/Textures/block/obsidian.png";

constexpr char* MODEL_PATH_COW = "../Assets/Models/Cow/source/cow.fbx";
constexpr char* MODEL_PATH_BLAZE = "../Assets/Models/Blaze/source/blaze.fbx";
constexpr char* MODEL_PATH_CHICKEN = "../Assets/Models/Chicken/source/chicken.fbx";
constexpr char* MODEL_PATH_CREEPER = "../Assets/Models/Creeper/Creeper.obj";
constexpr char* MODEL_PATH_DRAGON= "../Assets/Models/Dragon/source/ender_dragon.fbx";
constexpr char* MODEL_PATH_ENDERMAN = "../Assets/Models/Enderman/source/enderman.fbx";
constexpr char* MODEL_PATH_HORSE = "../Assets/Models/Horse/source/horse.fbx";
constexpr char* MODEL_PATH_PIG = "../Assets/Models/Pig/source/pig.fbx";
constexpr char* MODEL_PATH_SHEEP = "../Assets/Models/Sheep/source/sheep.fbx";
constexpr char* MODEL_PATH_SKELETON = "../Assets/Models/Skeleton/skeleton.fbx";
constexpr char* MODEL_PATH_SPIDER = "../Assets/Models/Spider/source/spider.fbx";
constexpr char* MODEL_PATH_WITHERSKELETON = "../Assets/Models/Wither Skeleton/source/wither.fbx";
constexpr char* MODEL_PATH_ZOMBIE = "../Assets/Models/Zombie/source/zombie.fbx";
constexpr char* MODEL_PATH_ROCK = "../Assets/Models/Zombie/source/zombie.fbx";