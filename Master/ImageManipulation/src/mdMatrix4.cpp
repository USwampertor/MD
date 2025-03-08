#include "mdMatrix4.h"

namespace MD {
  Matrix4::Matrix4(const float& value) {
    memset(this, static_cast<uint32_t>(value), sizeof(Matrix4));
  }

  Matrix4::Matrix4(const Matrix4& other)
    : _m(other._m) {}

  Matrix4::Matrix4(const float& v00, const float& v01, const float& v02, const float v03,
                   const float& v10, const float& v11, const float& v12, const float v13,
                   const float& v20, const float& v21, const float& v22, const float v23,
                   const float& v30, const float& v31, const float& v32, const float v33) {
    _m.m00 = v00; _m.m01 = v01; _m.m02 = v02; _m.m03 = v03;
    _m.m10 = v10; _m.m11 = v11; _m.m12 = v12; _m.m13 = v13;
    _m.m20 = v20; _m.m21 = v21; _m.m22 = v22; _m.m23 = v23;
    _m.m30 = v30; _m.m31 = v31; _m.m32 = v32; _m.m33 = v33;
  }

  Matrix4
  Matrix4::operator+(const Matrix4& b) {
    Matrix4 temp;
    for (uint32_t i = 0; i < 4; ++i) {
      for (uint32_t j = 0; j < 4; ++j) {
        temp.m[i][j] = m[i][j] + b.m[i][j];
      }
    }
    return temp;

  }

  Matrix4
  Matrix4::operator-(const Matrix4& b) {
    Matrix4 temp;
    for (uint32_t i = 0; i < 4; ++i) {
      for (uint32_t j = 0; j < 4; ++j) {
        temp.m[i][j] = m[i][j] - b.m[i][j];
      }
    }
    return temp;
  }

  Matrix4
  Matrix4::operator*(const Matrix4& b) {
    Matrix4 temp = ZERO;
    for (uint32_t i = 0; i < 4; ++i) {
      for (uint32_t j = 0; j < 4; ++j) {
        for (uint32_t k = 0; k < 4; ++k) {
          temp.m[i][j] += m[i][k] * b.m[k][j];
        }
      }
    }
    return temp;
  }

  Matrix4&
  Matrix4::operator+=(const Matrix4& b) {
    for (uint32_t i = 0; i < 4; ++i) {
      for (uint32_t j = 0; j < 4; ++j) {
        m[i][j] += b.m[i][j];
      }
    }
    return *this;
  }

  Matrix4&
  Matrix4::operator-=(const Matrix4& b) {
    for (uint32_t i = 0; i < 4; ++i) {
      for (uint32_t j = 0; j < 4; ++j) {
        m[i][j] -= b.m[i][j];
      }
    }
    return *this;
  }

  Matrix4&
  Matrix4::operator*=(const Matrix4& b) {
    Matrix4 temp = ZERO;
    for (uint32_t i = 0; i < 4; ++i) {
      for (uint32_t j = 0; j < 4; ++j) {
        for (uint32_t k = 0; k < 4; ++k) {
          temp.m[i][j] += m[i][k] * b.m[k][j];
        }
      }
    }
    *this = temp;
    return *this;
  }

  Matrix4&
  Matrix4::operator+=(const float& value) {
    for (uint32_t i = 0; i < 4; ++i) {
      for (uint32_t j = 0; j < 4; ++j) {
        m[i][j] += value;
      }
    }
    return *this;
  }

  Matrix4&
  Matrix4::operator-=(const float& value) {
    for (uint32_t i = 0; i < 4; ++i) {
      for (uint32_t j = 0; j < 4; ++j) {
        m[i][j] -= value;
      }
    }
    return *this;
  }

  Matrix4&
  Matrix4::operator*=(const float& value) {
    for (uint32_t i = 0; i < 4; ++i) {
      for (uint32_t j = 0; j < 4; ++j) {
        m[i][j] *= value;
      }
    }
    return *this;
  }

  Matrix4&
  Matrix4::operator/=(const float& value) {
    for (uint32_t i = 0; i < 4; ++i) {
      for (uint32_t j = 0; j < 4; ++j) {
        m[i][j] /= value;
      }
    }
    return *this;
  }

