#pragma once
#include "mdImageManipulationPrerequisites.h"
#include "mdVertex.h"
#include "mdColor.h"

namespace MD {

struct Triangle
{
  Triangle() = default;

  Triangle(Vertex a, Vertex b, Vertex c)
    : v0(a),
      v1(b),
      v2(c) {}

  Vertex v0;
  Vertex v1;
  Vertex v2;

  Color c0 = Color::WHITE;
  Color c1 = Color::WHITE;
  Color c2 = Color::WHITE;

  Vertex center = Vertex::ZERO;
};

}
