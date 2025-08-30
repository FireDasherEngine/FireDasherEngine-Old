#include "Component.hpp"
#include <fstream>
#include <regex>
#include <sstream>
#include <unordered_map>

static const std::unordered_map<std::string, std::string> nameToInputType = {
		{"float", "Float"},
		{"int", "Int"},
		{"bool", "Bool"},
		{"std::string", "String"},
		{"string", "String"}, // alias
		{"Vector3", "Vector3"},
		{"Vector2", "Vector2"},
		{"glm::vec2", "Vector2"}
};

static std::string format(const std::string& name) {
	std::regex pattern("(?!^)([A-Z])");
	std::string spacedString = std::regex_replace(name, pattern, " $1");
	if (!spacedString.empty())
		spacedString[0] = toupper(spacedString[0]);
	return spacedString;
}

std::string ComponentRegistry::GenerateGetInfoFunctionFromHeaderSource(const std::string& source) {
	std::stringstream the;
	//the << "/* Auto generated code, do not modify */ ";

	std::regex classRegex(R"(RegisterComponent\s+class\s+(\w+)\s*:\s*public\s+\w+\s*\{([\s\S]*)\};)");
	std::smatch classMatch;
	if (std::regex_search(source, classMatch, classRegex)) {
		std::string name = classMatch[1].str();
		std::string body = classMatch[2].str();

		//the << "ComponentTypeInfo " << name << "::GetInfo()const{return{\"" << format(name) << "\",{";
		the << "ComponentTypeInfo GetInfo()const override{return{\"" << format(name) << "\",{";
		
		std::regex fieldRegex(R"(EditorField\s+([a-zA-Z0-9_:]+)\s+([a-zA-Z0-9_:]+))");
		auto fieldsBegin = std::sregex_iterator(body.begin(), body.end(), fieldRegex);
		auto fieldsEnd = std::sregex_iterator();

		for (auto& it = fieldsBegin; it != fieldsEnd; ++it) {
			the << "{\"" << format((*it)[2].str()) << "\",InputType::" << nameToInputType.find((*it)[1].str())->second << ",offsetof(" << name << "," << (*it)[2].str() << ")},";
		}

		the << "}};}";
		
		//std::regex deleteOldGetInfoRegex(R"(/\* Auto generated code, do not modify \*/\s*?ComponentTypeInfo\s+?GetInfo\s*?\(\)\s*?const\s+?override\s*?\{[^]*?;\s*?\})");
		//std::regex deleteOldGetInfoRegex(R"(/\*Auto generated code 0x274317\*/[\s\S]*?/\*End0x274317\*/)");
		/*std::string updated = source;
		std::smatch deleteOldGetInfoMatch;
		if (std::regex_search(body, deleteOldGetInfoMatch, deleteOldGetInfoRegex)) {
			std::cout << "Found existing get info, replacing\n";
			updated.replace(classMatch.position(2), classMatch.length(2), std::regex_replace(body, deleteOldGetInfoRegex, the.str()));
		} else {
			std::cout << "Did not find existing get info, adding\n";
			updated.replace(classMatch.position(2), classMatch.length(2), body + "\n\n" + the.str());
		}
		
		return updated;*/
	}

	return the.str();
	//return source;
}