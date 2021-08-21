#pragma once
#include <string>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

constexpr int GRID_SIZE = 200;								// The world exists in an nxn grid
constexpr int WALL_SIZE = 12;								// How many unit cubes in nxn should the wall be
constexpr int FLOOR_SCALE = 5;								// How large a single floor tile is
constexpr float WALL_THICKNESS = 0.1f;						// How thick is the wall
constexpr int WALL_DISTANCE = 20;							// How far from the model should its wall be
constexpr int MODEL_COUNT = 4;								// How many models are present in the world
constexpr float STAGE_WIDTH = 50.0f;						// How far in either direction each model will be placed
constexpr float SCALE_RATE = 0.2f;							// The rate at which models grow and shrink
constexpr float ROTATE_RATE = 20;							// The rate at which models rotate
constexpr float TRANSLATE_RATE = 2.0f;						// The rate at which models move left, right, up, and down
constexpr float CAMERA_JUMP_SPEED = 8.0f;					// The speed at which a camera moves from model to model
constexpr float CAMERA_ANGULAR_SPEED = 10.0f;				// The speed at which the camera rotates
constexpr float CAMERA_SPEED = 10.0f;						// Speed of camera zooming
constexpr vec3 CAMERA_OFFSET = vec3(0.0f, 0.0f, 10.0f);		// The default position of the camera relative to a model
constexpr float FIELD_OF_VIEW = 70.0f;						// The starting zoom level of the camera
constexpr float VIEW_WIDTH = 1024.0f;						// Default width of the game window at launch
constexpr float VIEW_HEIGHT = 768.0f;						// Default height of the game window at launch
constexpr float SHADOW_WIDTH = 1024.0f;						// Width of the viewport used by the shadow shader
constexpr float SHADOW_HEIGHT = 1024.0f;					// Height of the viewport used by the shadow shader
constexpr float NEAR_PLANE = 0.01f;							// How close to the camera something may be drawn
constexpr float FAR_PLANE = 200.0f;							// How far away to draw objects
constexpr float SUN_Z_OFFSET = 50.0f;						// How far along the Z axis from the camera should the sun be placed
constexpr float WORLD_SIZE = 400.0f;						// The length on the Z axis of each world (Overworld, Nether, & End separately)

// Difficulty values
constexpr vec3 STAGE_STARTING_LOCATION = { 0, -20, 10 };     // The starting position of the stage origin
constexpr float STAGE_INITIAL_SCALE = 2.0f;                 // The initial scale for the stage
constexpr float INITIAL_STAGE_SPEED = 4.0f;                 // The starting speed for the stage moving towards the camera
constexpr float INITIAL_WALL_SPEED = 2.0f;					// The starting speed of walls moving towards objects
constexpr int STARTING_DIFFICULTY = 9;                      // The starting number of cubes in the shape
constexpr float DIFFICULTY_SPEED_GROWTH = 0.1f;				// How much to increment the wall speed as difficulty increases
constexpr float DIFFICULTY_SPEED_MAX = 4.0f;				// How much to increment the wall speed as difficulty increases
constexpr int WORLDS = 3;                                   // Number of worlds in the game (overworld, nether, end)

// Lighting
constexpr float LIGHT_AMBIENT_STRENGTH = 0.4f;				// Intensity of the ambient light
constexpr float LIGHT_DIFFUSE_STRENGTH = 0.5f;				// Intensity of the diffuse light
constexpr float LIGHT_SPECULAR_STRENGTH = 0.9f;				// Intensity of the specular light
constexpr float SHININESS = 4.0f;							// Shininess coefficient of specular light
constexpr vec3 LIGHT_COLOUR_DAY = vec3(1.0f, 1.0f, 1.0f);	// Ambient colour in daytime
constexpr vec3 LIGHT_COLOUR_NIGHT = vec3(0.1f, 0.1f, 0.4f);	// Ambient colour in night time
constexpr vec3 LIGHT_COLOUR_NETHER = vec3(1.0f, 0.3f, 0.3f);// Ambient colour in the nether
constexpr vec3 LIGHT_COLOUR_END = vec3(1.0f, 1.0f, 1.0f);	// Ambient colour in the end
constexpr float AMBIENT_BOOST = 3.0f;                       // A value 1.0f - 10.0f to increase the ambient lighting of shapes and walls
constexpr vec4 worldSkyColours[WORLDS] = {
    vec4(0.529f, 0.808f, 0.922f, 1.0f),                     // Overworld sky colour
    vec4(0.40f, 0.208f, 0.222f, 1.0f),                      // Nether sky colour
    vec4(0.0f, 0.0f, 0.0f, 1.0f)                            // End sky colour
};

// Particle values
constexpr float GRAVITY = 4.0f;								// Downward force applied to some particles
constexpr float BURST_SCALE = 0.3f;							// How large each burst particle should be
constexpr float FLAME_RISE = 1.0f;							// How quickly flames should accelerate upward from the origin
constexpr float FLAME_SCALE = 0.2f;							// How large each flame particle should be
constexpr int DEFAULT_BURST_AMOUNT = 100;					// How many particles should be in a default burst
constexpr float DEFAULT_BURST_FORCE = 10.0f;				// How powerful a default burst should be
constexpr int DEFAULT_FLAME_AMOUNT = 1.0f;				    // How many particles emerge per frame from a flame
constexpr float DEFAULT_FLAME_FORCE = 1.0f;				    // How quickly a flame particle should move

// SHADERS
constexpr char VERTEX_SHADER_FILEPATH[] = "../Source/TexturedVert.glsl";
constexpr char FRAGMENT_SHADER_FILEPATH[] = "../Source/TexturedFrag.glsl";

