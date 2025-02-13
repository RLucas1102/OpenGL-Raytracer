#version 330 core

in vec3 ourColor;

out vec4 fragColor;

void main() {

	float depth = gl_FragCoord.z;
	fragColor = vec4(vec3(depth), 1.0f);

}