  bool
  Matrix4::operator==(const Matrix4& b) {
    for (uint32_t i = 0; i < 4; ++i) {
      for (uint32_t j = 0; j < 4; ++j) {
        if (m[i][j] != b.m[i][j]) { return false; }
      }
    }
    return true;
  }

  void
  Matrix4::lookAt(const Vector3& eyePos, const Vector3& targetPos, const Vector3& upDir) {
    const Vector3 zAxis = (eyePos - targetPos).normalized();
    const Vector3 xAxis = Vector3::cross(upDir, zAxis).normalized();
    const Vector3 yAxis = Vector3::cross(zAxis, xAxis).normalized();

    m[0][0] = xAxis.x;
    m[1][0] = xAxis.y;
    m[2][0] = xAxis.z;
    
    // m[3][0] = xAxis.x;

    m[0][1] = yAxis.x;
    m[1][1] = yAxis.y;
    m[2][1] = yAxis.z;
    
    // m[3][1] = yAxis.x;

    m[0][2] = zAxis.x;
    m[1][2] = zAxis.y;
    m[2][2] = zAxis.z;
    
    // m[3][2] = zAxis.x;

    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
    
    // m[3][3] = 0.0f;

    Vector3 eyeNeg = -eyePos;

    m[3][0] = eyePos | xAxis;
    m[3][1] = eyePos | yAxis;
    m[3][2] = eyePos | zAxis;
    m[3][3] = 1.0f;
  }


  void
  Matrix4::identity() {
    *this = IDENTITY;
  }

  void
  Matrix4::zero() {
    *this = ZERO;
  }

  Matrix4
  Matrix4::transposed() const {
    return Matrix4(m[0][0], m[1][0], m[2][0], m[3][0],
                     m[0][1], m[1][1], m[2][1], m[3][1],
                     m[0][2], m[1][2], m[2][2], m[3][2],
                     m[0][3], m[1][3], m[2][3], m[3][3]);
  }

  void
  Matrix4::transpose() {
    *this = this->transposed();
  }

