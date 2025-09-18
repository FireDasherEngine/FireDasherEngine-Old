#pragma once

#include "Component.hpp"
#include "Mesh/Mesh.hpp"
#include "Mesh/Material.hpp"

RegisterComponentFields class MeshRenderer : public Component {
	public:
		MeshRenderer() {};
		MeshRenderer(Mesh* mesh, Material* material);
		~MeshRenderer();
		Mesh* mesh = nullptr;
		Material* material = nullptr;

		void Render(Shader* shader);

		EditorField std::string meshPath;
		EditorField std::string materialPath;

		ComponentTypeInfo GetInfo()const override { return{ "Mesh Renderer",{{"Mesh Path",InputType::String,offsetof(MeshRenderer,meshPath)},{"Material Path",InputType::String,offsetof(MeshRenderer,materialPath)},} }; }
};