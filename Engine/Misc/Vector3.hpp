#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <format>

class Vector4 {
	public:
		Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
		Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
		Vector4(float s) : x(s), y(s), z(s), w(s) {}
		float x, y, z, w;
		inline glm::vec4 glm() { return glm::vec4(x, y, z, w); };
		inline std::string str() { return std::format("({}, {}, {}, {})", x, y, z, w); }
};

class Vector3 {
	public:
		Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
		Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
		Vector3(float s) : x(s), y(s), z(s) {}
		inline glm::vec3 glm() { return glm::vec3(x, y, z); };
		inline std::string str() { return std::format("({}, {}, {})", x, y, z); }

		inline Vector3 operator*(float scale) const { return {x * scale, y * scale, z * scale}; }
		inline Vector3 operator*(const Vector3& scale) const { return {x * scale.x, y * scale.y, z * scale.z}; }
		inline Vector3 operator+(const Vector3& add) const { return {x + add.x, y + add.y, z + add.z}; }

		inline void operator*=(float scale) {x *= scale; y *= scale; z *= scale;}
		inline void operator*=(const Vector3& scale) {x *= scale.x; y *= scale.y; z *= scale.z;}
		inline void operator+=(const Vector3& add) {x += add.x; y += add.y; z += add.z;}

		static Vector3 cross(const Vector3& a, const Vector3& b) {
			return {
				a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x
			};
		}

		float x, y, z;
};

class Vector2 {
	public:
		Vector2() : x(0.0f), y(0.0f) {}
		Vector2(float x, float y) : x(x), y(y) {}
		Vector2(float s) : x(s), y(s) {}
		float x, y;
		inline std::string str() { return std::format("({}, {})", x, y); }
};