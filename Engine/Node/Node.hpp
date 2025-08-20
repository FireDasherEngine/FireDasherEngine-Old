#pragma once

#include "Transform.hpp"
#include "Components/Component.hpp"
#include <string>
#include <map>
#include <vector>

class Component;

class Node {
	public:
		// Creates an empty object named "NewObject".
		Node();
		// Creates an empty object with a name.
		Node(const std::string& name);
		// Creates an empty object with a name then parents it.
		Node(const std::string& name, Node* parent);

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
}; 