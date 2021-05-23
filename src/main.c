#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <glad/glad.h>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include "utils.h"
#include "mat4x4.h"
#include "v3.h"
#include "camera.h"
#include "shader.h"
#include "texture.h"
#include "mesh.h"

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;
static const float FOV = M_PI_2;
static const float Z_NEAR = 0.1f;
static const float Z_FAR = 100.0f;
static const double MOUSE_SENSITIVITY = 0.002f;

static double lastMouseX = (double)WINDOW_WIDTH / 2.0;
static double lastMouseY = (double)WINDOW_HEIGHT / 2.0;

static camera_t playerCamera;
static float dt;

void handleResize(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    unsigned char moveDir = 0x00;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        moveDir |= CAMERA_FORWARD;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        moveDir |= CAMERA_BACKWARD;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        moveDir |= CAMERA_LEFT;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        moveDir |= CAMERA_RIGHT;
    }
    camera_move(&playerCamera, moveDir, dt);
}

void handleMouseMove(GLFWwindow *window, double xPos, double yPos)
{
    double dx = (xPos - lastMouseX);
    double dy = -(yPos - lastMouseY); // down is +ve for mouse coords
    lastMouseX = xPos;
    lastMouseY = yPos;
    camera_turn(&playerCamera, dx * MOUSE_SENSITIVITY, dy * MOUSE_SENSITIVITY);
}

int main(void)
{
    //
    // Create window
    //
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create window");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, handleResize);
    glfwSetCursorPosCallback(window, handleMouseMove);
    //
    // Set up GLAD
    //
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
        exit(EXIT_FAILURE);
    };

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    stbi_set_flip_vertically_on_load(true);
    //
    // Create shader programs
    //
    shader_t objectShader = shader_create(
        "./src/shaders/object.vs",
        "./src/shaders/object.fs");

    v3_t cubePositions[] = {
        v3_create(0.0f, 0.0f, 0.0f),
        v3_create(2.0f, 5.0f, -15.0f),
        v3_create(-1.5f, -2.2f, -2.5f),
        v3_create(-3.8f, -2.0f, -12.3f),
        v3_create(2.4f, -0.4f, -3.5f),
        v3_create(-1.7f, 3.0f, -7.5f),
        v3_create(1.3f, -2.0f, -2.5f),
        v3_create(1.5f, 2.0f, -2.5f),
        v3_create(1.5f, 0.2f, -1.5f),
        v3_create(-1.3f, 1.0f, -1.5f),
    };

    // clang-format on
    //
    // Create mesh
    //
    vertex_t *cubeVerts = utils_malloc(sizeof(vertex_t) * 128);
    int numCubeVerts = mesh_loadVerts(&cubeVerts, "./assets/cube.obj");
    texture_t diffuseMap = texture_load("./assets/container2.png", DIFFUSE);
    texture_t specularMap = texture_load("./assets/container2_specular.png", SPECULAR);
    texture_t meshTextures[] = {diffuseMap, specularMap};
    mesh_t cubeMesh = mesh_create(cubeVerts, numCubeVerts, meshTextures, 2);

    // intialize globals
    playerCamera = camera_create(v3_create(0.0f, 0.0f, 3.0f), -M_PI_2, 0.0f);
    float lastFrame = 0.0f;

    //
    // Update loop
    //
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        dt = currentFrame - lastFrame;
        lastFrame = currentFrame;
        v3_t sunlightDir = v3_create(0.0f, -1.0f, -1.0f);
        v3_t sunlightColor = v3_create(1.0f, 1.0f, 0.5f);

        // inputs
        processInput(window);

        // create transforms
        mat4x4_t view = camera_getViewTransform(playerCamera);
        mat4x4_t projection = mat4x4_createProj((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, FOV, Z_NEAR, Z_FAR);

        // render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //
        // draw cube
        //
        shader_use(objectShader);

        mat4x4_t objectModel = mat4x4_createIdentity();
        objectModel = mat4x4_mul(objectModel, mat4x4_createTranslate(v3_create(0.0, -2.0, 0.0)));
        objectModel = mat4x4_mul(objectModel, mat4x4_createRotX(glfwGetTime()));

        shader_setMat4x4(objectShader, "model", objectModel);
        shader_setMat4x4(objectShader, "view", view);
        shader_setMat4x4(objectShader, "projection", projection);
        shader_setV3(objectShader, "viewPos", playerCamera.pos);

        // sunlight
        shader_setV3(objectShader, "sunlight.dir", sunlightDir);
        shader_setV3(objectShader, "sunlight.ambient", v3_mul(sunlightColor, 0.1f));
        shader_setV3(objectShader, "sunlight.diffuse", v3_mul(sunlightColor, 0.8f));
        shader_setV3(objectShader, "sunlight.specular", v3_mul(sunlightColor, 1.0f));

        // materials
        shader_setFloat(objectShader, "material.shininess", 32.0f);

        for (int i = 0; i < 10; ++i)
        {
            mat4x4_t objectModel = mat4x4_createIdentity();
            objectModel = mat4x4_mul(objectModel, mat4x4_createTranslate(cubePositions[i]));
            objectModel = mat4x4_mul(objectModel, mat4x4_createRotX(currentFrame));
            objectModel = mat4x4_mul(objectModel, mat4x4_createScale(v3_create(0.5f, 0.5f, 0.5f)));
            shader_setMat4x4(objectShader, "model", objectModel);
            mesh_render(cubeMesh, objectShader);
        }

        // update
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}