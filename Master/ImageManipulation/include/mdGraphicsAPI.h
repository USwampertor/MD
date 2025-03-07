#pragma once
#include "mdImageManipulationPrerequisites.h"
#include "mdShader.h"
#include "mdGraphicsBuffer.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d11_2.h>
#include <dxgi.h>

namespace MD {

// 
// class ID3D11Device1;
// class ID3D11DeviceContext1;
// class IDXGISwapChain1;



class GraphicsAPI
{
public:

  GraphicsAPI(void* pWindowHandle);

  virtual ~GraphicsAPI();

  ID3D11Texture2D*
  createTexture(int32_t width, 
                int32_t height, 
                DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, 
                D3D11_USAGE usage = D3D11_USAGE_DEFAULT,
                uint32_t bindFlags = D3D11_BIND_SHADER_RESOURCE, 
                uint32_t cpuAccessFlags = 0, 
                uint32_t mipFlags = 1);


  UPtr<VertexShader>
  createVertexShader(const Path& filePath, 
                     const String& entryFunction);

  UPtr<PixelShader>
  createPixelShader(const Path& filePath, 
                    const String& entryFunction);

  ID3D11InputLayout* 
  createInputLayout(Vector<D3D11_INPUT_ELEMENT_DESC> pInputElementDescs, 
                    const UPtr<VertexShader>& pVertexShader);


  UPtr<GraphicsBuffer>
  createVertexBuffer(const Vector<char>& data);

  UPtr<GraphicsBuffer>
  createIndexBuffer(const Vector<char>& data);


private:

  void
  queryInterface(int32_t width, int32_t height);

public:

  void* 
  m_pWHandle = nullptr;

  ID3D11Device1*
  m_pDevice = nullptr;

  ID3D11DeviceContext1*
  m_pDeviceContext = nullptr;

  IDXGISwapChain1*
  m_pSwapChain = nullptr;

  ID3D11RenderTargetView* 
  m_pBackBufferRTV = nullptr;
  
  ID3D11DepthStencilView*
  m_pBackBufferDSV = nullptr;

  // VertexShader* 
  // m_pVertexShader = nullptr;
  // 
  // PixelShader*
  // m_pPixelShader = nullptr;
  // 
  // ID3D11InputLayout*
  // m_pInputLayout = nullptr;

  // ID3D11Texture2D* 
  // m_pDepthStencilBuffer = nullptr;

};

}

