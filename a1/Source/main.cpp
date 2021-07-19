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
const int WALL_SIZE = 8;
const float WALL_THICKNESS = 0.1f;
const int WALL_DISTANCE = 10 / WALL_THICKNESS;

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
        mat4 worldMatrix = translate(mat4(1.0f), mPosition) * rotate(mat4(1.0f), radians(mOrientation.x), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0f), radians(mOrientation.y), vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), radians(mOrientation.z), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), mScale * mScaleVector);
        glUniformMatrix4fv(mWorldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glDrawArrays(renderingMode, 0, 36);
    }

    // Properties
    GLuint mWorldMatrixLocation;
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
                && it->y + WALL_SIZE / 2 >= 0 && it->y  < WALL_SIZE / 2) 
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
                                originZ + WALL_DISTANCE         // Wall segment z
                            ), vao, shaderProgram, vec3(1.0f, 1.0f, WALL_THICKNESS)));
                    }
                }
            }
        }
    }

    void Draw(GLenum renderingMode) {
        glBindVertexArray(mvao);
        for (auto it = begin(voxels); it != end(voxels); ++it) {
            mat4 worldMatrix = translate(mat4(1.0f), mPosition) * rotate(mat4(1.0f), radians(mOrientation.x), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0f), radians(mOrientation.y), vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), radians(mOrientation.z), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), it->mScaleVector * mScale) * translate(mat4(1.0f), it->mPosition);
            glUniformMatrix4fv(mWorldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
            glDrawArrays(renderingMode, 0, 36);
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

    // Scale and translation
    const float scaleRate = 0.5f;
    const float minScale = 0.3f;
    const float moveRate = 0.5f;
    float userScale = 1.0f;
    vec3 userTranslate = vec3(0.0f, 0.0f, 0.0f);

    // Camera parameters for view transform
    float camPosX = -2.0f; float camPosY = 1.0f; float camPosZ = 5.0f;
    vec3 cameraPosition(camPosX, camPosY, camPosZ);
    vec3 cameraLookAt(0.0f, 0.0f, -1.0f);
    vec3 cameraUp(0.0f, 1.0f, 0.0f);

    // Other camera parameters
    float cameraSpeed = 1.0f;
    float cameraFastSpeed = 2 * cameraSpeed;
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;

    // Set projection matrix for shader, this won't change
    mat4 projectionMatrix = glm::perspective(70.0f,            // field of view in degrees
        800.0f / 600.0f,  // aspect ratio
        0.01f, 100.0f);   // near and far (near > 0)

    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Set initial view matrix
    mat4 viewMatrix = lookAt(cameraPosition,  // eye
        cameraPosition + cameraLookAt,  // center
        cameraUp); // up

    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);


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

    // @TODO 1 - Enable Depth Test
    glEnable(GL_DEPTH_TEST);

    GLenum renderingMode = GL_TRIANGLES;

    //int to keep track of the time
    int temp = 0;

    //see if the camera is to keep going in a certain direction
    bool leftD = false, rightD = false, upD = false, downD = false;

    // Test shape
    struct coordinates coord1 = { 0, 0, 0 };
    struct coordinates coord2 = { 0, 1, 0 };
    struct coordinates coord3 = { 0, -1, 0 };
    struct coordinates coord4 = { 1, 0, 0 };
    struct coordinates coord5 = { 0, 0, 1 };
    struct coordinates coord6 = { 0, 0, 2 };
    struct coordinates coord7 = { 0, 10, 0 };
    vector<struct coordinates> vect{ coord1, coord2, coord3, coord4, coord5, coord6, coord7 };

    Shape myShape(vec3(0.0f, 0.0f, 0.0f), vect, vao, shaderProgram, true);

    // Entering Game Loop
    while (!glfwWindowShouldClose(window))
    {
        // Frame time calculation
        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;

        // Each frame, reset color of each pixel to glClearColor

        // @TODO 1 - Clear Depth Buffer Bit as well
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Draw geometry
        glBindVertexArray(vao);
        //glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Draw ground
        GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
        for (int i = -100; i <= 100; i++) {
            mat4 gridMatrix = translate(mat4(1.0f), vec3(0, -10.0f, i)) * scale(mat4(1.0f), vec3(200.0f, 0.02f, 0.02f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &gridMatrix[0][0]);
            glDrawArrays(renderingMode, 0, 36);
        }
        for (int i = -100; i <= 100; i++) {
            mat4 gridMatrix = translate(mat4(1.0f), vec3(i, -10.0f, 0)) * scale(mat4(1.0f), vec3(0.02f, 0.02f, 200.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &gridMatrix[0][0]);
            glDrawArrays(renderingMode, 0, 36);
        }
        /*
        // Draw a single cube
        mat4 cubeMatrix = translate(mat4(1.0f), userTranslate) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f) * userScale);
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &cubeMatrix[0][0]);
        glDrawArrays(renderingMode, 0, 36);

        // Draw a wall in a 9x9 grid
        for (int i = -4; i <= 4; i++) {
            for (int j = -4; j <= 4; j++) {
                if (i == 0 && j == 0) {     // This can easily be modified to handle a 2D projection
                    continue;
                }
                mat4 wallMatrix = translate(mat4(1.0f), vec3(1.0f * i, 1.0f * j, -10.0f) + userTranslate) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 0.1f) * userScale);
                glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &wallMatrix[0][0]);
                glDrawArrays(renderingMode, 0, 36);
            }
        }*/


        // Draw shape and test transformations
        myShape.Draw(renderingMode);


        glBindVertexArray(0);

        // End Frame
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Handle inputs
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
            userScale += scaleRate * dt;
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && userScale > minScale)
            userScale -= scaleRate * dt;

        // This was solution for Lab02 - Moving camera exercise
        bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
        float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;


        // @TODO 4 - Calculate mouse motion dx and dy
        //         - Update camera horizontal and vertical angle


        // Please understand the code when you un-comment it!
        double mousePosX, mousePosY;
        glfwGetCursorPos(window, &mousePosX, &mousePosY);

        double dx = mousePosX - lastMousePosX;
        double dy = mousePosY - lastMousePosY;

        lastMousePosX = mousePosX;
        lastMousePosY = mousePosY;

        const float cameraAngularSpeed = 60.0f;
        temp++;

        //Lock the camera rotation to be only when the middle and right button are pressed
        if (lastMouseLeftState == GLFW_RELEASE && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
        }
        if (lastMouseLeftState == GLFW_RELEASE && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
            cameraVerticalAngle -= dy * cameraAngularSpeed * dt;
        }

        if (lastMouseLeftState == GLFW_RELEASE && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            cameraPosition.z -= currentCameraSpeed * dt;
        }

        if (lastMouseLeftState == GLFW_RELEASE && !glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS
            && !glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS
            && !glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {

            if (temp % 100 == 0) {
                double tdx = dx;
                double tdy = dy;

                if (dx < 0) {
                    tdx *= -1;
                }
                if (dy < 0) {
                    tdy *= -1;
                }
                if (tdx > tdy) {
                    if (dx < 0) {
                        if (!rightD) {
                            leftD = true;
                            rightD = false;
                            upD = false;
                            downD = false;
                        }
                        else {
                            rightD = false;
                            leftD = false;
                            upD = false;
                            downD = false;
                        }
                    }
                    else if (dx > 0) {
                        if (!leftD) {
                            rightD = true;
                            leftD = false;
                            upD = false;
                            downD = false;
                        }
                        else {
                            leftD = false;
                            rightD = false;
                            upD = false;
                            downD = false;
                        }
                    }
                }
                else {
                    if (dy < 0) {
                        if (!downD) {
                            upD = true;
                            rightD = false;
                            leftD = false;
                            downD = false;
                        }
                        else {
                            downD = false;
                            rightD = false;
                            leftD = false;
                            upD = false;
                        }
                    }
                    else if (dy > 0) {
                        if (!upD) {
                            downD = true;
                            rightD = false;
                            leftD = false;
                            upD = false;
                        }
                        else {
                            upD = false;
                            rightD = false;
                            leftD = false;
                            downD = false;
                        }
                    }
                }
            }
            //Checking if it keeps going in the direction   
            if (leftD) {
                cameraPosition.x -= currentCameraSpeed * dt;
            }
            if (rightD) {
                cameraPosition.x += currentCameraSpeed * dt;
            }
            if (upD) {
                cameraPosition.y += currentCameraSpeed * dt;
            }
            if (downD) {
                cameraPosition.y -= currentCameraSpeed * dt;
            }
        }



        //Change orientation with the arrow keys
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
            //initial position
            cameraPosition.x = camPosX; camPosY = 1.0f; cameraPosition.z = camPosZ;
            cameraLookAt.x = 0.0f; cameraLookAt.y = 0.0f; cameraLookAt.z = -1.0f;
            cameraUp.x = 0.0f; cameraUp.y = 1.0f; cameraUp.z = 0.0f;

            //initial orientation
            cameraHorizontalAngle = 90.0f;
            cameraVerticalAngle = 0.0f;
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


        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // Rz
        {
            myShape.mOrientation.z += 20 * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) // R-z
        {
            myShape.mOrientation.z -= 20 * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) // Ry
        {
            myShape.mOrientation.y += 20 * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) // R-y
        {
            myShape.mOrientation.y -= 20 * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) // grow object
        {
            myShape.mScale += 0.2f * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) // shrink object
        {
            myShape.mScale -= 0.2f * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) // move object forward
        {
            myShape.mPosition.z += 2 * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) // move object backward
        {
            myShape.mPosition.z -= 2 * dt;
        }
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) // move camera down
        {
            renderingMode = GL_TRIANGLES;
        }

        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) // move camera down
        {
            renderingMode = GL_LINE_LOOP;
        }

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) // move camera down
        {
            renderingMode = GL_POINTS;
        }

        // TODO 6
        // Set the view matrix for first and third person cameras
        // - In first person, camera lookat is set like below
        // - In third person, camera position is on a sphere looking towards center
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
    const vec3 orange1 = vec3(0.984f, 0.761f, 0.016f);
    const vec3 orange2 = vec3(0.898f, 0.22f, 0);
    const vec3 orange3 = vec3(0.6f, 0.6f, 0.6f);
    const vec3 orange4 = vec3(0.871f, 0.318f, 0.22f);
    const vec3 orange5 = vec3(0.996f, 0.824f, 0.463f);
    const vec3 orange6 = vec3(0.996f, 0.58f, 0.255f);

    vec3 vertexArray[] = {  // position,                            color
        vec3(-0.5f,-0.5f,-0.5f), orange1, //left - red
        vec3(-0.5f,-0.5f, 0.5f), orange1,
        vec3(-0.5f, 0.5f, 0.5f), orange1,

        vec3(-0.5f,-0.5f,-0.5f), orange1,
        vec3(-0.5f, 0.5f, 0.5f), orange1,
        vec3(-0.5f, 0.5f,-0.5f), orange1,

        vec3(0.5f, 0.5f,-0.5f), orange2, // far - blue
        vec3(-0.5f,-0.5f,-0.5f), orange2,
        vec3(-0.5f, 0.5f,-0.5f), orange2,

        vec3(0.5f, 0.5f,-0.5f), orange2,
        vec3(0.5f,-0.5f,-0.5f), orange2,
        vec3(-0.5f,-0.5f,-0.5f), orange2,

        vec3(0.5f,-0.5f, 0.5f), orange3, // bottom - turquoise
        vec3(-0.5f,-0.5f,-0.5f), orange3,
        vec3(0.5f,-0.5f,-0.5f), orange3,

        vec3(0.5f,-0.5f, 0.5f), orange3,
        vec3(-0.5f,-0.5f, 0.5f), orange3,
        vec3(-0.5f,-0.5f,-0.5f), orange3,

        vec3(-0.5f, 0.5f, 0.5f), orange4, // near - green
        vec3(-0.5f,-0.5f, 0.5f), orange4,
        vec3(0.5f,-0.5f, 0.5f), orange4,

        vec3(0.5f, 0.5f, 0.5f), orange4,
        vec3(-0.5f, 0.5f, 0.5f), orange4,
        vec3(0.5f,-0.5f, 0.5f), orange4,

        vec3(0.5f, 0.5f, 0.5f), orange5, // right - purple
        vec3(0.5f,-0.5f,-0.5f), orange5,
        vec3(0.5f, 0.5f,-0.5f), orange5,

        vec3(0.5f,-0.5f,-0.5f), orange5,
        vec3(0.5f, 0.5f, 0.5f), orange5,
        vec3(0.5f,-0.5f, 0.5f), orange5,

        vec3(0.5f, 0.5f, 0.5f), orange6, // top - yellow
        vec3(0.5f, 0.5f,-0.5f), orange6,
        vec3(-0.5f, 0.5f,-0.5f), orange6,

        vec3(0.5f, 0.5f, 0.5f), orange6,
        vec3(-0.5f, 0.5f,-0.5f), orange6,
        vec3(-0.5f, 0.5f, 0.5f), orange6
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
    window = glfwCreateWindow(800, 600, "Comp371 - Lab 03", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    // @TODO 3 - Disable mouse cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return false;
    }
    return true;
}