  Matrix4
  Matrix4::cofactored() const {
    Matrix4 temp;

    temp.m[0][0] = m[1][1]*m[2][2]*m[3][3] + m[1][2]*m[2][3]*m[3][1] +
                   m[1][3]*m[2][1]*m[3][2] - m[1][1]*m[2][3]*m[3][2] -
                   m[1][2]*m[2][1]*m[3][3] - m[1][3]*m[2][2]*m[3][1];
    temp.m[1][0] = m[0][1]*m[2][3]*m[3][2] + m[0][2]*m[2][1]*m[3][3] +
                   m[0][3]*m[2][2]*m[3][1] - m[0][1]*m[2][2]*m[3][3] -
                   m[0][2]*m[2][3]*m[3][1] - m[0][3]*m[2][1]*m[3][2];
    temp.m[2][0] = m[0][1]*m[1][2]*m[3][3] + m[0][2]*m[1][3]*m[3][1] +
                   m[0][3]*m[1][1]*m[3][2] - m[0][1]*m[1][3]*m[3][2] -
                   m[0][2]*m[1][1]*m[3][3] - m[0][3]*m[1][2]*m[3][1];
    temp.m[3][0] = m[0][1]*m[1][3]*m[2][2] + m[0][2]*m[1][1]*m[2][3] +
                   m[0][3]*m[1][2]*m[2][1] - m[0][1]*m[1][2]*m[2][3] -
                   m[0][2]*m[1][3]*m[2][1] - m[0][3]*m[1][1]*m[2][2];
    temp.m[0][1] = m[1][0]*m[2][3]*m[3][2] + m[1][2]*m[2][0]*m[3][3] +
                   m[1][3]*m[2][2]*m[3][0] - m[1][0]*m[2][2]*m[3][3] -
                   m[1][2]*m[2][3]*m[3][0] - m[1][3]*m[2][0]*m[3][2];
    temp.m[1][1] = m[0][0]*m[2][2]*m[3][3] + m[0][2]*m[2][3]*m[3][0] +
                   m[0][3]*m[2][0]*m[3][2] - m[0][0]*m[2][3]*m[3][2] -
                   m[0][2]*m[2][0]*m[3][3] - m[0][3]*m[2][2]*m[3][0];
    temp.m[2][1] = m[0][0]*m[1][3]*m[3][2] + m[0][2]*m[1][0]*m[3][3] +
                   m[0][3]*m[1][2]*m[3][0] - m[0][0]*m[1][2]*m[3][3] -
                   m[0][2]*m[1][3]*m[3][0] - m[0][3]*m[1][0]*m[3][2];
    temp.m[3][1] = m[0][0]*m[1][2]*m[2][3] + m[0][2]*m[1][3]*m[2][0] +
                   m[0][3]*m[1][0]*m[2][2] - m[0][0]*m[1][3]*m[2][2] -
                   m[0][2]*m[1][0]*m[2][3] - m[0][3]*m[1][2]*m[2][0];
    temp.m[0][2] = m[1][0]*m[2][1]*m[3][3] + m[1][1]*m[2][3]*m[3][0] +
                   m[1][3]*m[2][0]*m[3][1] - m[1][0]*m[2][3]*m[3][1] -
                   m[1][1]*m[2][0]*m[3][3] - m[1][3]*m[2][1]*m[3][0];
    temp.m[1][2] = m[0][0]*m[2][3]*m[3][1] + m[0][1]*m[2][0]*m[3][3] +
                   m[0][3]*m[2][1]*m[3][0] - m[0][0]*m[2][1]*m[3][3] -
                   m[0][1]*m[2][3]*m[3][0] - m[0][3]*m[2][0]*m[3][1];
    temp.m[2][2] = m[0][0]*m[1][1]*m[3][3] + m[0][1]*m[1][3]*m[3][0] +
                   m[0][3]*m[1][0]*m[3][1] - m[0][0]*m[1][3]*m[3][1] -
                   m[0][1]*m[1][0]*m[3][3] - m[0][3]*m[1][1]*m[3][0];
    temp.m[3][2] = m[0][0]*m[1][3]*m[2][1] + m[0][1]*m[1][0]*m[2][3] +
                   m[0][3]*m[1][1]*m[2][0] - m[0][0]*m[1][1]*m[2][3] -
                   m[0][1]*m[1][3]*m[2][0] - m[0][3]*m[1][0]*m[2][1];
    temp.m[0][3] = m[1][0]*m[2][2]*m[3][1] + m[1][1]*m[2][0]*m[3][2] +
                   m[1][2]*m[2][1]*m[3][0] - m[1][0]*m[2][1]*m[3][2] -
                   m[1][1]*m[2][2]*m[3][0] - m[1][2]*m[2][0]*m[3][1];
    temp.m[1][3] = m[0][0]*m[2][1]*m[3][2] + m[0][1]*m[2][2]*m[3][0] +
                   m[0][2]*m[2][0]*m[3][1] - m[0][0]*m[2][2]*m[3][1] -
                   m[0][1]*m[2][0]*m[3][2] - m[0][2]*m[2][1]*m[3][0];
    temp.m[2][3] = m[0][0]*m[1][2]*m[3][1] + m[0][1]*m[1][0]*m[3][2] +
                   m[0][2]*m[1][1]*m[3][0] - m[0][0]*m[1][1]*m[3][2] -
                   m[0][1]*m[1][2]*m[3][0] - m[0][2]*m[1][0]*m[3][1];
    temp.m[3][3] = m[0][0]*m[1][1]*m[2][2] + m[0][1]*m[1][2]*m[2][0] +
                   m[0][2]*m[1][0]*m[2][1] - m[0][0]*m[1][2]*m[2][1] -
                   m[0][1]*m[1][0]*m[2][2] - m[0][2]*m[1][1]*m[2][0];
    return temp;
  }

  void
  Matrix4::cofactor() {
    *this = this->cofactored();
  }

  void
  Matrix4::setValues(const float& value) {
    m[0][0] = m[0][1] = m[0][2] = m[0][3] = 
    m[1][0] = m[1][1] = m[1][2] = m[1][3] =
    m[2][0] = m[2][1] = m[2][2] = m[2][3] =
    m[3][0] = m[3][1] = m[3][2] = m[3][3] = value;
  }

