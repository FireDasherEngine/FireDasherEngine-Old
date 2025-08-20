#pragma once

#include <GL/glew.h>
#include <string>

class Texture {
	public:
		Texture() {}
		Texture(const char* path);
		~Texture();
		void Use() const;

		GLuint texture = 0;
};