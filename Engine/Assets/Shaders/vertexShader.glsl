#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

out vec2 bTexCoord;
out vec3 bNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	bTexCoord = aTexCoord;
	bNormal = normalize((model * vec4(aNormal, 0.0)).xyz);
}