  void
  Matrix4::setValues(const float& v00, const float& v01, const float& v02, const float v03,
                       const float& v10, const float& v11, const float& v12, const float v13,
                       const float& v20, const float& v21, const float& v22, const float v23,
                       const float& v30, const float& v31, const float& v32, const float v33) {
    _m.m00 = v00; _m.m01 = v01; _m.m02 = v02; _m.m03 = v03;
    _m.m10 = v10; _m.m11 = v11; _m.m12 = v12; _m.m13 = v13;
    _m.m20 = v20; _m.m21 = v21; _m.m22 = v22; _m.m23 = v23;
    _m.m30 = v30; _m.m31 = v31; _m.m32 = v32; _m.m33 = v33;
  }

  Matrix4
  Matrix4::inversed() {
    
    Matrix4 temp = *this;
    temp.cofactor();
    temp.transpose();

    temp *= std::pow(this->determinant(), -1.0f);
    return temp;
    // temp.m[0][0] = m[1][1]*m[2][2]*m[3][3] + m[1][2]*m[2][3]*m[3][1] +
    //                m[1][3]*m[2][1]*m[3][2] - m[1][1]*m[2][3]*m[3][2] -
    //                m[1][2]*m[2][1]*m[3][3] - m[1][3]*m[2][2]*m[3][1];
    // temp.m[1][0] = m[0][1]*m[2][3]*m[3][2] + m[0][2]*m[2][1]*m[3][3] +
    //                m[0][3]*m[2][2]*m[3][1] - m[0][1]*m[2][2]*m[3][3] -
    //                m[0][2]*m[2][3]*m[3][1] - m[0][3]*m[2][1]*m[3][2];
    // temp.m[2][0] = m[0][1]*m[1][2]*m[3][3] + m[0][2]*m[1][3]*m[3][1] +
    //                m[0][3]*m[1][1]*m[3][2] - m[0][1]*m[1][3]*m[3][2] -
    //                m[0][2]*m[1][1]*m[3][3] - m[0][3]*m[1][2]*m[3][1];
    // temp.m[3][0] = m[0][1]*m[1][3]*m[2][2] + m[0][2]*m[1][1]*m[2][3] +
    //                m[0][3]*m[1][2]*m[2][1] - m[0][1]*m[1][2]*m[2][3] -
    //                m[0][2]*m[1][3]*m[2][1] - m[0][3]*m[1][1]*m[2][2];
    // temp.m[0][1] = m[1][0]*m[2][3]*m[3][2] + m[1][2]*m[2][0]*m[3][3] +
    //                m[1][3]*m[2][2]*m[3][0] - m[1][0]*m[2][2]*m[3][3] -
    //                m[1][2]*m[2][3]*m[3][0] - m[1][3]*m[2][0]*m[3][2];
    // temp.m[1][1] = m[0][0]*m[2][2]*m[3][3] + m[0][2]*m[2][3]*m[3][0] +
    //                m[0][3]*m[2][0]*m[3][2] - m[0][0]*m[2][3]*m[3][2] -
    //                m[0][2]*m[2][0]*m[3][3] - m[0][3]*m[2][2]*m[3][0];
    // temp.m[2][1] = m[0][0]*m[1][3]*m[3][2] + m[0][2]*m[1][0]*m[3][3] +
    //                m[0][3]*m[1][2]*m[3][0] - m[0][0]*m[1][2]*m[3][3] -
    //                m[0][2]*m[1][3]*m[3][0] - m[0][3]*m[1][0]*m[3][2];
    // temp.m[3][1] = m[0][0]*m[1][2]*m[2][3] + m[0][2]*m[1][3]*m[2][0] +
    //                m[0][3]*m[1][0]*m[2][2] - m[0][0]*m[1][3]*m[2][2] -
    //                m[0][2]*m[1][0]*m[2][3] - m[0][3]*m[1][2]*m[2][0];
    // temp.m[0][2] = m[1][0]*m[2][1]*m[3][3] + m[1][1]*m[2][3]*m[3][0] +
    //                m[1][3]*m[2][0]*m[3][1] - m[1][0]*m[2][3]*m[3][1] -
    //                m[1][1]*m[2][0]*m[3][3] - m[1][3]*m[2][1]*m[3][0];
    // temp.m[1][2] = m[0][0]*m[2][3]*m[3][1] + m[0][1]*m[2][0]*m[3][3] +
    //                m[0][3]*m[2][1]*m[3][0] - m[0][0]*m[2][1]*m[3][3] -
    //                m[0][1]*m[2][3]*m[3][0] - m[0][3]*m[2][0]*m[3][1];
    // temp.m[2][2] = m[0][0]*m[1][1]*m[3][3] + m[0][1]*m[1][3]*m[3][0] +
    //                m[0][3]*m[1][0]*m[3][1] - m[0][0]*m[1][3]*m[3][1] -
    //                m[0][1]*m[1][0]*m[3][3] - m[0][3]*m[1][1]*m[3][0];
    // temp.m[3][2] = m[0][0]*m[1][3]*m[2][1] + m[0][1]*m[1][0]*m[2][3] +
    //                m[0][3]*m[1][1]*m[2][0] - m[0][0]*m[1][1]*m[2][3] -
    //                m[0][1]*m[1][3]*m[2][0] - m[0][3]*m[1][0]*m[2][1];
    // temp.m[0][3] = m[1][0]*m[2][2]*m[3][1] + m[1][1]*m[2][0]*m[3][2] +
    //                m[1][2]*m[2][1]*m[3][0] - m[1][0]*m[2][1]*m[3][2] -
    //                m[1][1]*m[2][2]*m[3][0] - m[1][2]*m[2][0]*m[3][1];
    // temp.m[1][3] = m[0][0]*m[2][1]*m[3][2] + m[0][1]*m[2][2]*m[3][0] +
    //                m[0][2]*m[2][0]*m[3][1] - m[0][0]*m[2][2]*m[3][1] -
    //                m[0][1]*m[2][0]*m[3][2] - m[0][2]*m[2][1]*m[3][0];
    // temp.m[2][3] = m[0][0]*m[1][2]*m[3][1] + m[0][1]*m[1][0]*m[3][2] +
    //                m[0][2]*m[1][1]*m[3][0] - m[0][0]*m[1][1]*m[3][2] -
    //                m[0][1]*m[1][2]*m[3][0] - m[0][2]*m[1][0]*m[3][1];
    // temp.m[3][3] = m[0][0]*m[1][1]*m[2][2] + m[0][1]*m[1][2]*m[2][0] +
    //                m[0][2]*m[1][0]*m[2][1] - m[0][0]*m[1][2]*m[2][1] -
    //                m[0][1]*m[1][0]*m[2][2] - m[0][2]*m[1][1]*m[2][0];

  }

