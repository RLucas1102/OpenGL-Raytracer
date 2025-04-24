#define GLFW_INCLUDE_NONE // Ensures no header conflicts; GLFW and GLAD can interfere if swapped
#include <glad/glad.h> // Extension loader library
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ShaderLoader.h>
#include <modelLoader/model.h>

#include <raytracing/camera/camera.h>
#include <raytracing/shape/shape_list.h>
#include <raytracing/shape/shape.h>
#include <raytracing/shape/sphere.h>
#include <raytracing/vec/vec3.h>

using std::make_shared;
using std::shared_ptr;

// Callbacks
void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* MyWindow, int key, int scancode, int action, int mods); // Make local to this file
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {

    // One of the few functions that can be called before GLFW is initalized
    glfwSetErrorCallback(error_callback);

    // Need to initialize GLFW first to use any functions tha require it
    if (!glfwInit()) {
        std::cerr << "Could not initialize GLFW" << std::endl;
        return 1;
    }
    else {
        // Using OpenGL 3.3 Core
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    // Create combined GLFW window and context object
    // Context creation is dependent on correctly installed drivers
    GLFWwindow* window = glfwCreateWindow(800, 800, "Test", NULL, NULL);
    if(!window) {
        glfwTerminate();
        std::cerr << "Could not create window" << std::endl;
        return 1;
    }

    // In order to use OpenGL API, you make a context current. In this case, our window
    glfwMakeContextCurrent(window);

    // Set window callbacks and settings
    glfwSetKeyCallback(window, key_callback); // ESCAPE to close window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1); // Set swap interval to 1; by default it is 0 and will waste CPU and GPU time on fast machines

    // Access to all OpenGL core and extension functions supported by both the context we created and the glad loader we generated.
    // Load OpenGL function pointers by retrieving function address and casting to GLADloadproc type (Used by GLAD to load functions)
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Could not load GLAD" << std::endl;
        return 1;
    }

    // Create Shader Programs
    // Shader normalShader("shaders/normalShader/shader.vs", "shaders/normalShader/shader.gs", "shaders/normalShader/shader.fs");
    // Shader lightShader("shaders/lightShader/shader.vs", "shaders/lightshader/shader.gs", "shaders/lightShader/shader.fs");
    Shader testShader("shaders/testShader/shader.vs", "shaders/testShader/shader.fs");

    // Create model
    Model myModel("models/sphere.obj");

    unsigned int amount = 10;
    glm::mat4* modelMatrices;
    glm::vec3* colors;
    modelMatrices = new glm::mat4[amount];
    colors = new glm::vec3[amount];
    std::srand(time(NULL));

    for(int i = 0; i < amount; i++) {
        glm::mat4 model = glm::mat4(1.0f);

        // 1. Translate to random position
        float x =  -3 + (3 + 3) * (std::rand() / (RAND_MAX + 1.0));
        float y =  -3 + (3 + 3) * (std::rand() / (RAND_MAX + 1.0));
        model = glm::translate(model, glm::vec3(x, y, -10));

        // 2. Scale randomly
        float scale = 0.5 + (1 - 0.5) * (std::rand() / (RAND_MAX + 1.0));
        model = glm::scale(model, glm::vec3(scale));

        // 3. Add to list of matrices
        modelMatrices[i] = model;

        // 4. Create random color for object
        float r = std::rand() / (RAND_MAX + 1.0);
        float g = std::rand() / (RAND_MAX + 1.0);
        float b = std::rand() / (RAND_MAX + 1.0);
        glm::vec3 color = glm::vec3(r, g, b);

        // 5. Add to list of colors
        colors[i] = color;
        
    }

    myModel.SetInstancedDraw(amount, modelMatrices);
    myModel.SetInstanceColors(amount, colors);
    
    // Create uniform buffer for matrices in vertex shader (Both normalShader and lightShader use all three matrices)
    // Create buffer and generate ID
    unsigned int UBO;       
    glGenBuffers(1, &UBO);

    // Get Uniform block location
    // unsigned int normalBlockIdx = glGetUniformBlockIndex(normalShader.ID, "Matrices");
    // unsigned int lightBlockIdx = glGetUniformBlockIndex(lightShader.ID, "Matrices");
    unsigned int testBlockIdx = glGetUniformBlockIndex(testShader.ID, "Matrices");

    // Bind each shaders uniform block to the binding point 0
    // glUniformBlockBinding(normalShader.ID, normalBlockIdx, 0);
    // glUniformBlockBinding(lightShader.ID, lightBlockIdx, 0);
    glUniformBlockBinding(testShader.ID, testBlockIdx, 0);

    // Bind UBO and reserve space for 3 4x4 matrices in the uniform buffer object
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0); // Unbind target

    // Bind all of the uniform buffer object to binding point 0
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, 3 * sizeof(glm::mat4));

    // Depth testing
    glEnable(GL_DEPTH_TEST);
    
    // Render loop
    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents(); // Process received events
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set color to clear window with
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen with color

        // World 
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.5f));

        // Camera
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));

        // Projection
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), 800.0f/800.0f, 0.1f, 100.0f);

        // Bind uniform buffer object and send matrices to vertex shader
        glBindBuffer(GL_UNIFORM_BUFFER, UBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(model));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        // Draw vertices
        testShader.use();
        myModel.InstancedDraw(amount);

        glfwSwapBuffers(window); // Swap front and back buffer

    }
    
    // Clean up and shut down
    glDeleteBuffers(1, &UBO);
    glfwTerminate(); // Release all GLFW resources and close windows

    return 0;
}

/*
 * In case a GLFW function fails, an error is reported 
 * to the GLFW error callback. You can receive these 
 * reports with an error callback.
 *
 */
void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

/* 
 * Receive key press when ESCAPE key is pressed
 */
void key_callback(GLFWwindow *MyWindow, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(MyWindow, true);
    }

    if(key == GLFW_KEY_A && action == GLFW_PRESS) {
        //myCamera.render(world);
    }

}

/*
 * Called when framebuffer size changes; Is necessary for any window
 */
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, 800, 800);
}

