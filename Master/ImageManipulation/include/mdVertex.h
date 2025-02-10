#pragma once

#include "mdImageManipulationPrerequisites.h"
#include "mdVector3.h"

namespace MD {

// 
// struct Vertex
// {
//   Vertex() = default;
// 
//   Vertex(const Vector3& newV)
//     : _m(newV) {}
// 
//   Vertex(const float& a, const float& b, const float& c)
//     : _m(a, b, c) {}
// 
//   Vector3 _m;
// 
//   float x() { return _m.x; }
//   float y() { return _m.y; }
//   float z() { return _m.z; }
// };
class Vertex : public Vector3
{
public:

  Vertex(float nx, float ny = 0, float nz = 0, float nu = 0, float nv = 0)
    : Vector3(nx, ny, nz),
      u(nu),
      v(nv) {}

  Vertex(const Vector3& copy, float nu = 0, float nv = 0)
    : Vector3(copy),
      u(nu),
      v(nv) {}

  float u;
  float v;
};

}

