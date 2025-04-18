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

// Camera/world setup
camera myCamera;
shape_list world;

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
    Shader normalShader("shaders/normalShader/shader.vs", "shaders/normalShader/shader.gs", "shaders/normalShader/shader.fs");
    Shader lightShader("shaders/lightShader/shader.vs", "shaders/lightShader/shader.gs","shaders/lightShader/shader2.fs");

    // Create model
    Model myModel("models/sphere.obj");

    // Camera setup
    myCamera.setAspect(1.0/1.0);
    myCamera.setImgWidth(800);

    // World setup
    // Query user for number of spheres
    int numSpheres;
    numSpheres = 1;
     std::cout << "How many spheres would you like to render?" << std::endl;
     std::cout << "Number of spheres: ";
     std::cin >> numSpheres;
     std::string line;
     std::string component;

    // Setup instanced array for instanced drawing
    glm::vec3 position;
    glm::vec3* positions;
    positions = new glm::vec3[numSpheres];

    std::cout << "For each sphere, enter an x, y, and z to set its position in space" << std::endl;
    std::cout << "Enter each position in the following format - x, y, z" << std::endl;
    for (int i = 0; i < numSpheres; i++) {
        std::cin.ignore(1000, '\n');
        std::cout << "Sphere " << i << ": ";
        std::getline(std::cin, line);
        std::stringstream ss (line);
        std::getline(ss, component, ',');
        position.x = std::stoi(component);
        std::getline(ss, component, ',');
        position.y = std::stoi(component);
        std::getline(ss, component, ',');
        position.z = std::stoi(component);
        positions[i] = position;
    }

    for (int i = 0; i < numSpheres; i++) {
        world.add(make_shared<sphere>(vec3(positions[i].x, 
                                           positions[i].y, 
                                           positions[i].z), 0.5));
    }

    myModel.SetInstancedDraw(numSpheres, positions);
    
    // Create uniform buffer for matrices in vertex shader (Both normalShader and lightShader use all three matrices)
    // Create buffer and generate ID
    unsigned int UBO;       
    glGenBuffers(1, &UBO);

    // Get Uniform block location
    unsigned int normalBlockIdx = glGetUniformBlockIndex(normalShader.ID, "Matrices");
    unsigned int lightBlockIdx = glGetUniformBlockIndex(lightShader.ID, "Matrices");

    // Bind each shaders uniform block to the binding point 0
    glUniformBlockBinding(normalShader.ID, normalBlockIdx, 0);
    glUniformBlockBinding(lightShader.ID, lightBlockIdx, 0);

    // Bind UBO and reserve space for 3 4x4 matrices in the uniform buffer object
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0); // Unbind target

    // Bind all of the uniform buffer object to binding point 0
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, 3 * sizeof(glm::mat4));

    // Depth testing
    glEnable(GL_DEPTH_TEST);

    // Light setup
    glm::vec3 lightPos = glm::vec3(1.0f, 1.0f, 2.0f);
    
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
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));

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
        lightShader.use();
        
        // Pass the light's position
        int lightPosLoc = glGetUniformLocation(lightShader.ID, "vsLightPos");
        glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

        myModel.InstancedDraw(numSpheres);

        // normalShader.use();
        // myModel.Draw();

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
        myCamera.render(world);
    }

}

/*
 * Called when framebuffer size changes; Is necessary for any window
 */
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, 800, 800);
}

