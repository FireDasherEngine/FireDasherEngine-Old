#pragma once

#include "Component.hpp"
#include "Mesh/Mesh.hpp"
#include "Mesh/Material.hpp"

RegisterComponent class MeshRenderer : public Component {
	public:
		MeshRenderer(Mesh* mesh, Material* material);
		~MeshRenderer();
		Mesh* mesh = nullptr;
		Material* material = nullptr;

		void Render(Shader* shader);

		EditorField std::string MeshPath;
		EditorField std::string MaterialPath;

		//ComponentTypeInfo GetInfo() const override;
		ComponentTypeInfo GetInfo()const override { return{ "Mesh Renderer",{{"Mesh Path",InputType::String,offsetof(MeshRenderer,MeshPath)},{"Material Path",InputType::String,offsetof(MeshRenderer,MaterialPath)},} }; }
};