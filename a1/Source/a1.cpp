// COMP 371 Assignment 1
// Spiral Staircase (Section DD Team 3)
// 
// Badele, Theodor (40129466)
// Fourneaux, Alexander (40022711)
// Reda, Antonio (40155615)
// Zhang, Chi (29783539)
// 
// Based on Labs Framework created by Nicolas Bergeron
/*
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

class Voxel {
public:
    Voxel(vec3 position, int vao, int shaderProgram) : mPosition(position), mvao(vao)
    {
        mWorldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    }

    void Draw() {
        glBindVertexArray(mvao);
        mat4 worldMatrix = translate(mat4(1.0f), mPosition) * rotate(mat4(1.0f), radians(mOrientation.x), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0f), radians(mOrientation.y), vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), radians(mOrientation.z), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(mScale, mScale, mScale));
        glUniformMatrix4fv(mWorldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

public:
    GLuint mWorldMatrixLocation;
    vec3 mPosition;
    vec3 mOrientation = vec3(0.0f, 0.0f, 0.0f);
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
    Shape(vec3 position, vector<struct coordinates> description, int vao, int shaderProgram) : mPosition(position), mvao(vao)
    {
        mWorldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
        int originX = description.front().x;
        int originY = description.front().y;
        int originZ = description.front().z;
        for (auto it = begin(description); it != end(description); ++it) {
            struct coordinates remappedCoordinates = {it->x - originX, it->y - originY, it->z - originZ};
            mDescription.push_back(remappedCoordinates);
            voxels.push_back(Voxel(vec3(remappedCoordinates.x, remappedCoordinates.y, remappedCoordinates.z), vao, shaderProgram));
        }
    }

    void Draw() {
        glBindVertexArray(mvao);
        for (auto it = begin(voxels); it != end(voxels); ++it) {
            mat4 worldMatrix = translate(mat4(1.0f), mPosition) * rotate(mat4(1.0f), radians(mOrientation.x), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0f), radians(mOrientation.y), vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), radians(mOrientation.z), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(mScale, mScale, mScale)) * translate(mat4(1.0f), it->mPosition);
            glUniformMatrix4fv(mWorldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

public:
    GLuint mWorldMatrixLocation;
    vector<struct coordinates> mDescription;
    vector<Voxel> voxels;
    vec3 mPosition;
    vec3 mOrientation = vec3(0.0f, 0.0f, 0.0f);
    float mScale = 1.0f;
    int mvao;
};

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


    // Camera parameters for view transform
    vec3 cameraPosition(0.6f, 1.0f, 10.0f);
    vec3 cameraLookAt(0.0f, 0.0f, -1.0f);
    vec3 cameraUp(0.0f, 1.0f, 0.0f);

    // Other camera parameters
    float cameraSpeed = 1.0f;
    float cameraFastSpeed = 2 * cameraSpeed;
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;
    bool  cameraFirstPerson = true; // press 1 or 2 to toggle this variable

    // Spinning cube at camera position
    float spinningCubeAngle = 0.0f;

    // Set projection matrix for shader, this won't change
    mat4 projectionMatrix = glm::perspective(70.0f,            // field of view in degrees
        4.0f / 3.0f,  // aspect ratio
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
    cout << lastMousePosX;
    // Other OpenGL states to set once before the Game Loop
    // Enable Backface culling
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    GLenum renderingMode = GL_TRIANGLES;
    
    mat4 groundWorldMatrix = translate(mat4(1.0f), vec3(0.0f, -0.01f, 0.0f)) * scale(mat4(1.0f), vec3(1000.0f, 1.10f, 1000.0f));
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

    // Test shape
    struct coordinates coord1 = { 0, 0, 0 };
    struct coordinates coord2 = { 0, 1, 0 };
    struct coordinates coord3 = { 0, -1, 0 };
    struct coordinates coord4 = { 1, 0, 0 };
    struct coordinates coord5 = { 0, 0, 1 };
    struct coordinates coord6 = { 0, 0, 2 };
    vector<struct coordinates> vect{ coord1, coord2, coord3, coord4, coord5, coord6 };

    Shape myShape( vec3(0.0f, 1.0f, 2.0f), vect, vao, shaderProgram );

    // Entering Game Loop
    while (!glfwWindowShouldClose(window))
    {
        // Frame time calculation
        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;

        // Each frame, reset color of each pixel to glClearColor

        // @TODO 1 - Clear Depth Buffer Bit as well
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw geometry
        glBindVertexArray(vao);
        //glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Draw ground
        mat4 groundWorldMatrix = translate(mat4(1.0f), vec3(0.0f, -0.01f, 0.0f)) * scale(mat4(1.0f), vec3(1000.0f, 0.02f, 1000.0f));
        GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &groundWorldMatrix[0][0]);

        //glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0
        

        // Spinning cube at camera position
        spinningCubeAngle += 180.0f * dt;
        
        if (!cameraFirstPerson) {
            // In third person view, let's draw the spinning cube in world space, like any other models
            mat4 spinningCubeWorldMatrix = translate(mat4(1.0f), cameraPosition) *
                rotate(mat4(1.0f), radians(spinningCubeAngle), vec3(0.0f, 1.0f, 0.0f)) *
                scale(mat4(1.0f), vec3(0.1f, 0.1f, 0.1f));

            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &spinningCubeWorldMatrix[0][0]);
        }
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        
        // Draw shape and test transformations
        myShape.Draw();

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // move camera down
        {
            myShape.mOrientation.z += 20 * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) // move camera down
        {
            myShape.mOrientation.z -= 20 * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) // move camera down
        {
            myShape.mOrientation.y += 20 * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) // move camera down
        {
            myShape.mOrientation.y -= 20 * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) // move camera down
        {
            myShape.mScale += 0.2f * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) // move camera down
        {
            myShape.mScale -= 0.2f * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) // move camera down
        {
            myShape.mPosition.z += 2 * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) // move camera down
        {
            myShape.mPosition.z -= 2 * dt;
        }
       

        glBindVertexArray(0);

        // End Frame
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Handle inputs
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) // move camera down
        {
            cameraFirstPerson = true;
        }

        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) // move camera down
        {
            cameraFirstPerson = false;
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


        

        // This was solution for Lab02 - Moving camera exercise
        // We'll change this to be a first or third person camera
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

        // Convert to spherical coordinates
        const float cameraAngularSpeed = 60.0f;
        cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
        cameraVerticalAngle   -= dy * cameraAngularSpeed * dt;

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

        cameraLookAt = vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));
        vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));

        glm::normalize(cameraSideVector);
        
      


         // @TODO 5 = use camera lookat and side vectors to update positions with ASDW
         // adjust code below
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // move camera to the left
        {
            cameraPosition.x -= currentCameraSpeed * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // move camera to the right
        {
            cameraPosition.x += currentCameraSpeed * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // move camera up
        {
            cameraPosition.y -= currentCameraSpeed * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // move camera down
        {
            cameraPosition.y += currentCameraSpeed * dt;
        }

        // TODO 6
        // Set the view matrix for first and third person cameras
        // - In first person, camera lookat is set like below
        // - In third person, camera position is on a sphere looking towards center

        mat4 viewMatrix = mat4(1.0);

        if (cameraFirstPerson) {
            viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
        }
        else {
            float radius = 5.0f;
            glm::vec3 position = cameraPosition - radius * cameraLookAt;
            viewMatrix = lookAt(position, position + cameraLookAt, cameraUp);
        }
      
        
        viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
        GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        // @TODO 2A - Shoot Projectiles
        //
        // shoot projectiles on mouse left click
        // To detect onPress events, we need to check the last state and the current state to detect the state change
        // Otherwise, you would shoot many projectiles on each mouse press
    // ...
    


    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

    // @TODO 3 - Disable mouse cursor
    // ...
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
*/