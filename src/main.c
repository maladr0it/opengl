#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <glad/glad.h>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "utils.h"
#include "shader.h"
#include "mat4x4.h"
#include "v3.h"
#include "v4.h"

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;
static const float FOV = M_PI_2;
static const float Z_NEAR = 0.1f;
static const float Z_FAR = 100.0f;
static const double MOUSE_SENSITIVITY = 0.002f;
static const float CAMERA_SPEED = 5.0f;

static double lastMouseX = (float)WINDOW_WIDTH / 2.0f;
static double lastMouseY = (float)WINDOW_HEIGHT / 2.0f;

static v3_t cameraPos;
static float cameraYaw;
static float cameraPitch;
static float dt;

v3_t getCameraFront(float yaw, float pitch)
{
    v3_t result;
    result.x = cosf(yaw) * cosf(pitch);
    result.y = sinf(pitch);
    result.z = sinf(yaw) * cosf(pitch);
    return v3_normalize(result);
}

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

    v3_t cameraFront = getCameraFront(cameraYaw, cameraPitch);
    v3_t cameraUp = v3_create(0.0f, 1.0f, 0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPos = v3_add(cameraPos, v3_mul(cameraFront, dt * CAMERA_SPEED));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPos = v3_sub(cameraPos, v3_mul(cameraFront, dt * CAMERA_SPEED));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPos = v3_sub(cameraPos, v3_mul(v3_normalize(v3_cross(cameraFront, cameraUp)), dt * CAMERA_SPEED));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPos = v3_add(cameraPos, v3_mul(v3_normalize(v3_cross(cameraFront, cameraUp)), dt * CAMERA_SPEED));
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        cameraYaw += 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        cameraYaw -= 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        cameraPitch += 0.1;
    }
}

void handleMouseMove(GLFWwindow *window, double xPos, double yPos)
{
    double mouseSensitivity = 0.002f;

    double dx = (xPos - lastMouseX) * mouseSensitivity;
    double dy = -(yPos - lastMouseY) * mouseSensitivity; // negative as down is +ve for mouse coords
    lastMouseX = xPos;
    lastMouseY = yPos;

    cameraYaw += dx;
    cameraPitch = clampf(cameraPitch + dy, -M_PI_2 + 0.001f, M_PI_2 - 0.001f);
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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
        exit(EXIT_FAILURE);
    };

    //
    // Create shader program
    //
    shader_t shader = shader_create(
        "./src/shaders/vert.vs",
        "./src/shaders/frag.fs");

    //
    // Create data and buffers
    //
    // clang-format off
    float verts[] = {
        // pos                // tex coord
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,     
    };
    // clang-format on

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // tex coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glad_glEnableVertexAttribArray(1);

    //
    // Create textures
    //
    stbi_set_flip_vertically_on_load(true);
    int width, height, numChannels;
    unsigned char *imageData;

    // texture 1
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    imageData = stbi_load("./assets/container.jpg", &width, &height, &numChannels, 0);
    if (imageData == NULL)
    {
        printf("Failed to load texture");
        exit(EXIT_FAILURE);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    stbi_image_free(imageData);

    // texture 2
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    imageData = stbi_load("./assets/awesomeface.png", &width, &height, &numChannels, 0);
    if (imageData == NULL)
    {
        printf("Failed to load texture");
        exit(EXIT_FAILURE);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    stbi_image_free(imageData);

    shader_use(shader);
    shader_setInt(shader, "texture1", 0);
    shader_setInt(shader, "texture2", 1);

    glEnable(GL_DEPTH_TEST);

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

    // intialize globals
    cameraPos = v3_create(0.0f, 0.0f, 3.0f);
    cameraYaw = -M_PI_2;
    cameraPitch = 0;

    float lastFrame = 0.0f;
    //
    // Update loop
    //
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = (float)glfwGetTime();
        dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // inputs
        processInput(window);

        //
        // create transform matrix
        //
        v3_t cameraFront = getCameraFront(cameraYaw, cameraPitch);
        v3_t cameraUp = v3_create(0.0f, 1.0f, 0.0f);

        mat4x4_t view = mat4x4_createIdentity();
        view = mat4x4_mul(view, mat4x4_createLookAt(cameraPos, v3_add(cameraPos, cameraFront), cameraUp));
        shader_setMat4x4(shader, "view", view);

        mat4x4_t projection = mat4x4_createProj((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, FOV, Z_NEAR, Z_FAR);
        shader_setMat4x4(shader, "projection", projection);

        // render
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_use(shader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO);

        for (int i = 0; i < 10; ++i)
        {
            mat4x4_t model = mat4x4_createIdentity();
            model = mat4x4_mul(model, mat4x4_createTranslate(cubePositions[i]));
            model = mat4x4_mul(model, mat4x4_createRotX((float)glfwGetTime()));
            shader_setMat4x4(shader, "model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // update
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}