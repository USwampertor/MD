#pragma once
#include "mdImageManipulationPrerequisites.h"
#include "mdVector3.h"

namespace MD {

class Matrix4
{
  public:
  Matrix4() = default;

  ~Matrix4() = default;

  Matrix4(const float& value);

  Matrix4(const Matrix4& other);

  Matrix4(const float& v00, const float& v01, const float& v02, const float v03,
          const float& v10, const float& v11, const float& v12, const float v13,
          const float& v20, const float& v21, const float& v22, const float v23,
          const float& v30, const float& v31, const float& v32, const float v33);

  /**
    * @brief + operator overload
    * @param b the other matrix to add
    * @return the sum of *this and b
    *
    */
  Matrix4
  operator+(const Matrix4& b);

  /**
    * @brief - operator overload
    * @param b the other matrix to subtract
    * @return the difference of *this and b
    *
    */
  Matrix4
  operator-(const Matrix4& b);

  /**
    * @brief * operator overload
    * @param b the other matrix to multiply
    * @return the multiplication of *this and b
    *
    */
  Matrix4
  operator*(const Matrix4& b);

  /**
    * @brief += operator overload
    * @param b the other matrix to add
    * @return *this summed b
    *
    */
  Matrix4&
  operator+=(const Matrix4& b);

  /**
    * @brief -= operator overload
    * @param b the other matrix to subtract
    * @return *this minus b
    *
    */
  Matrix4&
  operator-=(const Matrix4& b);

  /**
    * @brief *= operator overload
    * @param b the other matrix to multiply
    * @return *this summed b
    *
    */
  Matrix4&
  operator*=(const Matrix4& b);

  /**
    * @brief += operator overload
    * @param value to add to the matrix
    * @return naumatrix equal to *this summed value
    *
    */
  Matrix4&
  operator+=(const float& value);

  /**
    * @brief -= operator overload
    * @param value to substract to the matrix
    * @return naumatrix equal to *this minus value
    *
    */
  Matrix4&
  operator-=(const float& value);

  /**
    * @brief *= operator overload
    * @param value to multiply the matrix
    * @return naumatrix equal to *this times value
    *
    */
  Matrix4&
  operator*=(const float& value);

  /**
    * @brief /= operator overload
    * @param value to divide to the matrix
    * @return naumatrix equal to *this folded value
    *
    */
  Matrix4&
  operator/=(const float& value);

  /**
    * @brief == operator overload
    * @param nauMatrix to compare
    * @return true if *this is equal to b
    *
    */
  bool
  operator==(const Matrix4& b);

  void
  lookAt(const Vector3& eyePos, const Vector3& targetPos, const Vector3& upDir);

  void
  identity();

  void
  zero();

  Matrix4
  transposed() const;

  void
  transpose();

  Matrix4
  cofactored() const;

  void
  cofactor();

  void
  setValues(const float& value);

  void
  setValues(const float& v00, const float& v01, const float& v02, const float v03,
            const float& v10, const float& v11, const float& v12, const float v13,
            const float& v20, const float& v21, const float& v22, const float v23,
            const float& v30, const float& v31, const float& v32, const float v33);

  Matrix4
  inversed();

  void
  inverse();

  const float
  determinant() const;

  Matrix4&
  orthogonal(const float Width,
              const float Height,
              const float ZNear,
              const float ZFar);

  Matrix4&
  perspective(const float Width,
              const float Height,
              const float ZNear,
              const float ZFar,
              const float FOV);

  void
  translate(const Vector3& translation);

  void
  rotateX(const float& angle);

  void
  rotateY(const float& angle);

  void
  rotateZ(const float& angle);

  void
  scale(const Vector3& scale);

  void
  scale(const float& scale);

  Vector3
  transformPosition(const Vector3& v) const;

  Vector3
  transformDirection(const Vector3& v) const;

  Vector3
  getForwardVector() const;
  
  Vector3
  getRightVector() const;

  Vector3
  getUpVector() const;

  String
  toString();

  /**
    * ZERO filled Matrix
    */
  static const Matrix4 ZERO;

  /**
    * IDENTITY matrix
    */
  static const Matrix4 IDENTITY;

public:

  union {
    /**
      * Row major based matrix struct
      */
    struct {
      float m00, m01, m02, m03;
      float m10, m11, m12, m13;
      float m20, m21, m22, m23;
      float m30, m31, m32, m33;
    }_m;
    float m[4][4];
    float fVec[16];
  };

};

}

/*

Row major directx
Colum major opengl

W V P

World = s x r x t = Row    major
World = t x r x s = Column major


x y z w


*/