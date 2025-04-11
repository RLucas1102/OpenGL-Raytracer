#define GLFW_INCLUDE_NONE // Ensures no header conflicts; GLFW and GLAD can interfere if swapped
#include <glad/glad.h> // Extension loader library
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <ShaderLoader.h>
#include <modelLoader/model.h>

// Callbacks
void error_callback(int error, const char* description);
static void esc_callback(GLFWwindow* MyWindow, int key, int scancode, int action, int mods); // Make local to this file
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

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // IF using Docking Branch (I'm not)

    // Setup Platform/Renderer backends (MAKE SURE YOU SET THE WINDOW AS THE CURRENT CONTEXT FIRST)
    ImGui_ImplGlfw_InitForOpenGL(window, true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    // Set window callbacks and settings
    glfwSetKeyCallback(window, esc_callback); // ESCAPE to close window
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
    Shader lightShader("shaders/lightShader/shader.vs", "shaders/lightShader/shader.gs","shaders/lightShader/shader.fs");

    // Create model
    Model myModel("models/sphere.obj");

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

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow(); // Show demo window! :)

        // World 
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 0.0f));

        // Camera
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

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

        myModel.Draw();

        // normalShader.use();
        // myModel.Draw();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window); // Swap front and back buffer

    }
    
    // Clean up and shut down
    glDeleteBuffers(1, &UBO);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
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
void esc_callback(GLFWwindow *MyWindow, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(MyWindow, true);
    }
}

/*
 * Called when framebuffer size changes; Is necessary for any window
 */
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, 800, 800);
}

