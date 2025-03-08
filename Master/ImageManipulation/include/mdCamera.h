#pragma once
#include "mdImageManipulationPrerequisites.h"
#include "mdMatrix4.h"
#include "mdVector3.h"

namespace MD {


class Camera
{
public:

  void
  setLookAt(const Vector3& eyePos, const Vector3& targetPos, const Vector3& upDir) {
    position = eyePos;
    target = targetPos;
    up = upDir;
    view.lookAt(position, target, up);
  }

  void
    setPerspective(const float newWidth,
                   const float newHeight,
                   const float newZNear,
                   const float newZFar,
                   const float newFOV) {
    fov = newFOV;
    width = newWidth;
    height = newHeight;
    zNear = newZNear;
    zFar = newZFar;
    projection = projection.perspective(width, height, zNear, zFar, fov);
  }

  Vector3 position;
  Vector3 target;
  Vector3 up;

  float fov;
  float width;
  float height;
  float zNear;
  float zFar;

  Matrix4 view;
  Matrix4 projection;

};

}

