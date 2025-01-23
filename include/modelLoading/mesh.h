#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // Holds all OpenGL type declarations

#include <glm/glm.hpp> // Used for matrices and transformations 
#include <glm/gtc/matrix_transform.hpp>

#include <shader/shaders.h> // User-defined shader program for shader loading

#include <string>
#include <vector>

using namespace std;

// Each vertex will be contained within a struct
// The memory layout of structs is sequential like an array
// The above fact will be become useful when loading vertices to GPU
struct Vertex {

	glm::vec3 position;

	glm::vec3 normal;

	glm::vec2 texture;

};

class Mesh {

	public:
		// Mesh data
		vector<Vertex> vertices;
		vector<unsigned int> indices;

		// Constructor
		// For each model, load every mesh by its vertices and indices input
		Mesh(vector<Vertex> vertices, vector<unsigned int> indices) {

			this->vertices = vertices;
			this->indices = indices;

			setupMesh();
		}

		// Draws the shape onto our screen
		void Draw() {
			// Draw Mesh
        	glBindVertexArray(VAO);
        	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        	glBindVertexArray(0);

		}

		// Called to delete buffers
		void DeleteBuffers() {
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);
			glDeleteVertexArrays(1, &VAO);
		}

	private:
		// Render data
		unsigned int VAO, VBO, EBO; // Declare a vertex buffer, element buffer, and vertex array object

		void setupMesh() {

			// Bind buffers and load vertex/index data into buffers
			// ----------------------------------------------------
			glGenBuffers(1, &VBO); // Gen IDs for each buffer and the array
			glGenBuffers(1, &EBO);
			glGenVertexArrays(1, &VAO);

			glBindVertexArray(VAO); // Bind the vertex array we want to use first

			glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind VBO as an array buffer to store vertices
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW); // Load vertices array we defined into buffer

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Bind EBO as an element array buffer to store indices
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW); // Load indices array we defined into element buffer

			// Position location
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			glEnableVertexAttribArray(0);

			// Normal location
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
			glEnableVertexAttribArray(1);

			// Texture location
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));
			glEnableVertexAttribArray(2);

			// Unbind vertex array
			glBindVertexArray(0);

		}
};

#endif
