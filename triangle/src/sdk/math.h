#pragma once
#include <cmath>


class Vector3
{
public:
	float x, y, z;
public:
	Vector3(const float x = 0.0, const float y = 0.0, const float z = 0.0) : x(x), y(y), z(z) {}
	Vector3 operator + (const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
	Vector3 operator - (const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
	Vector3 operator * (const float& rhs) const { return Vector3(x * rhs, y * rhs, z * rhs); }
	Vector3 operator / (const float& rhs) const { return Vector3(x / rhs, y / rhs, z / rhs); }
	Vector3& operator += (const Vector3& rhs) { return *this = *this + rhs; }
	Vector3& operator -= (const Vector3& rhs) { return *this = *this - rhs; }
	Vector3& operator *= (const float& rhs) { return *this = *this * rhs; }
	Vector3& operator /= (const float& rhs) { return *this = *this / rhs; }
	float Length() const { return sqrtf(x * x + y * y + z * z); }
	Vector3 Normalize() const { return *this * (1 / Length()); }
	float Distance(const Vector3& rhs) const { return (*this - rhs).Length(); }
};

struct Vector2 { float x, y; };
struct Vector4 { float x, y, z, w; };


// Type aliases
using screen_t = Vector2;
using vec_t = Vector3;
using vec4_t = Vector4;
using velocity_t = float;
using milli_t = int32_t;

// Functions
bool world_to_screen(vec_t pos, screen_t& screen, float vm[16]);