#pragma once
#include "mdImageManipulationPrerequisites.h"

namespace MD {

class Vector3 {
public:

  Vector3() = default;

  Vector3(const Vector3& other);

  Vector3(const float& nx, const float& ny = 0, const float& nz = 0);

  ~Vector3() = default;

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
  Vector3
  operator+(const Vector3& v) const;

  /**
    * @brief - operator overload
    * @param b the other matrix to substract
    * @return a vector difference of *this and v
    *
    */
  Vector3
  operator-(const Vector3& v) const;

  /**
    * @brief * operator overload
    * @param b the other matrix to multiply
    * @return a vector multiplication of *this times v
    *
    */
  Vector3
  operator*(const Vector3& v) const;

  /**
    * @brief / operator overload
    * @param b the other matrix to divide
    * @return a vector division of *this divided by v
    *
    */
  Vector3
  operator/(const Vector3& v) const;

  /**
    * @brief + operator overload
    * @param plus the float to add to all vector
    * @return a vector sum of *this + plus
    *
    */
  Vector3
  operator+(const float& plus) const;

  /**
    * @brief - operator overload
    * @param minus the float to subtract to all vector
    * @return a vector difference of *this - minus
    *
    */
  Vector3
  operator-(const float& minus) const;

  /**
    * @brief * operator overload
    * @param times the float to multiply all vector
    * @return a vector multiplication of *this times times
    *
    */
  Vector3
  operator*(const float& times) const;

  /**
    * @brief / operator overload
    * @param under the float to divide all vector
    * @return a vector divided of *this divided by times
    *
    */
  Vector3
  operator/(const float& under) const;

  /**
    * @brief | operator overload for dot product
    * @param the other vector
    * @return dot product
    *
    */
  float
  operator|(const Vector3& v) const;

  /**
    * @brief ^ operator overload for cross product
    * @param the other vector
    * @return cross product
    *
    */
  Vector3
  operator^(const Vector3& v) const;

  /**
    * @brief == operator overload
    * @param the other vector to compare
    * @return true if this components are == to v components
    *
    */
  bool
  operator==(const Vector3& v) const;

  /**
    * @brief != operator overload
    * @param the other vector to compare
    * @return true if this components are != to v components
    *
    */
  bool
  operator!=(const Vector3& v) const;

  /**
    * @brief < operator overload
    * @param the other vector to compare
    * @return true if this components are < to v components
    *
    */
  bool
  operator<(const Vector3& v) const;

  /**
    * @brief > operator overload
    * @param the other vector to compare
    * @return true if this components are > to v components
    *
    */
  bool
  operator>(const Vector3& v) const;

  /**
    * @brief <= operator overload
    * @param the other vector to compare
    * @return true if this components are <= to v components
    *
    */
  bool
  operator<=(const Vector3& v) const;

  /**
    * @brief >= operator overload
    * @param the other vector to compare
    * @return true if this components are >= to v components
    *
    */
  bool
  operator>=(const Vector3& v) const;

  /**
    * @brief - operator overload
    * @param
    * @return the negative of the vector
    *
    */
  Vector3
  operator-() const;

  /**
    * @brief += operator overload
    * @param the other vector to add
    * @return *this + v components
    *
    */
  Vector3&
  operator+=(const Vector3& v);

  /**
    * @brief -= operator overload
    * @param the other vector to subtract
    * @return *this - v components
    *
    */
  Vector3&
  operator-=(const Vector3& v);

  /**
    * @brief *= operator overload
    * @param the other vector to multiply
    * @return *this * v components
    *
    */
  Vector3&
  operator*=(const Vector3& v);

  /**
    * @brief /= operator overload
    * @param the other vector to divide
    * @return *this / v components
    *
    */
  Vector3&
  operator/=(const Vector3& v);

  /**
    * @brief *= operator overload
    * @param the float to multiply with
    * @return *this * float
    *
    */
  Vector3&
  operator*=(const float& scale);