  void
  Matrix4::inverse() {
    *this = this->inversed();
    // CY_ASSERT(this->determinant() != 0.0f &&
    //           Utils::format("The determinant for matrix \n%s is 0!", this->toString()).c_str());
    // Matrix3x3 temp(0);
    // Matrix4 tmp2 = *this;
    // 
    // temp.m[0][0] = m[1][1]; temp.m[0][1] = m[1][2]; temp.m[0][2] = m[1][3];
    // temp.m[1][0] = m[2][1]; temp.m[1][1] = m[2][2]; temp.m[1][2] = m[2][3];
    // temp.m[2][0] = m[3][1]; temp.m[2][1] = m[3][2]; temp.m[2][2] = m[3][3];
    // tmp2.m[0][0] = temp.determinant();
    // 
    // temp.m[0][0] = m[1][0]; temp.m[0][1] = m[1][2]; temp.m[0][2] = m[1][3];
    // temp.m[1][0] = m[2][0]; temp.m[1][1] = m[2][2]; temp.m[1][2] = m[2][3];
    // temp.m[2][0] = m[3][0]; temp.m[2][1] = m[3][2]; temp.m[2][2] = m[3][3];
    // tmp2.m[1][0] = temp.determinant();
    // 
    // temp.m[0][0] = m[1][0]; temp.m[0][1] = m[1][1]; temp.m[0][2] = m[1][3];
    // temp.m[1][0] = m[2][0]; temp.m[1][1] = m[2][1]; temp.m[1][2] = m[2][3];
    // temp.m[2][0] = m[3][0]; temp.m[2][1] = m[3][1]; temp.m[2][2] = m[3][3];
    // tmp2.m[2][0] = temp.determinant();
    // 
    // temp.m[0][0] = m[1][0]; temp.m[0][1] = m[1][1]; temp.m[0][2] = m[1][2];
    // temp.m[1][0] = m[2][0]; temp.m[1][1] = m[2][1]; temp.m[1][2] = m[2][2];
    // temp.m[2][0] = m[3][0]; temp.m[2][1] = m[3][1]; temp.m[2][2] = m[3][2];
    // tmp2.m[3][0] = temp.determinant();
    // 
    // temp.m[0][0] = m[0][1]; temp.m[0][1] = m[0][2]; temp.m[0][2] = m[0][3];
    // temp.m[1][0] = m[2][1]; temp.m[1][1] = m[2][2]; temp.m[1][2] = m[2][3];
    // temp.m[2][0] = m[3][1]; temp.m[2][1] = m[3][2]; temp.m[2][2] = m[3][3];
    // tmp2.m[0][1] = temp.determinant();
    // 
    // temp.m[0][0] = m[0][0]; temp.m[0][1] = m[0][2]; temp.m[0][2] = m[0][3];
    // temp.m[1][0] = m[2][0]; temp.m[1][1] = m[2][2]; temp.m[1][2] = m[2][3];
    // temp.m[2][0] = m[3][0]; temp.m[2][1] = m[3][2]; temp.m[2][2] = m[3][3];
    // tmp2.m[1][1] = temp.determinant();
    // 
    // temp.m[0][0] = m[0][0]; temp.m[0][1] = m[0][1]; temp.m[0][2] = m[0][3];
    // temp.m[1][0] = m[2][0]; temp.m[1][1] = m[2][1]; temp.m[1][2] = m[2][3];
    // temp.m[2][0] = m[3][0]; temp.m[2][1] = m[3][1]; temp.m[2][2] = m[3][3];
    // tmp2.m[2][1] = temp.determinant();
    // 
    // temp.m[0][0] = m[0][0]; temp.m[0][1] = m[0][1]; temp.m[0][2] = m[0][2];
    // temp.m[1][0] = m[2][0]; temp.m[1][1] = m[2][1]; temp.m[1][2] = m[2][2];
    // temp.m[2][0] = m[3][0]; temp.m[2][1] = m[3][1]; temp.m[2][2] = m[3][2];
    // tmp2.m[3][1] = temp.determinant();
    // 
    // temp.m[0][0] = m[0][1]; temp.m[0][1] = m[0][2]; temp.m[0][2] = m[0][3];
    // temp.m[1][0] = m[1][1]; temp.m[1][1] = m[1][2]; temp.m[1][2] = m[1][3];
    // temp.m[2][0] = m[3][1]; temp.m[2][1] = m[3][2]; temp.m[2][2] = m[3][3];
    // tmp2.m[0][2] = temp.determinant();
    // 
    // temp.m[0][0] = m[0][0]; temp.m[0][1] = m[0][2]; temp.m[0][2] = m[0][3];
    // temp.m[1][0] = m[1][0]; temp.m[1][1] = m[1][2]; temp.m[1][2] = m[1][3];
    // temp.m[2][0] = m[3][0]; temp.m[2][1] = m[3][2]; temp.m[2][2] = m[3][3];
    // tmp2.m[1][2] = temp.determinant();
    // 
    // temp.m[0][0] = m[0][0]; temp.m[0][1] = m[0][1]; temp.m[0][2] = m[0][3];
    // temp.m[1][0] = m[1][0]; temp.m[1][1] = m[1][1]; temp.m[1][2] = m[1][3];
    // temp.m[2][0] = m[3][0]; temp.m[2][1] = m[3][1]; temp.m[2][2] = m[3][3];
    // tmp2.m[2][2] = temp.determinant();
    // 
    // temp.m[0][0] = m[0][0]; temp.m[0][1] = m[0][1]; temp.m[0][2] = m[0][2];
    // temp.m[1][0] = m[1][0]; temp.m[1][1] = m[1][1]; temp.m[1][2] = m[1][2];
    // temp.m[2][0] = m[3][0]; temp.m[2][1] = m[3][1]; temp.m[2][2] = m[3][2];
    // tmp2.m[3][2] = temp.determinant();
    // 
    // temp.m[0][0] = m[0][1]; temp.m[0][1] = m[0][2]; temp.m[0][2] = m[0][3];
    // temp.m[1][0] = m[1][1]; temp.m[1][1] = m[1][2]; temp.m[1][2] = m[1][3];
    // temp.m[2][0] = m[2][1]; temp.m[2][1] = m[2][2]; temp.m[2][2] = m[2][3];
    // tmp2.m[0][3] = temp.determinant();
    // 
    // temp.m[0][0] = m[0][0]; temp.m[0][1] = m[0][2]; temp.m[0][2] = m[0][3];
    // temp.m[1][0] = m[1][0]; temp.m[1][1] = m[1][2]; temp.m[1][2] = m[1][3];
    // temp.m[2][0] = m[2][0]; temp.m[2][1] = m[2][2]; temp.m[2][2] = m[2][3];
    // tmp2.m[1][3] = temp.determinant();
    // 
    // temp.m[0][0] = m[0][0]; temp.m[0][1] = m[0][1]; temp.m[0][2] = m[0][3];
    // temp.m[1][0] = m[1][0]; temp.m[1][1] = m[1][1]; temp.m[1][2] = m[1][3];
    // temp.m[2][0] = m[2][0]; temp.m[2][1] = m[2][1]; temp.m[2][2] = m[2][3];
    // tmp2.m[2][3] = temp.determinant();
    // 
    // temp.m[0][0] = m[0][0]; temp.m[0][1] = m[0][1]; temp.m[0][2] = m[0][2];
    // temp.m[1][0] = m[1][0]; temp.m[1][1] = m[1][1]; temp.m[1][2] = m[1][2];
    // temp.m[2][0] = m[2][0]; temp.m[2][1] = m[2][1]; temp.m[2][2] = m[2][2];
    // tmp2.m[3][3] = temp.determinant();
    // 
    // *this = tmp2;
    // *this *= (1 / this->determinant());
  }

