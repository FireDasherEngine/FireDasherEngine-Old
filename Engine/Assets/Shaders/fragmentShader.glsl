#version 460 core

#define lightDirection vec3(0.36, 0.80, -0.48)
out vec4 FragColor;

in vec2 bTexCoord;
in vec3 bNormal;

uniform sampler2D texture1;
uniform vec3 color1;

void main() {
	float light = dot(bNormal, lightDirection) * 0.5 + 0.5;
	FragColor = texture(texture1, bTexCoord) * vec4(color1 * light, 1.0);
}