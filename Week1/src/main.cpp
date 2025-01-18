#include <glad/glad.h> // Include GLAD before GLFW
#include <GLFW/glfw3.h>
#include <iostream>

const int windowWidth = 1000;
const int windowHeight = 1000;

// Function Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {


    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Using OpenGL 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Necessary for MAC users

    // Create a window
    GLFWwindow* myWindow = glfwCreateWindow(windowWidth, windowHeight, "Depth Testing", NULL, NULL);
    if(myWindow == NULL) {

        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;

    }

    glfwMakeContextCurrent(myWindow); // Makes context of window current on calling thread
    glfwSetFramebufferSizeCallback(myWindow, framebuffer_size_callback); // When the window changes size, this is called

    // Load the address of OpenGL function pointers; this is OS specific thus glfwGetProcAddress is used
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {

        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;

    }

    // Render Loop
    while(!glfwWindowShouldClose(myWindow)) {
        
        // Rendering commands
        glClearColor(0.0f, 1.0f, 0.4f, 1.0f); // state-setting
        glClear(GL_COLOR_BUFFER_BIT); // state-using

        // Swap and do events
        glfwSwapBuffers(myWindow); // Swap front and back buffer
        glfwPollEvents(); // Checks if any events were triggered and calls functions

    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, windowWidth, windowHeight);
}
