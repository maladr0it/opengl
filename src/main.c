#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <glad/glad.h>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include "utils.h"
#include "mat4x4.h"
#include "v3.h"
#include "camera.h"
#include "shader.h"
#include "texture.h"

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

    //
    // Create shader programs
    //
    shader_t objectShader = shader_create(
        "./src/shaders/object.vs",
        "./src/shaders/object.fs");

    shader_t lightShader = shader_create(
        "./src/shaders/light.vs",
        "./src/shaders/light.fs");

    //
    // Create data
    //
    // clang-format off
    float verts[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    
    v3_t cubePositions[] = {
        v3_create( 0.0f,  0.0f,  0.0f),
        v3_create( 2.0f,  5.0f, -15.0f),
        v3_create(-1.5f, -2.2f, -2.5f),
        v3_create(-3.8f, -2.0f, -12.3f),
        v3_create( 2.4f, -0.4f, -3.5f),
        v3_create(-1.7f,  3.0f, -7.5f),
        v3_create( 1.3f, -2.0f, -2.5f),
        v3_create( 1.5f,  2.0f, -2.5f),
        v3_create( 1.5f,  0.2f, -1.5f),
        v3_create(-1.3f,  1.0f, -1.5f),
    };
    // clang-format on

    //
    // create buffers and vertex arrays
    //
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // object
    unsigned int objectVAO;
    glGenVertexArrays(1, &objectVAO);
    glBindVertexArray(objectVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // light
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    //
    // Create textures
    //
    unsigned int diffuseMap = texture_load("./assets/container2.png");
    unsigned int specularMap = texture_load("./assets/container2_specular.png");
    unsigned int emissionMap = texture_load("./assets/matrix.jpg");

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

        // inputs
        processInput(window);

        // create transforms
        mat4x4_t view = camera_getViewTransform(playerCamera);
        mat4x4_t projection = mat4x4_createProj((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, FOV, Z_NEAR, Z_FAR);

        // render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float time = glfwGetTime();

        v3_t lightColor = v3_create(0.5f + sin(time * 2.0f) / 2.0f, 0.5f + sin(time * 0.7f) / 2.0f, 0.5f + sin(time * 1.3f) / 2.0);
        lightColor = v3_create(1.0, 1.0, 1.0);

        float lightPathRadius = 10.0f;
        v3_t lightPos = v3_create(sinf(time) * lightPathRadius, 0.0f, cosf(time) * lightPathRadius);
        lightPos = v3_create(0.0f, 0.0f, -2.0f);

        // draw cube
        glBindVertexArray(objectVAO);
        shader_use(objectShader);
        shader_setV3(objectShader, "viewPos", playerCamera.pos);

        shader_setV3(objectShader, "light.pos", playerCamera.pos);
        shader_setV3(objectShader, "light.dir", camera_getFront(playerCamera));
        shader_setFloat(objectShader, "light.cutoff", cosf(0.30f));
        shader_setFloat(objectShader, "light.outerCutoff", cosf(0.40f));

        shader_setV3(objectShader, "light.ambient", v3_mul(lightColor, 0.1f));
        shader_setV3(objectShader, "light.diffuse", v3_mul(lightColor, 0.8f)); // darken diffuse light a bit
        shader_setV3(objectShader, "light.specular", v3_mul(lightColor, 1.0f));
        shader_setFloat(objectShader, "light.constant", 1.0f);
        shader_setFloat(objectShader, "light.linear", 0.09f);
        shader_setFloat(objectShader, "light.quadratic", 0.032f);

        shader_setInt(objectShader, "material.diffuse", 0);
        shader_setInt(objectShader, "material.specular", 1);
        shader_setInt(objectShader, "material.emission", 2);
        shader_setFloat(objectShader, "material.shininess", 32.0f);

        shader_setMat4x4(objectShader, "view", view);
        shader_setMat4x4(objectShader, "projection", projection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, emissionMap);

        for (int i = 0; i < 10; ++i)
        {
            mat4x4_t objectModel = mat4x4_createIdentity();
            objectModel = mat4x4_mul(objectModel, mat4x4_createTranslate(cubePositions[i]));
            objectModel = mat4x4_mul(objectModel, mat4x4_createRotX(time));
            shader_setMat4x4(objectShader, "model", objectModel);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // draw light
        // glBindVertexArray(lightVAO);
        // shader_use(lightShader);
        // shader_setV3(lightShader, "lightColor", lightColor);
        // shader_setMat4x4(lightShader, "view", view);
        // shader_setMat4x4(lightShader, "projection", projection);
        // mat4x4_t lightModel = mat4x4_createIdentity();
        // lightModel = mat4x4_mul(lightModel, mat4x4_createTranslate(lightPos));
        // shader_setMat4x4(lightShader, "model", lightModel);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        // update
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}