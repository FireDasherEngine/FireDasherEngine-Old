#include "Component.hpp"

#include <iostream>

Component::Component() {}
void Component::Start() {}
void Component::Update() {}
//ComponentTypeInfo Component::GetInfo() const { return {"Unknown Component"}; }

/*std::map<std::string, ComponentTypeInfo> ComponentRegistry::components = {
	{"Camera",
		{"Camera",
			{
				{"Field of View", InputType::Float, offsetof(Camera, fov)},
				{"Near Plane", InputType::Float, offsetof(Camera, nearPlane), 0.01f},
				{"Far Plane", InputType::Float, offsetof(Camera, farPlane), 0.01f},
			}
		}
	},
	{"Mesh Renderer",
		{"Mesh Renderer",
			{
				{"Mesh File Path", InputType::String, offsetof(MeshRenderer, MeshPath)},
				{"Material File Path", InputType::String, offsetof(MeshRenderer, MaterialPath)},
			}
		}
	},
};*/