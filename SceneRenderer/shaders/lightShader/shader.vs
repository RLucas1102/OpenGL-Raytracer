#version 330 core

layout (location = 0) in vec3 aPos; // Position of vertices is at attribute location 0

layout (std140) uniform matrices {

    mat4 model;                    
    mat4 view;
    mat4 projection;
    
};

void main() {

    gl_Position = projection * view * model * vec4(aPos, 1.0f);

}