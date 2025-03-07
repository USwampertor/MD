#pragma once
#include "mdImageManipulationPrerequisites.h"


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d11_2.h>
#include <dxgi.h>
#include <d3dcompiler.h>

namespace MD {

class Shader
{
public:
  Shader() = default;
  virtual ~Shader();

  bool 
  compile(const Path& filePath, const String& entry, const String& shaderModel);


  ID3DBlob* getBlob() {
    return m_pBlob;
  }

private:
  ID3DBlob* m_pBlob;
};

class VertexShader : public Shader {
public:
  VertexShader() = default;
  virtual ~VertexShader();


  friend class GraphicsAPI;
public:
  

  ID3D11VertexShader* m_pVertexShader = nullptr;
};


class PixelShader : public Shader {
public:
  PixelShader() = default;
  virtual ~PixelShader();

  friend class GraphicsAPI;
public:


  ID3D11PixelShader* m_pPixelShader = nullptr;
};



}

