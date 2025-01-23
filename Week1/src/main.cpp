// "OpenGL" libraries
#include <glad/glad.h> // OpenGL library; Include GLAD before GLFW
#include <GLFW/glfw3.h> // Window/IO

// Matrix libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shader library
#include <shader/shaders.h>

// Standard libraries
#include <iostream>

// Globals
const int windowWidth = 1000;
const int windowHeight = 1000;

// Function Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {

    // GLFW Initialization
    // -------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Using OpenGL 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Necessary for MAC users

    // Create a window
    // ---------------
    GLFWwindow* myWindow = glfwCreateWindow(windowWidth, windowHeight, "Depth Testing", NULL, NULL);
    if(myWindow == NULL) {

        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;

    }

    // Window Configuration
    // --------------------
    glfwMakeContextCurrent(myWindow); // Makes context of window current on calling thread
    glfwSetFramebufferSizeCallback(myWindow, framebuffer_size_callback); // When the window changes size, this is called

    // GLAD
    // ----
    // Load the address of OpenGL function pointers; this is OS specific thus glfwGetProcAddress is used
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {

        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;

    }

    // Create shader program
	// ---------------------
	Shader ourShader = Shader("files/shader.vs", "files/shader.fs");

    // Vertex/Index data
	// -----------------
	float cubeVertices[] = {
		// Vertices			// Colors
		0.5f,  0.5f,  0.5f,	 1.0f, 0.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
	    0.5f, -0.5f,  0.5f,	 0.0f, 0.0f, 1.0f,
	    0.5f, -0.5f, -0.5f,	 1.0f, 1.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f,	 1.0f, 0.0f, 1.0f,
	   -0.5f,  0.5f, -0.5f,	 0.0f, 1.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f,	 0.5f, 0.5f, 1.0f,
	   -0.5f, -0.5f, -0.5f,	 1.0f, 0.5f, 0.5f
	};

	unsigned int cubeIndices[] = {
		7, 3, 1,
		1, 5, 7,

		6, 2, 0,
		0, 4, 6,

		4, 5, 7,
		7, 6, 4,

		0, 1, 3,
		3, 2, 0,

		7, 3, 2,
		2, 6, 7,

		5, 1, 0,
		0, 4, 5

	};

    float planeVertices[] = {
        // Vertices             // Colors
        -0.5f, -0.5f, 0.0f,     0.5f, 0.5f, 0.5f,
         0.5f, -0.5f, 0.0f,     0.5f, 0.5f, 0.5f,
         0.5f,  0.5f, 0.0f,     0.5f, 0.5f, 0.5f,
        -0.5f,  0.5f, 0.0f,     0.5f, 0.5f, 0.5f 
    };

    unsigned int planeIndices[] = {
        0,1,2,
        2,3,0
    }; 

	// Bind and load buffers with vertex/index data for cube
	// -----------------------------------------------------
	unsigned int cubeVBO, cubeVAO, cubeEBO; // Declare buffers
	glGenBuffers(1, &cubeVBO);		// Generate IDs for buffers
	glGenBuffers(1, &cubeEBO);
	glGenVertexArrays(1, &cubeVAO);

	glBindVertexArray(cubeVAO); // Bind the vertex array first

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO); // Bind as an array buffer for vertices
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW); // Load vertex data into VBO

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO); // Bind buffers as element array buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW); // Load index data in EBO

	// Position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // Locate the vertex position attribute on GPU
	glEnableVertexAttribArray(0);

	// Color data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // Locate the color attribute on GPU
	glEnableVertexAttribArray(1);

    // Bind and load buffers with vertex/index data for plane
    // ------------------------------------------------------
    unsigned int planeVBO, planeVAO, planeEBO; // Declare buffers
    glGenBuffers(1, &planeVBO); // Generate IDs for buffers
    glGenBuffers(1, &planeEBO);
    glGenVertexArrays(1, &planeVAO);

    glBindVertexArray(planeVAO); // Bind vertex array first

    glBindBuffer(GL_ARRAY_BUFFER, planeVBO); // Bind as array buffer for vertices
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW); // Load in vertex data into VBO

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO); // Bind as element array buffer for indices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(planeIndices), planeIndices, GL_STATIC_DRAW); // Load in index data into EBO

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // Locate position attribute on GPU
    glEnableVertexAttribArray(0); // Enable position

    // Color 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // Locate color attribute on GPU
    glEnableVertexAttribArray(1); // Enable Color

    // Camera settings
    // ---------------
    glm::vec3 cameraPos = glm::vec3(0.5f, 1.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, -0.2f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // Object settings
    // ---------------
    glm::vec3 cube1 = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cube2 = glm::vec3(1.0f, 0.0f, -5.0f);
    glm::vec3 plane = glm::vec3(0.0f, -0.5f, -2.0f);

    // Depth settings (Configuring Global OpenGL state)
    // ------------------------------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE); // Depth buffer enabled
    glDepthFunc(GL_LESS); // Appears as if we never enabled depth testing;

    // Render Loop
    // -----------
    while(!glfwWindowShouldClose(myWindow)) {
        
        // Rendering commands
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // state-setting
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // state-using

        ourShader.use(); // Activate our shader

        glBindVertexArray(cubeVAO);

        // Projection matrix
	    // Our projection rarely changes, thus it can be kept outside the main render loop and set only once
	    glm::mat4 projection; // Create a perspective matrix based on input parameters
	    projection = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 2.0f, 100.0f);

        // View matrix
        glm::mat4 view;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        // Model matrix (cube1)
		glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cube1);
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        // Pass transformations
        ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);
        ourShader.setMat4("model", model);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // Draw the 36 vertices (1 cube, 6 faces, 12 triangles)

        // Model matrix (cube2)
        model = glm::mat4(1.0f);
        model = glm::translate(model, cube2);
        model = glm::rotate(model, glm::radians(75.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        // Pass transformations
        ourShader.setMat4("model", model);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // Draw the 36 vertices (1 cube, 6 faces, 12 triangles)

        glBindVertexArray(planeVAO);

        // Model matrix (plane)
        model = model = glm::mat4(1.0f);
        model = glm::translate(model, plane);
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(10.0f));

        // Pass transformations
        ourShader.setMat4("model", model);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // Draw the 36 vertices (1 cube, 6 faces, 12 triangles)

        // Swap and do events
        glfwSwapBuffers(myWindow); // Swap front and back buffer
        glfwPollEvents(); // Checks if any events were triggered and calls functions

    }

    // Clean up
    // --------
    glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &cubeVAO);
	glDeleteBuffers(1, &cubeEBO);
    glDeleteBuffers(1, &planeVBO);
	glDeleteBuffers(1, &planeVAO);
	glDeleteBuffers(1, &planeEBO);
    glfwTerminate();

    return 0;
}

// Functions Definitions
// ---------------------

// Callback for when the window changes size
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, windowWidth, windowHeight);
}
