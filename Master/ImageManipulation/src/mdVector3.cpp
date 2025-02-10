#include "mdVector3.h"

namespace MD {

  
  
Vector3::Vector3(const Vector3& other)
  : x(other.x),
    y(other.y),
    z(other.z) {}


Vector3::Vector3(const float& nx, const float& ny, const float& nz) 
  : x(nx),
    y(ny),
    z(nz) {}

float&
Vector3::operator()(const uint32_t& index) {
  return (&x)[index];
}

const float&
Vector3::operator()(const uint32_t& index) const {
  return (&x)[index];
}

float&
Vector3::operator[](const uint32_t& index) {
  return (&x)[index];
}

const float
Vector3::operator[](const uint32_t& index) const {
  return (&x)[index];
}


Vector3
Vector3::operator+(const Vector3& v) const {
  return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3
Vector3::operator-(const Vector3& v) const {
  return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3
Vector3::operator*(const Vector3& v) const {
  return Vector3(x * v.x, y * v.y, z * v.z);
}

Vector3
Vector3::operator/(const Vector3& v) const {
  return Vector3(x / v.x, y / v.y, z / v.z);
}

Vector3
Vector3::operator+(const float& plus) const {
  return Vector3(x + plus, y + plus, z + plus);
}

Vector3
Vector3::operator-(const float& minus) const {
  return Vector3(x - minus, y - minus, z - minus);
}

Vector3
Vector3::operator*(const float& times) const {
  return Vector3(x * times, y * times, z * times);
}

Vector3
Vector3::operator/(const float& under) const {
  return Vector3(x / under, y / under, z / under);
}

float
Vector3::operator|(const Vector3& v) const {
  return x * v.x + y * v.y + z * v.z;
}

Vector3
Vector3::operator^(const Vector3& v) const {
  return Vector3((y * v.z) - (z * v.y),
                  (z * v.x) - (x * v.z),
                  (x * v.y) - (y * v.x));
}

bool
Vector3::operator==(const Vector3& v) const {
  return x == v.x && y == v.y && z == v.z;
}

bool
Vector3::operator!=(const Vector3& v) const {
  return x != v.x || y != v.y || z != v.z;
}

bool
Vector3::operator<(const Vector3& v) const {
  return x < v.x && y < v.y && z < v.z;
}

bool
Vector3::operator>(const Vector3& v) const {
  return x > v.x && y > v.y && z > v.z;
}

bool
Vector3::operator<=(const Vector3& v) const {
  return x <= v.x && y <= v.y && z <= v.z;
}

bool
Vector3::operator>=(const Vector3& v) const {
  return x >= v.x && y >= v.y && z >= v.z;
}

Vector3
Vector3::operator-() const {
  return Vector3(-x, -y, -z);
}

Vector3&
Vector3::operator+=(const Vector3& v) {
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}

Vector3&
Vector3::operator-=(const Vector3& v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return *this;
}

Vector3&
Vector3::operator*=(const Vector3& v) {
  x *= v.x;
  y *= v.y;
  z *= v.z;
  return *this;
}

Vector3&
Vector3::operator/=(const Vector3& v) {
  x /= v.x;
  y /= v.y;
  z /= v.z;
  return *this;
}

Vector3&
Vector3::operator*=(const float& scale) {
  x *= scale;
  y *= scale;
  z *= scale;
  return *this;
}

Vector3&
Vector3::operator/=(const float& scale) {
  x /= scale;
  y /= scale;
  z /= scale;
  return *this;
}

float
Vector3::dot(const Vector3& a, const Vector3& b) {
  return a | b;
}

Vector3
Vector3::cross(const Vector3& a, const Vector3& b) {
  return a ^ b;
}

float
Vector3::projection(const Vector3& a, const Vector3& b) {
  return (a | b) / std::pow(a.magnitude(), 2);
}

float
Vector3::sqrDistance(const Vector3& a, const Vector3& b) {
  return std::pow(a.x - b.x, 2) +
         std::pow(a.y - b.y, 2) +
         std::pow(a.z - b.z, 2);
}

float
Vector3::distance(const Vector3& a, const Vector3& b) {
  return 
      std::sqrt(std::pow(a.x - b.x, 2) +
                std::pow(a.y - b.y, 2) +
                std::pow(a.z - b.z, 2));
}

void
Vector3::setValues(const float& newX, const float& newY, const float& newZ) {
  x = newX;
  y = newY;
  z = newZ;
}

void
Vector3::min(const Vector3& v) {
  if (v.x < x) x = v.x;
  if (v.y < y) y = v.y;
  if (v.z < z) z = v.z;
}

void
Vector3::max(const Vector3& v) {
  if (v.x > x) x = v.x;
  if (v.y > y) y = v.y;
  if (v.z > z) z = v.z;
}

void
Vector3::floor() {
  x = std::floor(x);
  y = std::floor(y);
  z = std::floor(z);
}

void
Vector3::ceiling() {
  x = std::ceil(x);
  y = std::ceil(y);
  z = std::ceil(z);
}

void
Vector3::round() {
  x = std::round(x);
  y = std::round(y);
  z = std::round(z);
}

float
Vector3::magnitude() const {
  return std::sqrt(x * x + y * y + z * z);
}

float
Vector3::sqrMagnitude() const {
  return (x * x + y * y + z * z);
}

Vector3
Vector3::normalized() const {

  float sqr = std::pow(x, 2.0f) + std::pow(y, 2.0f) + std::pow(z, 2.0f);
 
  float unit = 1.0f / std::sqrt(sqr);

  return Vector3((x * unit), (y * unit), (z * unit));
}


void
Vector3::normalize() {

  float sqr = std::pow(x, 2.0f) + std::pow(y, 2.0f) + std::pow(z, 2.0f);

  float unit = 1.0f / std::sqrt(sqr);

  x *= unit;
  y *= unit; 
  z *= unit;
}

bool
Vector3::isZero() const {
  return 0.0f == x && 0.0f == y && 0.0f == z;
}

bool
Vector3::isNearlySame(const Vector3& a, const Vector3& b, const float& error) {
  return std::abs(a.x - b.x) <= error &&
         std::abs(a.y - b.y) <= error &&
         std::abs(a.z - b.z) <= error;
}

String
Vector3::toString() {

  String output;

  output += "( ";
  output += Utils::format("%2.2f", x);
  output += ", ";
  output += Utils::format("%2.2f", y);
  output += ", ";
  output += Utils::format("%2.2f", z);
  output += " )";

  return output;
}
  
const Vector3 Vector3::ZERO   = Vector3(0.0f, 0.0f, 0.0f);

const Vector3 Vector3::ONE    = Vector3(1.0f, 1.0f, 1.0f);

const Vector3 Vector3::ONEX   = Vector3(1.0f, 0.0f, 0.0f);

const Vector3 Vector3::ONEY   = Vector3(0.0f, 1.0f, 0.0f);

const Vector3 Vector3::ONEZ   = Vector3(0.0f, 0.0f, 1.0f);

const Vector3 Vector3::RIGHT  = Vector3(1.0f, 0.0f, 0.0f);

const Vector3 Vector3::UP     = Vector3(0.0f, 1.0f, 0.0f);

const Vector3 Vector3::FRONT  = Vector3(0.0f, 0.0f, 1.0f);

const Vector3 Vector3::LEFT   = Vector3(-1.0f, 0.0f, 0.0f);

const Vector3 Vector3::DOWN   = Vector3(0.0f, -1.0f, 0.0f);

const Vector3 Vector3::BACK   = Vector3(0.0f, 0.0f, -1.0f);


}