  const float
  Matrix4::determinant() const {
    float a;
    a = (m[0][3] * m[1][2] * m[2][1] * m[3][0]) - (m[0][2] * m[1][3] * m[2][1] * m[3][0]) -
        (m[0][3] * m[1][1] * m[2][2] * m[3][0]) + (m[0][1] * m[1][3] * m[2][2] * m[3][0]) +
        (m[0][2] * m[1][1] * m[2][3] * m[3][0]) - (m[0][1] * m[1][2] * m[2][3] * m[3][0]) -
        (m[0][3] * m[1][2] * m[2][0] * m[3][1]) + (m[0][2] * m[1][3] * m[2][0] * m[3][1]) +
        (m[0][3] * m[1][0] * m[2][2] * m[3][1]) - (m[0][0] * m[1][3] * m[2][2] * m[3][1]) -
        (m[0][2] * m[1][0] * m[2][3] * m[3][1]) + (m[0][0] * m[1][2] * m[2][3] * m[3][1]) +
        (m[0][3] * m[1][1] * m[2][0] * m[3][2]) - (m[0][1] * m[1][3] * m[2][0] * m[3][2]) -
        (m[0][3] * m[1][0] * m[2][1] * m[3][2]) + (m[0][0] * m[1][3] * m[2][1] * m[3][2]) +
        (m[0][1] * m[1][0] * m[2][3] * m[3][2]) - (m[0][0] * m[1][1] * m[2][3] * m[3][2]) -
        (m[0][2] * m[1][1] * m[2][0] * m[3][3]) + (m[0][1] * m[1][2] * m[2][0] * m[3][3]) +
        (m[0][2] * m[1][0] * m[2][1] * m[3][3]) - (m[0][0] * m[1][2] * m[2][1] * m[3][3]) -
        (m[0][1] * m[1][0] * m[2][2] * m[3][3]) + (m[0][0] * m[1][1] * m[2][2] * m[3][3]);
    return a;
  }
  
