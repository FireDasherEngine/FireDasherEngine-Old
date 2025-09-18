#pragma once

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include "Component.hpp"

#define RegisterComponent(name) static const bool _component_registry_##name## = ComponentRegistry::RegistryRegisterComponent<name>(#name)

class Component;

enum class InputType {
	// You should NEVER use the Nothing type!!
	Nothing = 0,
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
		static std::string GenerateGetInfoFunctionFromHeaderSource(const std::string& source);
		template<typename T> static bool RegistryRegisterComponent(const std::string& name) {
			Get()[name] = [](){ return new T(); };
			return true;
		};
		static auto& Get() {
			static std::unordered_map<std::string, std::function<Component* ()>> components;
			return components;
		}
};