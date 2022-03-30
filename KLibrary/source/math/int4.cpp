#include "math/int4.h"

#include <iostream>

#include "utility/convert.h"


// Constructors
kl::int4::int4() {
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}
kl::int4::int4(int a) {
	x = a;
	y = a;
	z = a;
	w = a;
}
kl::int4::int4(int x, int y, int z, int w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}
kl::int4::int4(const kl::int2& v, int z, int w) {
	x = v.x;
	y = v.y;
	this->z = z;
	this->w = w;
}
kl::int4::int4(int x, const kl::int2& v, int w) {
	this->x = x;
	y = v.x;
	z = v.y;
	this->w = w;
}
kl::int4::int4(int x, int y, const kl::int2& v) {
	this->x = x;
	this->y = y;
	z = v.x;
	w = v.y;
}
kl::int4::int4(const kl::int2& v1, const kl::int2& v2) {
	x = v1.x;
	y = v1.y;
	z = v2.x;
	w = v2.y;
}
kl::int4::int4(const kl::int3& v, int w) {
	x = v.x;
	y = v.y;
	z = v.z;
	this->w = w;
}
kl::int4::int4(int x, const kl::int3& v) {
	this->x = x;
	y = v.x;
	z = v.y;
	w = v.z;
}

// Addition
kl::int4 kl::int4::add(const kl::int4& obj) const {
	return kl::int4(x + obj.x, y + obj.y, z + obj.z, w + obj.w);
}
kl::int4 kl::int4::operator+(const kl::int4& obj) const {
	return this->add(obj);
}
void kl::int4::operator+=(const kl::int4& obj) {
	x += obj.x; y += obj.y; z += obj.z; w += obj.w;
}

// Subtraction
kl::int4 kl::int4::sub(const kl::int4& obj) const {
	return kl::int4(x - obj.x, y - obj.y, z - obj.z, w - obj.w);
}
kl::int4 kl::int4::operator-(const kl::int4& obj) const {
	return this->sub(obj);
}
void kl::int4::operator-=(const kl::int4& obj) {
	x -= obj.x; y -= obj.y; z -= obj.z; w -= obj.w;
}

// Multiplication
kl::int4 kl::int4::mul(int a) const {
	return kl::int4(x * a, y * a, z * a, w * a);
}
kl::int4 kl::int4::operator*(int a) const {
	return this->mul(a);
}
void kl::int4::operator*=(int a) {
	x *= a; y *= a; z *= a; w *= a;
}
kl::int4 kl::int4::mul(const kl::int4& obj) const {
	return kl::int4(x * obj.x, y * obj.y, z * obj.z, w * obj.w);
}
kl::int4 kl::int4::operator*(const kl::int4& obj) const {
	return this->mul(obj);
}
void kl::int4::operator*=(const kl::int4& obj) {
	x *= obj.x; y *= obj.y; z *= obj.z; w *= obj.w;
}

// Division
kl::int4 kl::int4::div(int a) const {
	return kl::int4(x / a, y / a, z / a, w / a);
}
kl::int4 kl::int4::operator/(int a) const {
	return this->div(a);
}
void kl::int4::operator/=(int a) {
	x /= a; y /= a; z /= a; w /= a;
}
kl::int4 kl::int4::div(const kl::int4& obj) const {
	return kl::int4(x / obj.x, y / obj.y, z / obj.z, w / obj.w);
}
kl::int4 kl::int4::operator/(const kl::int4& obj) const {
	return this->div(obj);
}
void kl::int4::operator/=(const kl::int4& obj) {
	x /= obj.x; y /= obj.y; z /= obj.z; w /= obj.w;
}

// Comparison
bool kl::int4::equals(const kl::int4& obj) const {
	return x == obj.x && y == obj.y && z == obj.z && w == obj.w;
}
bool kl::int4::operator==(const kl::int4& obj) const {
	return this->equals(obj);
}
bool kl::int4::operator!=(const kl::int4& obj) const {
	return !this->equals(obj);
}

// Returns a negated vec
kl::int4 kl::int4::negate() const {
	return this->mul(-1);
}

// Returns an absolute vec
kl::int4 kl::int4::abso() const {
	return kl::int4(abs(x), abs(y), abs(z), abs(w));
}

// Overloading std::cout
std::ostream& kl::operator<<(std::ostream& os, const kl::int4& obj) {
	os << "(" << kl::convert::toString(obj.x) << ", " << kl::convert::toString(obj.y) << ", " << kl::convert::toString(obj.z) << ", " << kl::convert::toString(obj.w) << ")";
	return os;
}
