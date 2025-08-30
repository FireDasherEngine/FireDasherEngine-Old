#pragma once

#include "Transform.hpp"
#include "Components/Component.hpp"
#include "Scene.hpp"
#include <string>
#include <map>
#include <vector>

class Component;
class Scene;

class Node {
	public:
		// Creates an empty object named "NewObject" without a scene.
		Node();
		// Creates an empty object named "NewObject" with a scene.
		Node(Scene* scene);
		// Creates an empty object with a name without a scene.
		Node(const std::string& name);
		// Creates an empty object with a name with a scene.
		Node(const std::string& name, Scene* scene);
		// Creates an empty object with a name then parents it, and sets it's scene to the parent's scene.
		Node(const std::string& name, Node* parent);
		// Creates an empty object named "NewObject" then parents it, and sets it's scene to the parent's scene.
		Node(Node* parent);

		// Deletes the node and all its children.
		void Destroy();

		// Adds as component.
		void AddComponent(Component* component);

		// Gets a component by class type.
		template<typename T> T* GetComponent() {
			for (Component* component : components) {
				if (T* tcomponent = dynamic_cast<T*>(component); tcomponent) {
					return tcomponent;
				}
			}
			return nullptr;
		};

		// Adds node to children, removing node from old parent if nescessary.
		void AddChild(Node* child);
		// Removes node from children.
		void RemoveChild(Node* child);
		// Gets all children recusively.
		std::vector<Node*> GetDescendants();
		// Gets all children recusively.
		void GetDescendants(std::vector<Node*>& result);
		// Converts the local transform to a world 4x4 Matrix.
		glm::mat4x4 worldMatrix();

		std::string name;
		Transform transform;
		std::vector<Component*> components;
		std::vector<Node*> children;
		Node* parent = nullptr;
		Scene* scene = nullptr;
}; 