constexpr char SHADOW_VERTEX_SHADER_FILEPATH[] = "../Source/ShadowVert.glsl";
constexpr char SHADOW_FRAGMENT_SHADER_FILEPATH[] = "../Source/ShadowFrag.glsl";
constexpr char SHADOW_DEPTH_SHADER_FILEPATH[] = "../Source/ShadowDepth.glsl";

// TEXTURES
constexpr char* TEXTURE_PATH_TITLE = "../Assets/Textures/titlescreen.png";
constexpr char* TEXTURE_PATH_THANKYOU = "../Assets/Textures/thankyou.png";
constexpr char* TEXTURE_PATH_TILE = "../Assets/Textures/tile.jpg";
constexpr char* TEXTURE_PATH_BLOCK = "../Assets/Textures/emerald.jpg";
constexpr char* TEXTURE_PATH_BRICK = "../Assets/Textures/bricks.png";
constexpr char* TEXTURE_PATH_FIRE = "../Assets/Textures/fire.jpg";
constexpr char* TEXTURE_PATH_PARTICLE = "../Assets/Textures/particle.jpg";
constexpr char* TEXTURE_PATH_SUN = "../Assets/Textures/sun.jpg";
constexpr char* TEXTURE_PATH_MOON = "../Assets/Textures/moon.jpg";
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
constexpr char* TEXTURE_PATH_ENDSPACE = "../Assets/Textures/block/dragon_egg.png";
constexpr char* TEXTURE_PATH_WARPWOOD = "../Assets/Textures/block2/warped_stem.png";
constexpr char* TEXTURE_PATH_WARPLEAVES = "../Assets/Textures/block2/warped_wart_block.png";
constexpr char* TEXTURE_PATH_WARPPLANKS = "../Assets/Textures/block2/warped_door_top.png";
constexpr char* TEXTURE_PATH_NETHERBRICKS = "../Assets/Textures/block2/nether_bricks.png";
constexpr char* TEXTURE_PATH_ENDPORTAL = "../Assets/Textures/block2/end_portal_frame_top.png";

//UI
constexpr char* MODEL_PATH_L = "../Assets/Models/UI/Score/L.obj";
constexpr char* MODEL_PATH_TIME = "../Assets/Models/UI/Time/Time.obj";
constexpr char* MODEL_PATH_ZERO = "../Assets/Models/UI/Time/zero.obj";
constexpr char* MODEL_PATH_COLON = "../Assets/Models/UI/Time/colon.obj";
constexpr char* MODEL_PATH_ONE = "../Assets/Models/UI/Time/one.obj";
constexpr char* MODEL_PATH_TWO = "../Assets/Models/UI/Time/two.obj";
constexpr char* MODEL_PATH_THREE = "../Assets/Models/UI/Time/three.obj";
constexpr char* MODEL_PATH_FOUR = "../Assets/Models/UI/Time/four.obj";
constexpr char* MODEL_PATH_FIVE = "../Assets/Models/UI/Time/five.obj";
constexpr char* MODEL_PATH_SIX = "../Assets/Models/UI/Time/six.obj";
constexpr char* MODEL_PATH_SEVEN = "../Assets/Models/UI/Time/seven.obj";
constexpr char* MODEL_PATH_EIGHT = "../Assets/Models/UI/Time/eight.obj";
constexpr char* MODEL_PATH_NINE = "../Assets/Models/UI/Time/nine.obj";

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
constexpr char* MODEL_PATH_PIGLIN = "../Assets/Models/Piglin/piglin.obj";
constexpr char* MODEL_PATH_ZOGLIN = "../Assets/Models/Zoglin/zoglin.obj";
constexpr char* MODEL_PATH_HOGLIN = "../Assets/Models/Zoglin/hoglin.obj";
constexpr char* MODEL_PATH_BLOCK = "../Assets/Models/Block/mega_brick.obj";
constexpr char* MODEL_PATH_GHAST = "../Assets/Models/Ghast/ghast.obj";
constexpr char* MODEL_PATH_ENDTOWER = "../Assets/Models/EndTower/block.obj";
constexpr char* MODEL_PATH_EGG = "../Assets/Models/Egg/egg.obj";
constexpr char* MODEL_PATH_FDRAGON = "../Assets/Models/Dragon/fancy/enderdragon.obj";

// AUDIO
constexpr char* AUDIO_PATH_MUSIC = "../Assets/Audio/mii.ogg";
constexpr char* AUDIO_PATH_WOW = "../Assets/Audio/wow.ogg";
constexpr char* AUDIO_PATH_BRUH = "../Assets/Audio/bruh.ogg";
constexpr char* AUDIO_PATH_EXPLOSION = "../Assets/Audio/explosion.ogg";
constexpr char* AUDIO_PATH_LAUNCH = "../Assets/Audio/launch.ogg";
constexpr char* AUDIO_PATH_CHIMES = "../Assets/Audio/chimes.ogg";
constexpr char* AUDIO_PATH_IMPACT = "../Assets/Audio/impact.ogg";
constexpr char* AUDIO_PATH_WOOSH = "../Assets/Audio/woosh.ogg";
constexpr char* AUDIO_PATH_BLAST = "../Assets/Audio/blast.ogg";
constexpr char* AUDIO_PATH_OVERWORLD = "../Assets/Audio/cat.ogg";
constexpr char* AUDIO_PATH_NETHER = "../Assets/Audio/pigstep.ogg";
constexpr char* AUDIO_PATH_END = "../Assets/Audio/ward.ogg";