#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Components/Camera.hpp"
#include "Shader.hpp"
#include "Renderer3D.hpp"

class Viewport {
	public:
		Viewport();
		~Viewport();
		void ResizeFB(GLsizei width, GLsizei height);
		void Render() const;
		GLuint texture;
		Renderer3D* renderer = nullptr;

	private:
		GLsizei width = 800, height = 600;
		GLuint FBO = 0, RenderBuffer = 0;
};