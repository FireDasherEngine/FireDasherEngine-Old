#include "MeshRenderer.hpp"

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material) : mesh(mesh), material(material) {}
MeshRenderer::~MeshRenderer() { mesh->~Mesh(); }
/*ComponentTypeInfo MeshRenderer::GetInfo() const {
	return
		{ "Mesh Renderer",
			{
				{"Mesh File Path", InputType::String, offsetof(MeshRenderer, MeshPath)},
				{"Material File Path", InputType::String, offsetof(MeshRenderer, MaterialPath)},
			}
		};
}*/
void MeshRenderer::Render(Shader* shader) {
	shader->SetMat4("model", object->worldMatrix());
	shader->SetVec3("color1", material->color.glm());

	material->texture.Use();

	mesh->Use();
	glDrawElements(GL_TRIANGLES, (GLsizei)mesh->triangles.size(), GL_UNSIGNED_INT, (void*)0);
}
RegisterComponent(MeshRenderer);