  Matrix4&
  Matrix4::perspective(const float width,
                       const float height,
                       const float zNear,
                       const float zFar,
                       const float FOV) {
    float halfFOV = FOV * 0.5;
    float plane0[4] = { 1.0f / std::tanf(halfFOV), 0.0f,                                0.0f,                           0.0f };
    float plane1[4] = { 0.0f,                      width / std::tanf(halfFOV) / height, 0.0f,                           0.0f };
    float plane2[4] = { 0.0f,                      0.0f,                                zFar / (zFar - zNear),          1.0f };
    float plane3[4] = { 0.0f,                      0.0f,                                -zNear * zFar / (zFar - zNear), 0.0f };

  
  
    Matrix4 tmp;
    
    tmp.m[0][0] = plane0[0];
    tmp.m[0][1] = plane0[1];
    tmp.m[0][2] = plane0[2];
    tmp.m[0][3] = plane0[3];

    tmp.m[1][0] = plane1[0];
    tmp.m[1][1] = plane1[1];
    tmp.m[1][2] = plane1[2];
    tmp.m[1][3] = plane1[3];

    tmp.m[2][0] = plane2[0];
    tmp.m[2][1] = plane2[1];
    tmp.m[2][2] = plane2[2];
    tmp.m[2][3] = plane2[3];

    tmp.m[3][0] = plane3[0];
    tmp.m[3][1] = plane3[1];
    tmp.m[3][2] = plane3[2];
    tmp.m[3][3] = plane3[3];

    return tmp;
  }


