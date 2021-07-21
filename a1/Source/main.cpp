// COMP 371 Assignment 1
// Spiral Staircase (Section DD Team 3)
// 
// Badele, Theodor (40129466)
// Fourneaux, Alexander (40022711)
// Reda, Antonio (40155615)
// Zhang, Chi (29783539)
// 
// Based on Labs Framework created by Nicolas Bergeron

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


using namespace glm;
using namespace std;

////////////////////// CONSTANTS //////////////////////
const int WALL_SIZE = 12;                            // How many unit cubes in nxn should the wall be
const float WALL_THICKNESS = 0.1f;                  // How thick is the wall
const int WALL_DISTANCE = 10 / WALL_THICKNESS;      // How far from the model should its wall be
const int MODEL_COUNT = 4;                          // How many models are present in the world
const float STAGE_WIDTH = 50.0f;                    // How far in either direction each model will be placed
const float SCALE_RATE = 0.2f;                      // The rate at which models grow and shrink
const float ROTATE_RATE = 20;                       // The rate at which models rotate
const float TRANSLATE_RATE = 2.0f;                  // The rate at which models move left, right, up, and down
const float CAMERA_JUMP_SPEED = 8.0f;               // The speed at which a camera moves from model to model
const float CAMERA_SPEED = 0.75f;                   // Speed of camera zooming
const vec3 CAMERA_OFFSET = vec3(-2.0f, 2.0f, 10.0f);// The default position of the camera relative to a model

/////////////////////// MODELS ///////////////////////

class Voxel {
public:
    // Functions
    Voxel(vec3 position, int vao, int shaderProgram, vec3 localScale = vec3(1.0f, 1.0f, 1.0f)) : mPosition(position), mvao(vao), mScaleVector(localScale)
    {
        mWorldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    }

    void Draw(GLenum renderingMode) {
        glBindVertexArray(mvao);
        mat4 worldMatrix = mAnchor * translate(mat4(1.0f), mPosition) * rotate(mat4(1.0f), radians(mOrientation.x), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0f), radians(mOrientation.y), vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), radians(mOrientation.z), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), mScale * mScaleVector);
        glUniformMatrix4fv(mWorldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glDrawArrays(renderingMode, 0, 36);
    }

    // Properties
    GLuint mWorldMatrixLocation;
    mat4 mAnchor = mat4(1.0f);
    vec3 mPosition;
    vec3 mOrientation = vec3(0.0f, 0.0f, 0.0f);
    vec3 mScaleVector = vec3(1.0f, 1.0f, 1.0f);
    float mScale = 1.0f;
    int mvao;
};

struct coordinates
{
    int x;
    int y;
    int z;
};

class Shape {
public:
    // Functions
    Shape(vec3 position, vector<coordinates> description, int vao, int shaderProgram, bool hasWall) : mPosition(position), mvao(vao)
    {
        mWorldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
        bool projection[WALL_SIZE][WALL_SIZE];
        for (int i = 0; i < WALL_SIZE; i++) {
            for (int j = 0; j < WALL_SIZE; j++) {
                projection[i][j] = false;
            }
        }
        int originX = description.front().x;
        int originY = description.front().y;
        int originZ = description.front().z;
        for (auto it = begin(description); it != end(description); ++it) {
            if (it->x + WALL_SIZE / 2 >= 0 && it->x < WALL_SIZE / 2
                && it->y + WALL_SIZE / 2 >= 0 && it->y < WALL_SIZE / 2)
            {
                projection[it->x + WALL_SIZE / 2][it->y + WALL_SIZE / 2] = true;
            }
            struct coordinates remappedCoordinates = { it->x - originX, it->y - originY, it->z - originZ };
            mDescription.push_back(remappedCoordinates);
            voxels.push_back(Voxel(vec3(remappedCoordinates.x, remappedCoordinates.y, remappedCoordinates.z), vao, shaderProgram));
        }

        if (hasWall) {
            // Draw wall
            for (int i = 0; i < WALL_SIZE; i++) {
                for (int j = 0; j < WALL_SIZE; j++) {
                    if (projection[i][j] == false) {    // If the projection does not cover this area, create a wall section here
                        coordinates placedWall = {  };
                        voxels.push_back(Voxel(
                            vec3(
                                i - WALL_SIZE / 2 - originX,    // Wall segment x
                                j - WALL_SIZE / 2 - originY,    // Wall segment y
                                originZ - WALL_DISTANCE         // Wall segment z
                            ), vao, shaderProgram, vec3(1.0f, 1.0f, WALL_THICKNESS)));
                    }
                }
            }
        }
    }

