#pragma once

#include "Misc/Vector3.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <format>

class Transform {
	public:
		Transform() : position(0.0f), rotation(0.0f), scale(1.0f) {}
		Transform(const Vector3& position, const Vector3& rotation, const Vector3& scale) : position(position), rotation(rotation), scale(scale) {}
		Vector3 position, rotation, scale;
		inline glm::mat4 mat4() {
			glm::mat4 mat = glm::mat4(1.0f);
			mat = glm::translate(mat, position.glm());
			mat = glm::rotate(mat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			mat = glm::rotate(mat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			mat = glm::rotate(mat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			mat = glm::scale(mat, scale.glm());
			return mat;
		}
		inline std::string str() {
			return std::format("{}, {}, {}", position.str(), rotation.str(), scale.str());
		}
};