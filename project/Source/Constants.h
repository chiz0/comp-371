#pragma once
#include <string>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

constexpr int GRID_SIZE = 200;							// The world exists in an nxn grid
constexpr int WALL_SIZE = 12;                           // How many unit cubes in nxn should the wall be
constexpr int FLOOR_SCALE = 5;							// How large a single floor tile is
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
constexpr float FAR_PLANE = 100.0f;
constexpr float GRAVITY = 4.0f;
constexpr float BURST_SCALE = 0.3f;
constexpr float BURST_DISTANCE_FROM_CAMERA = 1.0f;
constexpr float FLAME_RISE = 1.0f;
constexpr float FLAME_SCALE = 0.2f;

constexpr float ANIMATE_CREATION_MOVE_SPEED = 20.0f;
constexpr int ANIMATE_CREATION_VOXEL_SPREAD = 10;
constexpr float ANIMATE_ROTATE_SPEED = 10.0f;

// SHADERS
constexpr char VERTEX_SHADER_FILEPATH[] = "../Source/TexturedVert.glsl";
constexpr char FRAGMENT_SHADER_FILEPATH[] = "../Source/TexturedFrag.glsl";

constexpr char SHADOW_VERTEX_SHADER_FILEPATH[] = "../Source/ShadowVert.glsl";
constexpr char SHADOW_FRAGMENT_SHADER_FILEPATH[] = "../Source/ShadowFrag.glsl";
constexpr char SHADOW_DEPTH_SHADER_FILEPATH[] = "../Source/ShadowDepth.glsl";

// TEXTURES
constexpr char* TEXTURE_PATH_TILE = "../Assets/Textures/tile.jpg";
constexpr char* TEXTURE_PATH_METAL = "../Assets/Textures/metal.jpg";
constexpr char* TEXTURE_PATH_BRICK = "../Assets/Textures/brick.jpg";
constexpr char* TEXTURE_PATH_FIRE = "../Assets/Textures/fire.jpg";
constexpr char* TEXTURE_PATH_PARTICLE = "../Assets/Textures/particle.jpg";
constexpr char* TEXTURE_PATH_GRASS = "../Assets/Textures/block/grass_block_top.png";
constexpr char* TEXTURE_PATH_WATER = "../Assets/Textures/block/light_blue_concrete.png";
constexpr char* TEXTURE_PATH_LOG = "../Assets/Textures/block/oak_log.png";
constexpr char* TEXTURE_PATH_LEAVES = "../Assets/Textures/block/green_concrete.png";
constexpr char* TEXTURE_PATH_PLANKS = "../Assets/Textures/block/oak_planks - copia.png";
constexpr char* TEXTURE_PATH_ROCK = "../Assets/Textures/block/cobblestone.png";
constexpr char* TEXTURE_PATH_IRON = "../Assets/Textures/block/iron_ore.png";
constexpr char* TEXTURE_PATH_DIAMOND = "../Assets/Textures/block/diamond_ore.png";
constexpr char* TEXTURE_PATH_OBSIDIAN = "../Assets/Textures/block/obsidian.png";
constexpr char* TEXTURE_PATH_NETHERRACK = "../Assets/Textures/block2/netherrack.png";
constexpr char* TEXTURE_PATH_LAVA = "../Assets/Textures/block/lava_still.png";
constexpr char* TEXTURE_PATH_ENDSTONE = "../Assets/Textures/block2/end_stone_bricks.png";

// MODELS
constexpr char* MODEL_PATH_COW = "../Assets/Models/Cow/cow+mooshroom.obj";
constexpr char* MODEL_PATH_BLAZE = "../Assets/Models/Blaze/untitled.obj";
constexpr char* MODEL_PATH_CHICKEN = "../Assets/Models/Chicken/chicken.obj";
constexpr char* MODEL_PATH_CREEPER = "../Assets/Models/Creeper/creeper.obj";
constexpr char* MODEL_PATH_DRAGON= "../Assets/Models/Dragon/enderdragon.obj";
constexpr char* MODEL_PATH_ENDERMAN = "../Assets/Models/Enderman/enderman.obj";
constexpr char* MODEL_PATH_HORSE = "../Assets/Models/Horse/horse+zombiehorse+skeltonhorse+mule+donkey.obj";
constexpr char* MODEL_PATH_PIG = "../Assets/Models/Pig/pig.obj";
constexpr char* MODEL_PATH_SHEEP = "../Assets/Models/Sheep/sheep.obj";
constexpr char* MODEL_PATH_SKELETON = "../Assets/Models/Skeleton/skeleton+skeleton_stray+wither_skeleton.obj";
constexpr char* MODEL_PATH_SPIDER = "../Assets/Models/Spider/spider.obj";
constexpr char* MODEL_PATH_WITHERSKELETON = "../Assets/Models/Wither Skeleton/skeleton+skeleton_stray+wither_skeleton.obj";
constexpr char* MODEL_PATH_ZOMBIE = "../Assets/Models/Zombie/zombie.obj";
constexpr char* MODEL_PATH_CYLINDER = "../Assets/Models/CYLINDER/cylinder.obj";

// AUDIO
constexpr char* AUDIO_PATH_MUSIC = "../Assets/Audio/mii.ogg";
constexpr char* AUDIO_PATH_WOW = "../Assets/Audio/wow.ogg";
constexpr char* AUDIO_PATH_BRUH = "../Assets/Audio/bruh.ogg";