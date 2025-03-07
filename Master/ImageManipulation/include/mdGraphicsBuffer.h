#pragma once
#include "mdImageManipulationPrerequisites.h"

#include <d3d11_2.h>

namespace MD {

class GraphicsBuffer
{
public:

  GraphicsBuffer() = default;

  virtual ~GraphicsBuffer();

  friend class GraphicsAPI;
public:

  ID3D11Buffer* m_pBuffer = nullptr;
};

}

