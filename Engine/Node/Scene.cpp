#include "Scene.hpp"
#include <fstream>

#include "Components/Camera.hpp"

// write whatever the cast turns it into
#define w(data) file.write(reinterpret_cast<const char*>(&data), sizeof(data))
// write a string
#define ws(data) file.write(data.c_str(), data.size() + 1)
// write a char*
#define wr(data, length) file.write(data, length)
// write a single byte
#define wb(data) file.put(data)

// read whatever the cast turns it into
#define r(data) file.read(reinterpret_cast<char*>(&data), sizeof(data));

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
}

void Scene::CalculateAllNodes() {
	AllNodes = RootNode->GetDescendants();
}

void Scene::SaveNode(std::ofstream& file, Node* node) {
	ws(node->name);
	w(node->transform);
	// Components
	/*wb('\x0002');
	for (Component* component : node->components) {
		auto cinfo = component->GetInfo();
		ws(cinfo.name);
	}
	wb('\x0003');*/
	// Children
	wb('\x0002');
	for (Node* child : node->children) {
		SaveNode(file, child);
	}
	wb('\x0003');
}

void Scene::SaveScene(const std::string& path) {
	std::ofstream file(path, std::ios::out | std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Error!!! Could not save scene at " << path << std::endl;
		return;
	}

	// The format identifier is FDES1 converted from Base36 to Base16 which stands for FireDasherEngine Scene format 1
	int formatIdentifier = 0x189FB11;
	w(formatIdentifier);

	int version = 1;
	w(version);

	SaveNode(file, RootNode);

	file.close();
}

Node* Scene::LoadNode(std::ifstream& file, Node* parent) {
	Node* node = parent ? new Node(rs(file), parent) : new Node(rs(file));
	r(node->transform);
	char byte;
	while(file.get(byte) && byte != '\x0003') {
		file.seekg(-1, std::ios::cur);
		LoadNode(file, node);
	}
	AllNodes.push_back(node);
	return node;
}

void Scene::LoadScene(const std::string& path) {
	std::ifstream file(path, std::ios::in | std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Error!!! Could not load scene at " << path << std::endl;
		return;
	}

	int formatIdentifier = 0;
	r(formatIdentifier);
	if (formatIdentifier != 0x189FB11) {
		std::cerr << "Error!!! Incorrect file format at " << path << std::endl;
		return;
	}
	
	int version = 0;
	r(version);

	RootNode = LoadNode(file, nullptr);

	file.close();
}