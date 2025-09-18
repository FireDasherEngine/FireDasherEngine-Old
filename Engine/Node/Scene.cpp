#include "Scene.hpp"
#include <fstream>
#include <algorithm>

#include "Components/Camera.hpp"

/*
// write whatever the cast turns it into
#define w(data) file.write(reinterpret_cast<const char*>(&data), sizeof(data))
// write a string
#define ws(data) file.write(data.c_str(), data.size() + 1)
// write a char*
#define wr(data, length) file.write(data, length)
// write a single byte
#define wb(data) file.put(data)
// read whatever the cast turns it into
#define r(data) file.read(reinterpret_cast<char*>(&data), sizeof(data))

static std::string rs(std::ifstream& file) {
	std::string result = "";
	char ch;
	while (file.get(ch)) {
		if (ch == '\0') {
			break;
		}
		result += ch;
	}
	return result;
}*/

void Scene::CalculateAllNodes() {
	allNodes = rootNode->GetDescendants();
}

//void Scene::SaveNode(std::ofstream& file, Node* node) {
//	ws(node->name);
//	w(node->transform);
//	// Components
//	wb('\x0002');
//	for (Component* component : node->components) {
//		auto cinfo = component->GetInfo();
//		ws(cinfo.name);
//	}
//	wb('\x0003');
//	// Children
//	wb('\x0002');
//	for (Node* child : node->children) {
//		SaveNode(file, child);
//	}
//	wb('\x0003');
//}

json Scene::SaveNode(Node* node) {
	std::vector<json> theChildren;
	theChildren.reserve(node->children.size());
	for (Node* child : node->children) {
		theChildren.push_back(SaveNode(child));
	}

	std::vector<json> theComponents;
	theComponents.reserve(node->components.size());
	for (Component* component : node->components) {
		ComponentTypeInfo info = component->GetInfo();
		json j = {
			{"name", info.name},
		};
		for (const ComponentFieldInfo& field : info.fields) {
			void* fieldptr = (char*)component + field.offset;
			switch (field.type) {
				case InputType::Float:
					j["fields"][field.name] = *((float*)fieldptr);
					break;
				case InputType::Int:
					j["fields"][field.name] = *((int*)fieldptr);
					break;
				case InputType::Bool:
					j["fields"][field.name] = *((bool*)fieldptr);
					break;
				case InputType::String:
					j["fields"][field.name] = *((std::string*)fieldptr);
					break;
				case InputType::Vector3:
					{
						Vector3 thing1 = *((Vector3*)fieldptr);
						j["fields"][field.name] = { thing1.x, thing1.y, thing1.z };
					}
					break;
				case InputType::Vector2:
					{
						Vector2 thing2 = *((Vector2*)fieldptr);
						j["fields"][field.name] = { thing2.x, thing2.y };
					}
					break;
			}
		}
		theComponents.push_back(j);
	}

	json j = {
		{"name", node->name},
		{"transform", {node->transform.position.x, node->transform.position.y, node->transform.position.z, node->transform.rotation.x, node->transform.rotation.y, node->transform.rotation.z, node->transform.scale.x, node->transform.scale.y, node->transform.scale.z}},
	};
	if (!theComponents.empty()) j["components"] = theComponents;
	if (!theChildren.empty()) j["children"] = theChildren;
	return j;
}

void Scene::SaveScene(const std::string& path) {
	std::ofstream file(path, std::ios::out | std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Error!!! Could not save scene at " << path << std::endl;
		return;
	}

	sj = {
		{"a", 0x189FB11},
	};

	sj["rootNode"] = SaveNode(rootNode);

	//// The format identifier is FDES1 converted from Base36 to Base16 which stands for FireDasherEngine Scene format 1
	//int formatIdentifier = 0x189FB11;
	//w(formatIdentifier);

	//int version = 1;
	//w(version);

	//SaveNode(file, rootNode);

	file << sj/*.dump(1, '\x009')*/;

	file.close();
}

//Node* Scene::LoadNode(std::ifstream& file, Node* parent) {
//	Node* node = parent ? new Node(rs(file), parent) : new Node(rs(file));
//	r(node->transform);
//	char byte;
//	while(file.get(byte) && byte != '\x0003') {
//		file.seekg(-1, std::ios::cur);
//		LoadNode(file, node);
//	}
//	allNodes.push_back(node);
//	return node;
//}

//Node* Scene::LoadNode(const json& nodeJson) {
//	Node* node = new Node(nodeJson["name"]);
//
//	node->transform.position.x = nodeJson["transform"][0];
//	node->transform.position.y = nodeJson["transform"][1];
//	node->transform.position.z = nodeJson["transform"][2];
//
//	node->transform.rotation.x = nodeJson["transform"][3];
//	node->transform.rotation.y = nodeJson["transform"][4];
//	node->transform.rotation.z = nodeJson["transform"][5];
//
//	node->transform.scale   .x = nodeJson["transform"][6];
//	node->transform.scale   .y = nodeJson["transform"][7];
//	node->transform.scale   .z = nodeJson["transform"][8];
//
//	if (nodeJson.contains("components")) {
//		for (const json& j : nodeJson["components"]) {
//			Component* component = ComponentRegistry::Get()[j["name"]]();
//			ComponentTypeInfo info = component->GetInfo();
//			for (const ComponentFieldInfo& field : info.fields) {
//				void* fieldptr = (char*)component + field.offset;
//				switch (field.type) {
//					case InputType::Float:
//						*((float*)fieldptr) = j["fields"][field.name];
//						break;
//					case InputType::Int:
//						*((int*)fieldptr) = j["fields"][field.name];
//						break;
//					case InputType::Bool:
//						*((bool*)fieldptr) = j["fields"][field.name];
//						break;
//					case InputType::String:
//						*((std::string*)fieldptr) = j["fields"][field.name];
//						break;
//					case InputType::Vector3:
//						{
//							auto& fielda = j["fields"][field.name];
//							*((Vector3*)fieldptr) = Vector3{fielda[0], fielda[1], fielda[2]};
//						}
//						break;
//					case InputType::Vector2:
//						{
//							auto& fielda = j["fields"][field.name];
//							*((Vector2*)fieldptr) = Vector2{fielda[0], fielda[1]};
//						}
//						break;
//				}
//			}
//		}
//	}
//
//	if (nodeJson.contains("children")) {
//		for (const json& j : nodeJson["children"]) {
//			node->children.push_back(LoadNode(j));
//		}
//	}
//
//	return node;
//}
//
//void Scene::LoadScene(const std::string& path) {
//	std::ifstream file(path, std::ios::in | std::ios::binary);
//	if (!file.is_open()) {
//		std::cerr << "Error!!! Could not load scene at " << path << std::endl;
//		return;
//	}
//
//	sj = json::parse(file);
//
//	if (sj["a"] != 0x189FB11) {
//		std::cerr << "Error!!! Incorrect file format at " << path << std::endl;
//		return;
//	}
//
//	rootNode = LoadNode(sj["rootNode"]);
//
//	/*int formatIdentifier = 0;
//	r(formatIdentifier);
//	if (formatIdentifier != 0x189FB11) {
//		std::cerr << "Error!!! Incorrect file format at " << path << std::endl;
//		return;
//	}
//	
//	int version = 0;
//	r(version);
//
//	rootNode = LoadNode(file, nullptr);*/
//
//	file.close();
//}