    void Draw(GLenum renderingMode) {
        for (auto it = begin(voxels); it != end(voxels); ++it) {
            mat4 worldMatrix = translate(mat4(1.0f), mPosition) * rotate(mat4(1.0f), radians(mOrientation.x), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0f), radians(mOrientation.y), vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), radians(mOrientation.z), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), it->mScaleVector * mScale);
            it->mAnchor = worldMatrix;
            it->Draw(renderingMode);
        }
    }

    // Properties
    GLuint mWorldMatrixLocation;
    vector<struct coordinates> mDescription;
    vector<Voxel> voxels;
    vec3 mPosition;
    vec3 mOrientation = vec3(0.0f, 0.0f, 0.0f);
    float mScale = 1.0f;
    int mvao;
};

/////////////////////// MAIN ///////////////////////

int compileAndLinkShaders();

int createVertexArrayObject();
int createVertexArrayObjectColoured(vec3 frontBackColour, vec3 topBottomColour, vec3 leftRightColour);

bool initContext();

GLFWwindow* window = NULL;

int main(int argc, char* argv[])
{
    if (!initContext()) return -1;

    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Compile and link shaders here ...
    int shaderProgram = compileAndLinkShaders();

    // We can set the shader once, since we have only one
    glUseProgram(shaderProgram);

    // Other camera parameters
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;

    // Set projection matrix for shader, this won't change
    mat4 projectionMatrix = glm::perspective(70.0f,            // field of view in degrees
        800.0f / 600.0f,  // aspect ratio
        0.01f, 100.0f);   // near and far (near > 0)

    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Define and upload geometry to the GPU here ...
    int vao = createVertexArrayObject();

    // For frame time
    float lastFrameTime = glfwGetTime();
    int lastMouseLeftState = GLFW_RELEASE;
    double lastMousePosX, lastMousePosY;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

    // Other OpenGL states to set once before the Game Loop
    // Enable Backface culling
    glEnable(GL_CULL_FACE);

    // Enable Depth Test
    glEnable(GL_DEPTH_TEST);

    GLenum renderingMode = GL_TRIANGLES;

    //int to keep track of the time
    int temp = 0;

    //see if the camera is to keep going in a certain direction
    bool leftD = false, rightD = false, upD = false, downD = false;

    // Track models
    vector<Shape> shapes;               // Set of all shapes in the world

    ///////// DESIGN MODELS HERE /////////
    // TODO: Add model coordinate descriptions
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
        { 1, -3, 0 },

        { 0, 0, 0 }
    };

    vector<struct coordinates> shape4{
        { 0, 0, 0 },
        { 0, 1, 0 },
        { 0, -1, 0 },
        { 1, 0, 0 },
        { 0, 0, 1 },
        { 0, 0, 2 }
    };

    // TODO: Add model colours
    // I suggest choosing a light colour (i.e. higher values) for the first value (front and back colour). It makes the line flicker less visible on the wall during movement.
    // Chi colour
    int chiColour = createVertexArrayObjectColoured(vec3(0.429f, 0.808f, 0.922f), vec3(0.248f, 0.511f, 0.804f), vec3(0.292f, 0.584f, 0.929f));
    // Alex colour
    int alexColour = createVertexArrayObjectColoured(vec3(0.984f, 0.761f, 0.016f), vec3(0.898f, 0.22f, 0), vec3(0.871f, 0.318f, 0.22f));
    // int colour3
    // int colour4

    shapes.push_back(Shape(vec3(STAGE_WIDTH, 10.0f, STAGE_WIDTH), chiShape, chiColour, shaderProgram, true));
    shapes.push_back(Shape(vec3(-STAGE_WIDTH, 10.0f, STAGE_WIDTH), alexShape, alexColour, shaderProgram, true));
    shapes.push_back(Shape(vec3(STAGE_WIDTH, 10.0f, -STAGE_WIDTH), theoShape, vao, shaderProgram, true));
    shapes.push_back(Shape(vec3(-STAGE_WIDTH, 10.0f, -STAGE_WIDTH), shape4, vao, shaderProgram, true));

    int focusedShape = 0;               // The shape currently being viewed and manipulated
    bool moveCameraToDestination = false;

    const vector<vec3> cameraPositions{
        CAMERA_OFFSET + shapes[0].mPosition,
        CAMERA_OFFSET + shapes[1].mPosition,
        CAMERA_OFFSET + shapes[2].mPosition,
        CAMERA_OFFSET + shapes[3].mPosition
    };


    // Camera parameters for view transform
    vec3 cameraPosition = vec3(0.0f, 1.0f, 20.0f);
    vec3 cameraLookAt(0.0f, -1.0f, 0.0f);
    vec3 cameraUp(0.0f, 1.0f, 0.0f);
    vec3 cameraDestination = cameraPosition;

    // Set initial view matrix
    mat4 viewMatrix = lookAt(cameraPosition,  // eye
        cameraPosition + cameraLookAt,  // center
        cameraUp); // up

    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

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

        // Clear Depth Buffer Bit
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw ground

        glBindVertexArray(groundColour);

        GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
        for (int i = -50; i <= 50; i++) {
            mat4 gridMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, i)) * scale(mat4(1.0f), vec3(100.0f, 0.02f, 0.02f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &gridMatrix[0][0]);
            glDrawArrays(renderingMode, 0, 36);
        }
        for (int i = -50; i <= 50; i++) {
            mat4 gridMatrix = translate(mat4(1.0f), vec3(i, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.02f, 0.02f, 100.0f));
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

        // This was solution for Lab02 - Moving camera exercise
        bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
        float currentCameraSpeed = (fastCam) ? CAMERA_SPEED * 2 : CAMERA_SPEED;


        double mousePosX, mousePosY;
        glfwGetCursorPos(window, &mousePosX, &mousePosY);

        double dx = mousePosX - lastMousePosX;
        double dy = mousePosY - lastMousePosY;

        lastMousePosX = mousePosX;
        lastMousePosY = mousePosY;

        const float cameraAngularSpeed = 60.0f;

        // Lock the camera rotation to be only when the middle and right button are pressed
        if (lastMouseLeftState == GLFW_RELEASE && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
        }
        if (lastMouseLeftState == GLFW_RELEASE && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
            cameraVerticalAngle -= dy * cameraAngularSpeed * dt;
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
            cameraHorizontalAngle += cameraAngularSpeed * dt;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            cameraHorizontalAngle -= cameraAngularSpeed * dt;
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            cameraVerticalAngle += cameraAngularSpeed * dt;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            cameraVerticalAngle -= cameraAngularSpeed * dt;
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

        mat4 viewMatrix = mat4(1.0);

        glm::vec3 position = cameraPosition;
        viewMatrix = lookAt(position, position + cameraLookAt, cameraUp);

        GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

    }


    // Shutdown GLFW
    glfwTerminate();

    return 0;
}

