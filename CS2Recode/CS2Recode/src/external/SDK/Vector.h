#pragma once
#include <math.h>
#include <cmath>

struct Vec {
	float x, y, z;
	Vec(float x = 0.0f, float y = 0.0f, float z = 0.0f) :x(x), y(y), z(z) { }

	Vec operator + (const Vec& other) {
		return{ x + other.x,y + other.y,z + other.z };
	}

	Vec operator - (const Vec& other) {
		return{ x - other.x,y - other.y,z - other.z };
	}

	bool operator < (const Vec& other) {
		return (x < other.x && y < other.y && z < other.x);
	}

	bool operator > (const Vec& other) {
		return (x > other.x && y > other.y && z > other.x);
	}

	bool operator == (const Vec& other) {
		return (x == other.x && y == other.y && z == other.x);
	}

	float Dist(Vec& other) {
		return std::sqrt(std::pow(other.x - x, 2) + std::pow(other.y - y, 2));
	}

	bool IsZero() {
		return x == 0 && y == 0 && z == 0;
	}

};
