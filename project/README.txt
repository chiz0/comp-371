COMP 371 Final Project
 
Badele, Theodor (40129466)
Fourneaux, Alexander (40022711)
Reda, Antonio (40155615)
Zhang, Chi (29783539)

--------------------------------------------------------------------------------------------------------
Welcome to SuperHyperCube - Copyright Infringement Edition
Presented by Team Spiral Staircase
--------------------------------------------------------------------------------------------------------

IMPORTANT
--------------------------------------------------------------------------------------------------------
When building this project, please ensure you target x64 in the Visual Studio configuration manager.


OVERVIEW
--------------------------------------------------------------------------------------------------------
In this game, the player is presented with a randomly generated shape built of several cubic voxels, and
an associated hole in the wall. The player must rotate the shape using the WSAD keys until it fits
through the hole. The wall slowly approaches the shape, and the player must orient the shape correctly
before the two make contact. If, upon contact between the wall and shape, the shape fits through the hole,
the player is awarded one point. The game proceeds through three game worlds before displaying an end
screen and closing automatically. Throughout the game, the player sees their score and time written in
the sky.

 
SPECIAL FEATURES
--------------------------------------------------------------------------------------------------------
Here we highlight the features added to the game which were not required by the specifications.

Difficulty increases with each shape. The shape begins small with 9 voxels, but grows each time the
user passes a wall. In the first world, it increases by one voxel. In the second, by two voxels, and
in the third by three voxels.

The game progresses through three Minecraft-themed worlds:
- The Overworld is a grassy plains decorated with wooden scraps, caves, and monsters. Lighting in this
  world is dynamic, with a moving light source in the form of sun and moon, and a fade in light between
  dawn and dusk. The final half of the overworld takes place in night time, with a smaller amount of
  moonlight which casts a slightly blue light.
- The Nether is the second world, a hellish plane of red rock, lava, and monstrous pigmen. The lighting
  in this world sits below the shape and casts a red glow from the lava.
- The End is a hollow spacelike dimension which casts dramatic shadows and ends with a dramatic scene of
  dragons.

The game contains music from Minecraft which changes by world:
Overworld : C418 cat
Nether :    Lena Raine Pigstep
End :       C418 Ward
In addition, Minecraft sound effects appear in game:
On successful completion of a world, fireworks launching can be heard
On failure of a world, a clattering impact is played
On world transition, chimes are played

Rotation of shapes is handled by quaternions rather than euler angles. This gives the player a
consistent perspective on how to orient a shape; regardless of the shape's original "up" direction,
pressing D will always rotate in the same direction relative to the player's view.

The game contains animations and particle effects. On creation and destruction of a shape and wall,
voxels fly in and out of the world to gather into their final form. When the player clears a wall,
two confetti cannons blast colourful particles in all directions, which drop to the ground via gravity. 
Spread out among the worlds are fire spots, where smoke particles rise into the air.

To aid performance, the world is rendered in chunks, which are drawn only as they come close to the
camera. In addition, since the user's camera is clamped to face one direction, chunks which pass behind
the camera are deleted and no longer drawn.

Shapes are generated randomly, and a 2D projection is created in the wall before applying random
rotations to the shape. By this method, however, it is possible for there to be multiple correct
orientations for a shape. Because of this, correctness of orientation is not a simple flag of the
right direction, but rather rudimentary collision detection is used to create a new 2D projection
as the shape collides, and is compared with the projection on the wall.
Please note, this method conflicts with the specifications, but the design was approved by
professor Mendhurwar on the condition that the resulting shapes were complex enough to remain a 
challenge.


CONTROLS
--------------------------------------------------------------------------------------------------------

Mouse movement:	Orbit the camera around the shape
Space bar:	Start the game
W/S		Rotate the shape 90 degrees around the +/- X axis. Can be held for multiple rotations.
A/D		Rotate the shape 90 degrees around the +/- Y axis. Can be held for multiple rotations.