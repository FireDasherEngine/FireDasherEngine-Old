#pragma once

#include "Node.hpp"
#include "Components/Camera.hpp"

class Scene {
	public:
		Node* RootNode;
		Camera* MainCamera;
		std::vector<Node*> AllNodes;

		void CalculateAllNodes();
		void SaveScene(const std::string& path);
		void LoadScene(const std::string& path);

	private:
		void SaveNode(std::ofstream& file, Node* node);
		Node* LoadNode(std::ifstream& file, Node* parent);
};