#pragma once

#include "mdImageManipulationPrerequisites.h"
#include "mdVector3.h"
#include "mdColor.h"
namespace MD {

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
  Color c;
  float u;
  float v;
};

}

