#pragma once

#include <GL/glew.h>
#include <vector>
#include <string>
#include "Misc/Vector3.hpp"
#include "Renderer/Shader.hpp"

typedef unsigned int uint;

class Mesh {
	public:
		Mesh() {};
		Mesh(const std::vector<Vector3>& vertices, const std::vector<Vector2>& uv, const std::vector<Vector3>& normals, const std::vector<uint>& triangles);
		Mesh(const char* path);
		~Mesh();

		void Use() const;

		std::vector<Vector3> vertices;
		std::vector<Vector2> uv;
		std::vector<Vector3> normals;
		std::vector<uint> triangles;

	private:
		void init();

		GLuint VAO = 0;
		GLuint VBOs[3] = { 0, 0, 0 };
		GLuint EBO = 0;
};