const string VERTEX_SHADER_FILEPATH = "../Source/VertexShader.glsl";
const string FRAGMENT_SHADER_FILEPATH = "../Source/FragmentShader.glsl";

int compileShaderFromSource(string filepath, GLenum shaderType) {
    // Read from file
    ifstream sourceFile(filepath);
    string shaderSource = "";
    string sourceLine;
    if (sourceFile.is_open()) {
        while (getline(sourceFile, sourceLine)) {
            shaderSource += sourceLine + "\n";
        }
        sourceFile.close();
    }
    else {
        cout << "ERROR: Unable to open file.\n";
        return -1;
    }

    // Create and compile shader
    const GLchar* shaderSourcePointer = shaderSource.c_str();
    int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSourcePointer, NULL);
    glCompileShader(shader);

    // Check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Error: Shader compilation failed.\n" << infoLog << std::endl;
    }

    return shader;
}

int compileAndLinkShaders()
{
    int vertexShader = compileShaderFromSource(VERTEX_SHADER_FILEPATH, GL_VERTEX_SHADER);
    int fragmentShader = compileShaderFromSource(FRAGMENT_SHADER_FILEPATH, GL_FRAGMENT_SHADER);

    // Link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
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

int createVertexArrayObjectColoured(vec3 frontBackColour, vec3 topBottomColour, vec3 leftRightColour)
{
    // Cube model
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
        sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);

    GLuint colourVertexBufferObject;
    glGenBuffers(1, &colourVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, colourVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colourVertexArray), colourVertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(vec3),
        (void*)0      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    //glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
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

    // Create Window and rendering context using GLFW, resolution is 800x600
    window = glfwCreateWindow(1024, 768, "COMP 371 - Assignment 1", NULL, NULL);
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