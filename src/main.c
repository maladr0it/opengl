#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <glad/glad.h>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "shader.h"
#include "mat4x4.h"
#include "v3.h"
#include "v4.h"

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;
static float mixVal = 1.0f;

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
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixVal -= 0.01f;
        if (mixVal < 0)
        {
            mixVal = 0;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixVal += 0.01f;
        if (mixVal > 1)
        {
            mixVal = 1;
        }
    }
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
    glfwSetFramebufferSizeCallback(window, handleResize);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
        exit(EXIT_FAILURE);
    };

    //
    // Create shader program
    //
    shader_t shader = shader_create(
        "./src/shaders/textured/vert.vs",
        "./src/shaders/textured/frag.fs");

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

    //
    // Create data and buffers
    //
    // clang-format off
    float verts[] = {
        // pos                     // color                // tex coords
        -0.50f, +0.50f, +0.00f,    1.00f, 0.00f, 0.00f,    0.00f, 1.00f,    // top-left
        -0.50f, -0.50f, +0.00f,    0.00f, 1.00f, 0.00f,    0.00f, 0.00f,    // bottom-left
        +0.50f, +0.50f, +0.00f,    0.00f, 0.00f, 1.00f,    1.00f, 1.00f,    // top-right
        +0.50f, -0.50f, +0.00f,    1.00f, 1.00f, 0.00f,    1.00f, 0.00f,    // bottom-right
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 1, 3, 
    };
    // clang-format on

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glad_glEnableVertexAttribArray(1);
    // tex coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glad_glEnableVertexAttribArray(2);

    shader_use(shader);
    shader_setInt(shader, "texture1", 0);
    shader_setInt(shader, "texture2", 1);
    //
    // Update loop
    //
    while (!glfwWindowShouldClose(window))
    {
        // inputs
        processInput(window);
        shader_setFloat(shader, "mixVal", mixVal);

        // render
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_use(shader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // update
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}