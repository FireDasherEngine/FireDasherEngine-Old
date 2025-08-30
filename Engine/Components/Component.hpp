#pragma once

#include "Node/Node.hpp"
#include "ComponentRegistry.hpp"
#include <vector>
#include <string>

#define EditorField
#define RegisterComponentFields

class Node;
class ComponentRegistry;

class Component {
	public:
		Component();
		virtual void Start();
		virtual void Update();
		virtual ComponentTypeInfo GetInfo() const { return { "Unknown Component" }; }

		Node* object = nullptr;
};