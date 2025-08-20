#pragma once

#include "Shader.hpp"
#include "Node/Scene.hpp"

class Renderer3D {
	public:
		Renderer3D();
		~Renderer3D();
		void Render();

		Scene* scene = nullptr;
		Camera* overrideCamera = nullptr;
	private:
		Shader* shader = nullptr;
};