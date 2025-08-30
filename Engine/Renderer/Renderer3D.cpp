#include "Renderer3D.hpp"

#include "Components/Camera.hpp"

#include "Components/MeshRenderer.hpp"

Renderer3D::Renderer3D() {
	shader = new Shader("../Engine/Assets/Shaders/vertexShader.glsl", "../Engine/Assets/Shaders/fragmentShader.glsl");
}
Renderer3D::~Renderer3D() {
	shader->~Shader();
	shader = nullptr;
}
void Renderer3D::Render() {
	Camera*& camera = overrideCamera ? overrideCamera : scene->MainCamera;

	glViewport(0, 0, camera->viewportWidth, camera->viewportHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->Use();

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::inverse(camera->object->transform.mat4());

	glm::mat4 projection;
	projection = glm::perspectiveLH(glm::radians(camera->fov), (float)camera->viewportWidth / (float)camera->viewportHeight, camera->nearPlane, camera->farPlane); // The "LH" here means +Z is forward

	shader->SetMat4("view", view);
	shader->SetMat4("projection", projection);
	
	for (Node* object : scene->AllNodes) {
		if (MeshRenderer* meshRenderer = object->GetComponent<MeshRenderer>(); meshRenderer) {
			meshRenderer->Render(shader);
		}
	}
}