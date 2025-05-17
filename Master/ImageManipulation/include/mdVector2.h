#pragma once

#include "mdImageManipulationPrerequisites.h"

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif


namespace MD {


class Vector2
{
public:

  Vector2() = default;

  Vector2(const float& nx, const float& ny = 0);

  Vector2(const float& values);


  Vector2(const Vector2& other);

  ~Vector2() = default;

  /**
    * @brief () operator overload
    * @param index of the object we wanna get
    * @return value at index
    *
    */
  const float&
  operator()(const uint32_t& index) const;

  /**
    * @brief () operator overload
    * @param index of the object we wanna get
    * @return value at index
    *
    */
  float&
  operator()(const uint32_t& index);

  /**
    * @brief [] operator overload
    * @param index of the object we wanna get
    * @return value at index
    *
    */
  const float
  operator[](const uint32_t& index) const;

  /**
    * @brief [] operator overload
    * @param index of the object we wanna get
    * @return value at index
    *
    */
  float&
  operator[](const uint32_t& index);

  /**
    * @brief + operator overload
    * @param b the other matrix to add
    * @return a vector sum of *this and v
    *
    */
  Vector2
  operator+(const Vector2& v) const;

  /**
    * @brief - operator overload
    * @param b the other matrix to substract
    * @return a vector difference of *this and v
    *
    */
  Vector2
  operator-(const Vector2& v) const;

  /**
    * @brief * operator overload
    * @param b the other matrix to multiply
    * @return a vector multiplication of *this times v
    *
    */
  Vector2
  operator*(const Vector2& v) const;

  /**
    * @brief / operator overload
    * @param b the other matrix to divide
    * @return a vector division of *this divided by v
    *
    */
  Vector2
  operator/(const Vector2& v) const;

  /**
    * @brief + operator overload
    * @param plus the float to add to all vector
    * @return a vector sum of *this + plus
    *
    */
  Vector2
  operator+(const float& plus) const;

  /**
    * @brief - operator overload
    * @param minus the float to subtract to all vector
    * @return a vector difference of *this - minus
    *
    */
  Vector2
  operator-(const float& minus) const;

  /**
    * @brief * operator overload
    * @param times the float to multiply all vector
    * @return a vector multiplication of *this times times
    *
    */
  Vector2
  operator*(const float& times) const;

  /**
    * @brief / operator overload
    * @param under the float to divide all vector
    * @return a vector divided of *this divided by times
    *
    */
  Vector2
  operator/(const float& under) const;

  /**
    * @brief | operator overload for dot product
    * @param the other vector
    * @return dot product
    *
    */
  float
  operator|(const Vector2& v) const;

  /**
    * @brief ^ operator overload for cross product
    * @param the other vector
    * @return cross product
    *
    */
  float
  operator^(const Vector2& v) const;

  /**
    * @brief == operator overload
    * @param the other vector to compare
    * @return true if this components are == to v components
    *
    */
  bool
  operator==(const Vector2& v) const;

  /**
    * @brief != operator overload
    * @param the other vector to compare
    * @return true if this components are != to v components
    *
    */
  bool
  operator!=(const Vector2& v) const;

  /**
    * @brief < operator overload
    * @param the other vector to compare
    * @return true if this components are < to v components
    *
    */
  bool
  operator<(const Vector2& v) const;

  /**
    * @brief > operator overload
    * @param the other vector to compare
    * @return true if this components are > to v components
    *
    */
  bool
  operator>(const Vector2& v) const;

  /**
    * @brief <= operator overload
    * @param the other vector to compare
    * @return true if this components are <= to v components
    *
    */
  bool
  operator<=(const Vector2& v) const;

  /**
    * @brief >= operator overload
    * @param the other vector to compare
    * @return true if this components are >= to v components
    *
    */
  bool
  operator>=(const Vector2& v) const;

  /**
    * @brief - operator overload
    * @param
    * @return the negative of the vector
    *
    */
  Vector2
  operator-() const;

  /**
    * @brief += operator overload
    * @param the other vector to add
    * @return *this + v components
    *
    */
  Vector2&
  operator+=(const Vector2& v);

  /**
    * @brief -= operator overload
    * @param the other vector to subtract
    * @return *this - v components
    *
    */
  Vector2&
  operator-=(const Vector2& v);

  /**
    * @brief *= operator overload
    * @param the other vector to multiply
    * @return *this * v components
    *
    */
  Vector2&
  operator*=(const Vector2& v);

  /**
    * @brief /= operator overload
    * @param the other vector to divide
    * @return *this / v components
    *
    */
  Vector2&
  operator/=(const Vector2& v);

  /**
    * @brief *= operator overload
    * @param the float to multiply with
    * @return *this * float
    *
    */
  Vector2&
  operator*=(const float& scale);