  void
  Matrix4::translate(const Vector3& translation) {
    m[3][0] += translation.x;
    m[3][1] += translation.y;
    m[3][2] += translation.z;
  }

  void
  Matrix4::rotateX(const float& angle) {

  }

  void
  Matrix4::rotateY(const float& angle) {
//     float c = std::cosf(angle);
//     float s = std::sinf(angle);
// 
//     float m00 = m[0][0];
//     float m02 = m[0][2];
//     float m10 = m[1][0];
//     float m12 = m[1][2];
//     float m20 = m[2][0];
//     float m22 = m[2][2];
// 
//     m[0][0] = m00 * c + m[2][0] * s;
//     m[0][2] = m02 * -s + m[2][2] * c;
//     m[1][0] = m10 * c + m[2][0] * s;
//     m[1][2] = m12 * -s + m[2][2] * c;
//     m[2][0] = m20 * c + m[2][0] * s;
//     m[2][2] = m22 * -s + m[2][2] * c;
    identity();

    m[0][0] = cos(angle);
    m[0][2] = sin(angle);
    m[2][0] = -sin(angle);
    m[2][2] = cos(angle);

  }

  void
  Matrix4::rotateZ(const float& angle) {
    
  }

  Vector3
  Matrix4::transformPosition(const Vector3& v) const {
    return Vector3(v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + 1.0f * m[3][0],
                   v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + 1.0f * m[3][1],
                   v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + 1.0f * m[3][2]);
  }

  Vector3
  Matrix4::transformDirection(const Vector3& v) const {
    return Vector3(v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + 0.0f * m[3][0],
                   v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + 0.0f * m[3][1],
                   v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + 0.0f * m[3][2]);
  }
  
  String
  Matrix4::toString() {
    return Utils::format("[(%2.2f), \t (%2.2f), \t (%2.2f), \t (%2.2f)]\n[(%2.2f), \t (%2.2f), \t (%2.2f), \t (%2.2f)]\n[(%2.2f), \t (%2.2f), \t (%2.2f), \t (%2.2f)]\n[(%2.2f), \t (%2.2f), \t (%2.2f), \t (%2.2f)]\n", 
                         m[0][0], m[0][1], m[0][2], m[0][3],
                         m[1][0], m[1][1], m[1][2], m[1][3],
                         m[2][0], m[2][1], m[2][2], m[2][3],
                         m[3][0], m[3][1], m[3][2], m[3][3]);
  }

  const Matrix4 Matrix4::ZERO     = Matrix4(0.0f);

  const Matrix4 Matrix4::IDENTITY = Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
                                            0.0f, 1.0f, 0.0f, 0.0f,
                                            0.0f, 0.0f, 1.0f, 0.0f,
                                            0.0f, 0.0f, 0.0f, 1.0f);
}