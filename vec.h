/****************************

    vector-math.h
    version 1.8.2
    last change 2021.1.4/22:35
    code by Ark_Noya on Arks

*****************************/

#include <cmath>
#include <iostream>

using namespace std;

struct vec{
	float x;
	float y;
	float z;

	vec(float i = .0)
		:vec(i, i, i) {}
	vec(float x, float y, float z)
		:x(x), y(y), z(z) {}
};
vec operator* (const vec & a, const vec & b) {
	return vec(a.x * b.x, a.y * b.y, a.z * b.z);
}

struct vec2 {
	float x;
	float y;
	
	vec2(float i = .0)
		:vec2(i, i) {}
	vec2(float x, float y)
		:x(x), y(y) {}
};
vec2 operator- (const vec2 & a, const vec2 & b) {
	return vec2(a.x - b.x, a.y - b.y);
}
vec2 operator* (const vec2 & a, const vec2 & b) {
	return vec2(a.x * b.x, a.y * b.y);
}
vec2 operator/ (const vec2 & a, const vec2 & b) {
	return vec2(a.x / b.x, a.y / b.y);
}

ostream & operator << (ostream &os, const vec2 & v) {
	os << "(" << v.x << "," << v.y << ")";
	return os;
}