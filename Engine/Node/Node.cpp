#include "Node.hpp"

Node::Node() : transform((0.0f), (0.0f), (1.0f)), name("NewObject") {}
Node::Node(const std::string& name) : transform((0.0f), (0.0f), (1.0f)), name(name) {}
Node::Node(const std::string& name, Node* parent) : transform((0.0f), (0.0f), (1.0f)), name(name) {
	parent->AddChild(this);
}

void Node::Destroy() {
	parent->RemoveChild(this);
	if (!children.empty())
		for (Node* child : GetDescendants()) {
			delete child; // Manual memory manangement with raw pointers is very important!!!!
		}
	delete this;
}

void Node::AddComponent(Component* component) {
	components.push_back(component);
	component->object = this;
}
/*template<typename T> T* Node::GetComponent() {
	for (Component* component : components) {
		if (T* tcomponent = dynamic_cast<T*>(component); tcomponent) {
			return tcomponent;
		}
	}
	return nullptr;
}*/
void Node::AddChild(Node* child) {
	if (child->parent) {
		child->parent->RemoveChild(child);
	}
	children.push_back(child);
	child->parent = this;
}
void Node::RemoveChild(Node* child) {
	children.erase(std::find(children.begin(), children.end(), child));
}

std::vector<Node*> Node::GetDescendants() {
	std::vector<Node*> result;
	GetDescendants(result);
	return result;
}

void Node::GetDescendants(std::vector<Node*>& result) {
	for (Node* child : children) {
		result.push_back(child);
		child->GetDescendants(result);
	}
}

glm::mat4x4 Node::worldMatrix() {
	if (parent) {
		return parent->worldMatrix() * transform.mat4();
	} else {
		return transform.mat4();
	}
}