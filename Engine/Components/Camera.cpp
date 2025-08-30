#include "Camera.hpp"

Camera::Camera() {}
/*ComponentTypeInfo Camera::GetInfo() const {
	return
		{"Camera",
			{
				{"Field of View", InputType::Float, offsetof(Camera, fov)},
				{"Near Plane", InputType::Float, offsetof(Camera, nearPlane), 0.01f},
				{"Far Plane", InputType::Float, offsetof(Camera, farPlane), 0.01f},
			}
		};
}*/
//ComponentTypeInfo Camera::GetInfo()const{return{"Camera",{{"Fov",InputType::Float,offsetof(Camera,fov)},{"Near Plane",InputType::Float,offsetof(Camera,nearPlane)},{"Far Plane",InputType::Float,offsetof(Camera,farPlane)},}};}
RegisterComponent(Camera);