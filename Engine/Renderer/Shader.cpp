#include "Shader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
	GLuint vertexShader = LoadShader(vertexPath, GL_VERTEX_SHADER);
	GLuint fragmentShader = LoadShader(fragmentPath, GL_FRAGMENT_SHADER);
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	// Check for linking errors
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cerr << "Shader program linking failed:\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader() {
	glDeleteProgram(program);
	program = 0;
}

void Shader::Use() const {
	glUseProgram(program);
}

GLuint Shader::LoadShader(const std::string& path, GLenum type) {
	std::ifstream file(path);
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	const std::string shaderSource = buffer.str();

	if (shaderSource.empty()) {
		std::cerr << "Shader loading " << path << " is empty!\n" << std::endl;
	}

	const GLchar* c_shaderSource = shaderSource.c_str();
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &c_shaderSource, NULL);
	glCompileShader(shader);

	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cerr << "Shader loading " << path << " failed:\n" << infoLog << std::endl;
	}

	return shader;
}

void Shader::SetInt(const char* name, GLint value) const {
	glUniform1i(glGetUniformLocation(program, name), value);
}

void Shader::SetFloat(const char* name, GLfloat value) const {
	glUniform1f(glGetUniformLocation(program, name), value);
}

void Shader::SetMat4(const char* name, glm::mat4 value) const {
	glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetVec3(const char* name, glm::vec3 value) const {
	glUniform3fv(glGetUniformLocation(program, name), 1, glm::value_ptr(value));
}

void Shader::SetVec4(const char* name, glm::vec4 value) const {
	glUniform4fv(glGetUniformLocation(program, name), 1, glm::value_ptr(value));
}