#include "MeshRenderer.hpp"

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material) : mesh(mesh), material(material) {}
MeshRenderer::~MeshRenderer() { mesh->~Mesh(); }
void MeshRenderer::Render(Shader* shader) {
	shader->SetMat4("model", object->worldMatrix());
	shader->SetVec3("color1", material->color.glm());

	material->texture.Use();

	mesh->Draw();
}
RegisterComponent(MeshRenderer);