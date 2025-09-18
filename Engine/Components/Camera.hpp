#pragma once

#include "Component.hpp"

RegisterComponentFields class Camera : public Component {
	public:
		Camera();
		EditorField float fov = 90.0f;
		EditorField float nearPlane = 0.1f;
		EditorField float farPlane = 100.0f;
		int viewportWidth = 800, viewportHeight = 600;

		ComponentTypeInfo GetInfo()const override{return{"Camera",{{"Fov",InputType::Float,offsetof(Camera,fov)},{"Near Plane",InputType::Float,offsetof(Camera,nearPlane),0.1f},{"Far Plane",InputType::Float,offsetof(Camera,farPlane)},}};}
};