#pragma once

#include "Node.hpp"
//#include "Components/Camera.hpp"
#include <string>
#include <vector>

#include <json.hpp>

class Node;
class Camera;

using json = nlohmann::ordered_json;

class Scene {
	public:
		Node* rootNode;
		Camera* mainCamera;
		std::vector<Node*> allNodes;

		void CalculateAllNodes();
		void SaveScene(const std::string& path);
		void LoadScene(const std::string& path);

	private:
		json SaveNode(Node* node);
		Node* LoadNode(const json& nodeJson);

		json sj;
};