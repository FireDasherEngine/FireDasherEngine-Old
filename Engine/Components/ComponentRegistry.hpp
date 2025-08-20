#pragma once

#include <string>
#include <vector>

enum class InputType {
	// You should NEVER use the Nothing type!!
	Nothing,
	Float,
	Int,
	Bool,
	String,
	Vector3,
	Vector2
};

struct ComponentFieldInfo {
	std::string name;
	InputType type;
	size_t offset;
	float step = 1.0f;
};

struct ComponentTypeInfo {
	std::string name;
	std::vector<ComponentFieldInfo> fields;
};

class ComponentRegistry {
	public:
		/* Replaced by Component::GetInfo static void Register(const std::string& name, const std::vector<ComponentFieldInfo>& fields) {
			components[ name ] = ComponentTypeInfo{ name, fields };
		}
		static std::map<std::string, ComponentTypeInfo> components;*/

		static std::string GenerateGetInfoFunctionFromHeaderSource(const std::string& source);
};