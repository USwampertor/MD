
#include "mdVector2.h"


namespace MD {

  Vector2::Vector2(const float& nx, const float& ny)
    : x(nx),
      y(ny) {}

  Vector2::Vector2(const float& values)
    : x(values), 
      y(values) {}

 
  Vector2::Vector2(const Vector2& other) 
    : x(other[0]), 
      y(other[1]) {}

  float&
  Vector2::operator()(const uint32_t& index) {
    return (&x)[index];
  }

  const float&
  Vector2::operator()(const uint32_t& index) const {
    return (&x)[index];
  }

  float&
  Vector2::operator[](const uint32_t& index) {
    return (&x)[index];
  }

  const float
  Vector2::operator[](const uint32_t& index) const {
    return (&x)[index];
  }


  Vector2
  Vector2::operator+(const Vector2& v) const {
    return Vector2(x + v.x, y + v.y);
  }

  Vector2
  Vector2::operator-(const Vector2& v) const {
    return Vector2(x - v.x, y - v.y);
  }

  Vector2
  Vector2::operator*(const Vector2& v) const {
    return Vector2(x * v.x, y * v.y);
  }

  Vector2
  Vector2::operator/(const Vector2& v) const {
    return Vector2(x / v.x, y / v.y);
  }

  Vector2
  Vector2::operator+(const float& plus) const {
    return Vector2(x + plus, y + plus);
  }

  Vector2
  Vector2::operator-(const float& minus) const {
    return Vector2(x - minus, y - minus);
  }

  Vector2
  Vector2::operator*(const float& times) const {
    return Vector2(x * times, y * times);
  }

  Vector2
  Vector2::operator/(const float& under) const {
    return Vector2(x / under, y / under);
  }

  float
  Vector2::operator|(const Vector2& v) const {
    return x * v.x + y * v.y;
  }

  float
  Vector2::operator^(const Vector2& v) const {
    return x * v.x - y * v.y;
  }

  bool
  Vector2::operator==(const Vector2& v) const {
    return x == v.x && y == v.y;
  }

  bool
  Vector2::operator!=(const Vector2& v) const {
    return x != v.x || y != v.y;
  }

  bool
  Vector2::operator<(const Vector2& v) const {
    return x < v.x&& y < v.y;
  }

  bool
  Vector2::operator>(const Vector2& v) const {
    return x > v.x && y > v.y;
  }

  bool
  Vector2::operator<=(const Vector2& v) const {
    return x <= v.x && y <= v.y;
  }

  bool
  Vector2::operator>=(const Vector2& v) const {
    return x >= v.x && y >= v.y;
  }

  Vector2
  Vector2::operator-() const {
    return Vector2(-x, -y);
  }

  Vector2&
  Vector2::operator+=(const Vector2& v) {
    x += v.x;
    y += v.y;
    return *this;
  }

  Vector2&
  Vector2::operator-=(const Vector2& v) {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  Vector2&
  Vector2::operator*=(const Vector2& v) {
    x *= v.x;
    y *= v.y;
    return *this;
  }

  Vector2&
  Vector2::operator/=(const Vector2& v) {
    x /= v.x;
    y /= v.y;
    return *this;
  }

  Vector2&
  Vector2::operator*=(const float& scale) {
    x *= scale;
    y *= scale;
    return *this;
  }

  Vector2&
  Vector2::operator/=(const float& scale) {
    x /= scale;
    y /= scale;
    return *this;
  }

  float
  Vector2::dot(const Vector2& a, const Vector2& b) {
    return a | b;
  }

  float
  Vector2::cross(const Vector2& a, const Vector2& b) {
    return a ^ b;
  }

  float
  Vector2::projection(const Vector2& a, const Vector2& b) {
    return (a | b) / a.magnitude();
  }

  float
  Vector2::sqrDistance(const Vector2& a, const Vector2& b) {
    return std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2);
  }

  float
  Vector2::distance(const Vector2& a, const Vector2& b) {
    return std::sqrt(Vector2::sqrDistance(a, b) /*Math::sqr(a.x + b.x) + Math::sqr(a.y + b.y)*/);
  }

  void
  Vector2::setValues(const float& newX, const float& newY) {
    x = newX;
    y = newY;
  }

  void
  Vector2::min(const Vector2& v) {
    if (v.x < x) { x = v.x; }
    if (v.y < y) { y = v.y; }
  }

