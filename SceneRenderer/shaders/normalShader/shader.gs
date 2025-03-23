#version 330 core

layout (triangles) in; // Take in primitives as triangles
layout (line_strip, max_vertices = 6) out; // Output primitives as lines using line_strip

out vec4 fColor;

void main() {

    vec3 a = vec3(gl_in[1].gl_Position) - vec3(gl_in[0].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[0].gl_Position);
    vec3 normal = normalize(cross(b,a));

    float magnitude = 0.5f;
    for(int i = 0; i < 3; i++) {

        if(i == 0) {
            fColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
        }
        else if(i == 1) {
            fColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
        }
        else {
            fColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
        }

        gl_Position = gl_in[i].gl_Position;
        EmitVertex();

        gl_Position = gl_in[i].gl_Position + vec4(normal, 0.0f) * magnitude;
        EmitVertex();

        EndPrimitive();
    }
}