  /**
    * @brief /= operator overload
    * @param the float to divide the vector
    * @return *this / float
    *
    */
  Vector3&
  operator/=(const float& scale);

  /**
    * @brief the dot product
    * @param a Vector3 and b Vector3
    * @return the dot product between a and b
    *
    */
  static float
  dot(const Vector3& a, const Vector3& b);

  /**
    * @brief the cross product
    * @param a Vector3 and b Vector3
    * @return the cross product between a and b
    *
    */
  static Vector3
  cross(const Vector3& a, const Vector3& b);

  /**
    * @brief the projection of b over a
    * @param Vector3 a the one used as scale
    * @param Vector3 b the one projecting on a
    * @return scale of b in a
    *
    */
  static float
  projection(const Vector3& a, const Vector3& b);

  /**
    * @brief the square distance
    * @param a Vector3 and b Vector3
    * @return the square distance between the two points
    *
    */
  static float
  sqrDistance(const Vector3& a, const Vector3& b);

  /**
    * @brief the distance between two points in 2d space
    * @param a Vector3 and b Vector3
    * @return the distance between the two points
    *
    */
  static float
  distance(const Vector3& a, const Vector3& b);

  /**
    * @brief sets the value of a Vector3
    * @param float x
    * @param float y
    * @param float z
    * @return
    *
    */
  void
  setValues(const float& newX, const float& newY, const float& newZ);

  /**
    * @brief gets the min between two vectors
    * @param the other vector to compare
    * @return
    *
    */
  void
  min(const Vector3& v);

  /**
    * @brief gets the max between two vectors
    * @param the other vector to compare
    * @return
    *
    */
  void
  max(const Vector3& v);

  /**
    * @brief floors the components
    * @param
    * @return
    *
    */
  void
  floor();

  /**
    * @brief ceils the components
    * @param
    * @return
    *
    */
  void
  ceiling();

  /**
    * @brief rounds the components x.0 <- -> y.0
    * @param
    * @return
    *
    */
  void
  round();

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
  Vector3
  normalized() const;

  /**
    * @brief normalizes the vector
    * @param
    * @return
    *
    */
  void
  normalize();

  /**
    * @brief Checks if the vector is empty (0s)
    * @param
    * @return true if it is empty
    *
    */
  bool
  isZero() const;

  /**
    * @brief Checks if one vector is near the same as another vector based in an error
    * @param Vector other, error (default is LITTLENUMBER)
    * @return true if based on the error is the same
    *
    */
  static bool
  isNearlySame(const Vector3& a, const Vector3& b, const float& error);

  /**
    * @brief Returns the vector as a printable string
    * @param 
    * @return String with the values of the vector
    *
    */
  String
  toString();

  /**
    * Vector3 that has all values 0
    */
  static const Vector3 ZERO;

  /**
    * Vector3 that has all values 1
    */
  static const Vector3 ONE;

  /**
    * Vector3 that has x value 1
    */
  static const Vector3 ONEX;

  /**
    * Vector3 that has y value 1
    */
  static const Vector3 ONEY;

  /**
    * Vector3 that has x value 1
    */
  static const Vector3 ONEZ;

  /**
    * Vector3 that has x value 1
    */
  static const Vector3 RIGHT;

  /**
    * Vector3 with y value 1
    */
  static const Vector3 UP;

  /**
    * Vector3 with z value 1
    */
  static const Vector3 FRONT;

  /**
    * Vector3 that has x value -1
    */
  static const Vector3 LEFT;

  /**
    * Vector3 with y value -1
    */
  static const Vector3 DOWN;

  /**
    * Vector3 with z value -1
    */
  static const Vector3 BACK;

public:

  /**
    * @brief X value of the vector
    */
  float x;

  /**
    * @brief Y value of the vector
    */
  float y;

  /**
    * @brief Z value of the vector
    */
  float z;

};

}

