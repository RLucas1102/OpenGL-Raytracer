#version 330 core

out vec4 FragColor; // Single output variable is the fragment color

void main() {

    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);  // Color of our light
    vec3 objectColor = vec3(1.0f, 1.0f, 0.0f); // Color of the object

    float illumination = 1.0f;
    vec3 ambient = illumination * lightColor;

    vec3 result = (ambient) * objectColor;
    FragColor = vec4(result, 1.0f);
}