  /**
    * @brief /= operator overload
    * @param the float to divide the vector
    * @return *this / float
    *
    */
  Vector2&
  operator/=(const float& scale);

  /**
    * @brief the dot product
    * @param a nauVector2 and b nauVector2
    * @return the dot product between a and b
    *
    */
  static float
  dot(const Vector2& a, const Vector2& b);

  /**
    * @brief the cross product
    * @param a nauVector2 and b nauVector2
    * @return the cross product between a and b
    *
    */
  static float
  cross(const Vector2& a, const Vector2& b);

  /**
    * @brief the scale of b over a
    * @param Vector2 that is the reflection and b
    * @param Vector2 that reflects over
    * @return scale of b in a
    *
    */
  static float
  projection(const Vector2& a, const Vector2& b);

  /**
    * @brief the square distance
    * @param a nauVector2 and b nauVector2
    * @return the square distance bewteen the two points
    *
    */
  static float
  sqrDistance(const Vector2& a, const Vector2& b);

  /**
    * @brief the distance between two points in 2d space
    * @param a nauVector2 and b nauVector2
    * @return the distance bewteen the two points
    *
    */
  static float
  distance(const Vector2& a, const Vector2& b);

  /**
    * @brief sets the value of a nauVector2
    * @param float x and y
    * @return
    *
    */
  void
  setValues(const float& nx, const float& ny);

  /**
    * @brief gets the min between two vectors
    * @param the other vector to compare
    * @return
    *
    */
  void
  min(const Vector2& v);

  /**
    * @brief gets the max between two vectors
    * @param the other vector to compare
    * @return
    *
    */
  void
  max(const Vector2& v);

  /**
    * @brief gets the highest value of the vector
    * @param
    * @return
    *
    */
  float
  getHighest() const;

  /**
    * @brief gets the least value of the vector
    * @param
    * @return
    *
    */
  float
  getLowest() const;

  /**
    * @brief gets the magnitude value of the vector
    * @param
    * @return
    *
    */
  float
  magnitude() const;

  /**
    * @brief gets the squared magnitude value of the vector
    * @param
    * @return
    *
    */
  float
  sqrMagnitude() const;

  /**
    * @brief gets the normalized version of the vector
    * @param
    * @return the normalized vector
    *
    */
  Vector2
  normalized() const;

  /**
    * @brief gets the QUICK normalized version of the vector using the Carmack Equation
    * Use this if you don't have problems having a small error difference from the real value
    * @param
    * @return the normalized vector
    *
    */
  Vector2
  qNormalized() const;

  /**
    * @brief normalizes the vector
    * @param
    * @return
    *
    */
  void
  normalize();

  /**
    * @brief QUICK normalizes the vector using the Carmack Equation
    * Use this if you don't have problems having a small error difference from the real value
    * @param
    * @return
    *
    */
  void
  qNormalize();

  /**
    * @brief Checks if the vector is empty (0s)
    * @param
    * @return true if it is empty
    *
    */
  bool
  isZero() const;

  /**
    * @brief Checks if one vector is the same as another vector
    * @param Vector first Vector to compare
    * @param Vector second Vector to compare
    * @return true if based on the error is the same
    *
    */
  static bool
  isSame(const Vector2& a, const Vector2& b);

  /**
    * @brief Checks if one vector is near the same as another vector based in an error
    * @param const Vector2f& first Vector to compare
    * @param const Vector2f& second Vector to compare
    * @param const float& threshold between the magnitude of both vectors
    * @return true if based on the error is the same
    *
    */
  static bool
  isNearlySame(const Vector2& a, const Vector2& b, const float& error = 0.0001f);

  Vector2
  toNearestOctant() const;


  /**
    * @brief Returns the vector as a printable string
    * @param
    * @return
    *
    */
  String
  toString();

  /**
    * static const
    */

public:

  /**
    * Vector2 with all values 0
    */
  static const Vector2 ZERO;

  /**
    * Vector2 with all values 1
    */
  static const Vector2 ONE;

  /**
    * Vector2 with x value 1
    */
  static const Vector2 ONEX;

  /**
    * Vector2 with y value 1
    */
  static const Vector2 ONEY;

  /**
    * Vector2 with y value 1. This is our right in our world
    */
  static const Vector2 UP;

  /**
    * Vector2 with x value 1. This is our up in our world
    */
  static const Vector2 RIGHT;

  /**
    * Vector2 with y value -1. This is our down in our world
    */
  static const Vector2 DOWN;

  /**
    * Vector2 with x value -1. This is our left in our world
    */
  static const Vector2 LEFT;

  /**
    * Member declaration
    */

public:

  /**
    * x component
    */
  float x;

  /**
    * y component
    */
  float y;

};

}