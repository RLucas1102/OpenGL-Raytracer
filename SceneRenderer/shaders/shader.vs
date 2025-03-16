#version 330 core

layout (location = 0) in vec3 aPos; // Position of vertices is at attribute location 0

void main() {

    gl_Position = vec4(aPos, 1.0f); 

}