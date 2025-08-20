#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader {
	public:
		Shader() {}
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		~Shader();

		void Use() const;

		void SetInt  (const char* name, GLint value) const;
		void SetFloat(const char* name, GLfloat value) const;
		void SetMat4 (const char* name, glm::mat4 value) const;
		void SetVec3 (const char* name, glm::vec3 value) const;
		void SetVec4 (const char* name, glm::vec4 value) const;

		GLuint program = 0;

	private:
		GLuint LoadShader(const std::string& fileName, GLenum type);
};