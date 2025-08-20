#pragma once

#include "Misc/Vector3.hpp"
#include "Texture.hpp"

class Material {
	public:
		Material() : color(1.0f), texture("../Engine/Assets/DefaultTexture.png") {}
		Material(const Vector3& color) : color(color), texture("../Engine/Assets/DefaultTexture.png") {}
		Material(const char* texturePath) : color(1.0f), texture(texturePath) {}
		Material(const Vector3& color, const char* texturePath) : color(color), texture(texturePath) {}

		Vector3 color;
		Texture texture;
};