  void
  Vector2::max(const Vector2& v) {
    if (v.x > x) { x = v.x; }
    if (v.y > y) { y = v.y; }
  }

  float
  Vector2::getHighest() const {
    return std::max(x, y);
  }

  float
  Vector2::getLowest() const {
    return std::min(x, y);
  }

  float
  Vector2::magnitude() const {
    return std::sqrt(this->sqrMagnitude()/*x * x + y * y*/);
  }

  float
  Vector2::sqrMagnitude() const {
    return (x * x + y * y);
  }

  Vector2
  Vector2::normalized() const {
   
    float sqr = std::pow(static_cast<float>(x), 2.0f) +
                std::pow(static_cast<float>(y), 2.0f);
   
    float unit = 1.0f / std::sqrt(sqr);

    return Vector2((x * static_cast<float>(unit)), (y * static_cast<float>(unit)));
  }

  Vector2
  Vector2::qNormalized() const {
 
    float sqr = std::pow(static_cast<float>(x), 2.0f) +
                std::pow(static_cast<float>(y), 2.0f);
   
    float unit = 1.0f / std::sqrt(sqr);

    return Vector2((x * static_cast<float>(unit)), (y * static_cast<float>(unit)));
  }

  void
  Vector2::normalize() {
    *this = this->normalized();
//     CY_ASSERT(!Math::isNaN(x) &&
//               !Math::isNaN(y) &&
//               !Math::isInfinite(x) &&
//               !Math::isInfinite(y) &&
//                Utils::format("Value X or Y are either infinite or NAN").c_str());
// 
//     float sqr = Math::pow(static_cast<float>(x), 2.0f) +
//                 Math::pow(static_cast<float>(y), 2.0f);
// 
//     CY_ASSERT(sqr <= Math::EPSILONF &&
//               Utils::format("Invalid normalization: value inside square root is %f", sqr).c_str());
// 
//     CY_DEBUG_ONLY(sqrMagnitude());
// 
//     float unit = Math::invSqrt(sqr);
//     x *= static_cast<float>(unit);
//     y *= static_cast<float>(unit);
  }

  void
  Vector2::qNormalize() {
    *this = this->qNormalized();
//     CY_ASSERT(!Math::isNaN(x) &&
//               !Math::isNaN(y) &&
//               !Math::isInfinite(x) &&
//               !Math::isInfinite(y) &&
//                Utils::format("Value X or Y are either infinite or NAN").c_str());
// 
//     float sqr = Math::pow(static_cast<float>(x), 2.0f) +
//                 Math::pow(static_cast<float>(y), 2.0f);
// 
//     CY_ASSERT(sqr <= Math::EPSILONF &&
//               Utils::format("Invalid normalization: value inside square root is %f", sqr).c_str());
// 
//     CY_DEBUG_ONLY(sqrMagnitude());
// 
//     float unit = Math::qInvSqrt(sqr);
//     x *= static_cast<float>(unit);
//     y *= static_cast<float>(unit);
  }

  bool
  Vector2::isZero() const {
    return 0 == x && 0 == y;
  }

  bool
  Vector2::isSame(const Vector2& a, const Vector2& b) {
    return a == b;
  }

  bool
  Vector2::isNearlySame(const Vector2& a, const Vector2& b, const float& error) {
    return std::abs(a.x - b.x) <= error &&
           std::abs(a.y - b.y) <= error ;
  }

  Vector2
  Vector2::toNearestOctant() const {
    
    return Vector2(0,0);
  }

  String
  Vector2::toString() {

    String output;

    output += "( ";
    output += Utils::format("%2.2f", x);
    output += ", ";
    output += Utils::format("%2.2f", y);
    output += " )";

    return output;
  }

  const Vector2 Vector2::ZERO   = Vector2(0, 0);

  const Vector2 Vector2::ONE    = Vector2(1, 1);

  const Vector2 Vector2::ONEX   = Vector2(1, 0);

  const Vector2 Vector2::ONEY   = Vector2(0, 1);

  const Vector2 Vector2::UP     = Vector2(0, 1);

  const Vector2 Vector2::RIGHT  = Vector2(1, 0);

  const Vector2 Vector2::DOWN = Vector2(0, -1);

  const Vector2 Vector2::LEFT = Vector2(-1, 0);
}