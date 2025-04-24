#version 330 core

layout (location = 0) in vec3 aPos; // Position of vertices is at attribute location 0
layout (location = 1) in mat4 instanceMatrix; // Offset matrices for each object

layout (std140) uniform matrices {

    mat4 model;                    
    mat4 view;
    mat4 projection;
    
};

void main() {
    gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0f);
}