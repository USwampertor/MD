#include "mdGraphicsAPI.h"

#include <d3dcompiler.h>

namespace MD {



GraphicsAPI::GraphicsAPI(void* pWindowHandle) {

  HWND hwnd = static_cast<HWND>(pWindowHandle);

  RECT rc;
  GetClientRect(hwnd, &rc);

  Vector<IDXGIAdapter1*> vAdapters;
  IDXGIAdapter1* pAdapter = nullptr;
  IDXGIFactory1* pFactory = nullptr;

  CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&pFactory));


  for (uint32_t i = 0; pFactory->EnumAdapters1(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i) {
    DXGI_ADAPTER_DESC1 desc;
    pAdapter->GetDesc1(&desc);
    vAdapters.push_back(pAdapter);
  }

  //   if (pAdapter) {
  //     pAdapter->Release();
  //     pAdapter = nullptr;
  //   }

  SAFE_RELEASE(pAdapter);
  SAFE_RELEASE(pFactory);

  Vector<D3D_FEATURE_LEVEL> vFeatureLevels = { D3D_FEATURE_LEVEL_11_1,
                                               D3D_FEATURE_LEVEL_11_0,
                                               D3D_FEATURE_LEVEL_10_1,
                                               D3D_FEATURE_LEVEL_10_0,
                                               D3D_FEATURE_LEVEL_9_3, };

  D3D_FEATURE_LEVEL selectedFeatureLevel;


  ID3D11Device* pDevice = nullptr;
  ID3D11DeviceContext* pDeviceContext = nullptr;

  uint32_t deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
  deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  HRESULT hr =
  D3D11CreateDevice(vAdapters[0],
                    D3D_DRIVER_TYPE_UNKNOWN,
                    nullptr, 
                    deviceFlags, 
                    vFeatureLevels.data(), 
                    vFeatureLevels.size(), 
                    D3D11_SDK_VERSION,
                    &pDevice, 
                    &selectedFeatureLevel, 
                    &pDeviceContext);


  if (FAILED(hr)) {
    MessageBox(hwnd, L"Failed to create device", L"Error", MB_OK);
    return;
  }

  // ID3D11Device1* pDevice1 = nullptr;
  // ID3D11DeviceContext1* pDeviceContext1 = nullptr;


  // Making an extra cause I'm saving them as agnostic objects
  pDevice->QueryInterface(__uuidof(ID3D11Device1), (void**)&m_pDevice);
  pDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), (void**)&m_pDeviceContext);

  // m_pDevice = reinterpret_cast<void*>(pDevice1);
  // m_pDeviceContext = reinterpret_cast<void*>(pDeviceContext1);

  // SAFE_RELEASE(pDeviceContext1);
  // SAFE_RELEASE(pDevice1);

  SAFE_RELEASE(pDeviceContext);
  SAFE_RELEASE(pDevice);

  DXGI_SWAP_CHAIN_DESC1 scDesc;
  memset(&scDesc, 0, sizeof(scDesc));

  scDesc.Width = rc.right;
  scDesc.Height = rc.bottom;
  // This is because we are using colors as float
  scDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // DXGI_FORMAT_R32G32B32A32_FLOAT;
  scDesc.Stereo = false;
  scDesc.SampleDesc.Count = 1; // MSAA
  scDesc.SampleDesc.Quality = 0;
  scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  scDesc.BufferCount = 2;
  scDesc.Scaling = DXGI_SCALING_NONE;
  scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

  IDXGIDevice1* pDXGIDevice = nullptr;

  m_pDevice->QueryInterface(__uuidof(IDXGIDevice1), (void**)&pDXGIDevice);

  IDXGIAdapter* pDXGIAdapter = nullptr;
  pDXGIDevice->GetAdapter(&pDXGIAdapter);

  IDXGIFactory2* pFactory2 = nullptr;
  pDXGIAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&pFactory2);


  // IDXGISwapChain1* pSwapChain = nullptr;
  hr =
  pFactory2->CreateSwapChainForHwnd(m_pDevice,
                                    hwnd,
                                    &scDesc,
                                    nullptr,
                                    nullptr,
                                    &m_pSwapChain);

  pDXGIDevice->SetMaximumFrameLatency(3);

  if (FAILED(hr)) {
    MessageBox(hwnd, L"Failed to create swap chain", L"Error", MB_OK);
    return;
  }
  queryInterface(scDesc.Width, scDesc.Height);

  D3D11_VIEWPORT vp;

  vp.Width = static_cast<float>(scDesc.Width);
  vp.Height = static_cast<float>(scDesc.Height);
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  m_pDeviceContext->RSSetViewports(1, &vp);

  Path resourceDir = std::filesystem::current_path().string() + "/../resources";

  // m_pVertexShader = createVertexShader(Path(resourceDir.string() + "/vertexShader.hlsl"), 
  //                                      "vertex_main");
  // m_pPixelShader = createPixelShader(Path(resourceDir.string() + "/pixelShader.hlsl"), 
  //                                    "pixel_main");
}


GraphicsAPI::~GraphicsAPI() {
  SAFE_RELEASE(m_pBackBufferDSV);
  SAFE_RELEASE(m_pBackBufferRTV);
  SAFE_RELEASE(m_pSwapChain);
  SAFE_RELEASE(m_pDeviceContext);
  SAFE_RELEASE(m_pDevice);
}


UPtr<VertexShader>
GraphicsAPI::createVertexShader(const Path& filePath,
                                const String& entryFunction) {
  
  UPtr<VertexShader> pVertexShader = std::make_unique<VertexShader>();

  if (!pVertexShader->compile(filePath, entryFunction, "vs_5_0")) {
    return nullptr;
  }

  HRESULT hr = m_pDevice->CreateVertexShader(pVertexShader->getBlob()->GetBufferPointer(),
                                     pVertexShader->getBlob()->GetBufferSize(), 
                                     nullptr, 
                                     &pVertexShader->m_pVertexShader);

  
  if (FAILED(hr)) {
    MessageBox(nullptr, L"Error creating Vertex Shader", L"Error", MB_OK);
    return nullptr;
  }


  return pVertexShader;
}

UPtr<PixelShader>
GraphicsAPI::createPixelShader(const Path& filePath,
                               const String& entryFunction) {
  UPtr<PixelShader> pPixelShader = std::make_unique<PixelShader>();

  if (!pPixelShader->compile(filePath, entryFunction, "ps_5_0")) {
    return nullptr;
  }

  HRESULT hr = m_pDevice->CreatePixelShader(pPixelShader->getBlob()->GetBufferPointer(),
                                            pPixelShader->getBlob()->GetBufferSize(),
                                            nullptr,
                                            &pPixelShader->m_pPixelShader);

  if (FAILED(hr)) {
    MessageBox(nullptr, L"Error creating Pixel Shader", L"Error", MB_OK);
    return nullptr;
  }

  return pPixelShader;
}

ID3D11InputLayout* 
GraphicsAPI::createInputLayout(Vector<D3D11_INPUT_ELEMENT_DESC> pInputElementDescs, 
                               const UPtr<VertexShader>& pVertexShader) {
  
  
  ID3D11InputLayout* pInputLayout = nullptr;

  if (pInputElementDescs.empty()) { // || pVertexShader.expired()) {
    return nullptr;
  }

  // auto pShader = pVertexShader.lock();

  HRESULT hr = m_pDevice->CreateInputLayout(pInputElementDescs.data(),
                                            pInputElementDescs.size(),
                                /*pShader*/ pVertexShader->getBlob()->GetBufferPointer(),
                                /*pShader*/ pVertexShader->getBlob()->GetBufferSize(),
                                            &pInputLayout);


  if (FAILED(hr)) {
    MessageBox(nullptr, L"Error creating Input layout", L"Error", MB_OK);
    return nullptr;
  }

  return pInputLayout;
}

UPtr<GraphicsBuffer>
GraphicsAPI::createVertexBuffer(const Vector<char>& data) {
  UPtr<GraphicsBuffer> pBuffer = std::make_unique<GraphicsBuffer>();
  D3D11_BUFFER_DESC desc;
  memset(&desc, 0, sizeof(desc));

  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.ByteWidth = data.size();
  desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  desc.CPUAccessFlags = 0;
  desc.MiscFlags = 0;

  // As we have already data on it, we can use this object, if we dont have data, we can set nullptr
  D3D11_SUBRESOURCE_DATA initData;
  initData.pSysMem = data.data();
  initData.SysMemPitch = 0;
  initData.SysMemSlicePitch = 0;

  HRESULT hr = m_pDevice->CreateBuffer(&desc, &initData, &pBuffer->m_pBuffer);


  if (FAILED(hr)) {
    MessageBox(nullptr, L"Error creating vertex buffer", L"Error", MB_OK);
    return nullptr;
  }

  return pBuffer;
}

UPtr<GraphicsBuffer>
GraphicsAPI::createIndexBuffer(const Vector<char>& data) {
  UPtr<GraphicsBuffer> pBuffer = std::make_unique<GraphicsBuffer>();

  D3D11_BUFFER_DESC desc;
  memset(&desc, 0, sizeof(desc));
  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.ByteWidth = data.size();
  desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  desc.CPUAccessFlags = 0;
  desc.MiscFlags = 0;

  // As we have already data on it, we can use this object, if we dont have data, we can set nullptr
  D3D11_SUBRESOURCE_DATA initData;
  initData.pSysMem = data.data();
  initData.SysMemPitch = 0;
  initData.SysMemSlicePitch = 0;

  HRESULT hr = m_pDevice->CreateBuffer(&desc, &initData, &pBuffer->m_pBuffer);


  if (FAILED(hr)) {
    MessageBox(nullptr, L"Error creating vertex buffer", L"Error", MB_OK);
    return nullptr;
  }

  return pBuffer;
}



UPtr<GraphicsBuffer>
GraphicsAPI::createConstantBuffer(const Vector<char>& data) {
  UPtr<GraphicsBuffer> pBuffer = std::make_unique<GraphicsBuffer>();

  D3D11_BUFFER_DESC desc;
  memset(&desc, 0, sizeof(desc));
  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.ByteWidth = data.size();
  desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  desc.CPUAccessFlags = 0;
  desc.MiscFlags = 0;

  // As we have already data on it, we can use this object, if we dont have data, we can set nullptr
  D3D11_SUBRESOURCE_DATA initData;
  initData.pSysMem = data.data();
  initData.SysMemPitch = 0;
  initData.SysMemSlicePitch = 0;

  HRESULT hr = m_pDevice->CreateBuffer(&desc, &initData, &pBuffer->m_pBuffer);


  if (FAILED(hr)) {
    MessageBox(nullptr, L"Error creating vertex buffer", L"Error", MB_OK);
    return nullptr;
  }

  return pBuffer;
}


void
GraphicsAPI::writeToBuffer(const UPtr<GraphicsBuffer>& pBuffer, const Vector<char>& data) {
  // D3D11_MAPPED_SUBRESOURCE mappedResource;
  // m_pDeviceContext->Map(pBuffer->m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  // memcpy(mappedResource.pData, data.data(), data.size());
  // m_pDeviceContext->Unmap(pBuffer->m_pBuffer, 0);
  m_pDeviceContext->UpdateSubresource1(pBuffer->m_pBuffer, 0, nullptr, data.data(), 0, 0, 0);
}

void
GraphicsAPI::queryInterface(int32_t width, int32_t height) {
  if (!m_pSwapChain) {
    return;
  }

  ID3D11Texture2D* pBackBuffer = nullptr;
  m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
  // assert(pBackBuffer);
  m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pBackBufferRTV);

  SAFE_RELEASE(pBackBuffer);


  ID3D11Texture2D* pDepthStencil = nullptr;
  pDepthStencil = createTexture(width, 
                                height,
                                DXGI_FORMAT_D24_UNORM_S8_UINT,
                                D3D11_USAGE_DEFAULT,
                                D3D11_BIND_DEPTH_STENCIL);

  if (!pDepthStencil) {
    MessageBox(nullptr, L"Failed to create depth stencil", L"Error", MB_OK);
    return;
  }

  m_pDevice->CreateDepthStencilView(pDepthStencil, nullptr, &m_pBackBufferDSV);

  SAFE_RELEASE(pDepthStencil);

}


ID3D11Texture2D*
GraphicsAPI::createTexture(int32_t width, 
                           int32_t height, 
                           DXGI_FORMAT format /* = DXGI_FORMAT_R8G8B8A8_UNORM */, 
                           D3D11_USAGE usage /* = D3D11_USAGE_DEFAULT */, 
                           uint32_t bindFlags /* = D3D11_BIND_SHADER_RESOURCE */, 
                           uint32_t cpuAccessFlags /* = 0 */, 
                           uint32_t mipFlags /* = 1 */,
                           ID3D11ShaderResourceView** ppSRV,
                           ID3D11RenderTargetView** ppRTV,
                           ID3D11DepthStencilView** ppDSV /*= nullptr*/) {

  ID3D11Texture2D* pTexture = nullptr;
  D3D11_TEXTURE2D_DESC desc;
  
  memset(&pTexture, 0, sizeof(pTexture));
  
  desc.ArraySize = 1;
  desc.BindFlags = bindFlags;
  desc.CPUAccessFlags = cpuAccessFlags;
  desc.Format = format;
  desc.Height = height;
  desc.Width = width;
  desc.MipLevels = mipFlags;
  desc.MiscFlags = 0;
  
  desc.SampleDesc.Count = 1; //MSAA
  desc.SampleDesc.Quality = 0;

  desc.Usage = usage;

  if (FAILED(m_pDevice->CreateTexture2D(&desc, nullptr, &pTexture))) {
    return nullptr;
  }

  if (ppSRV != nullptr) {
    if (bindFlags & D3D11_BIND_SHADER_RESOURCE) {

      D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = CD3D11_SHADER_RESOURCE_VIEW_DESC();
      srvDesc.Format = format;
      srvDesc.Texture2D.MipLevels = mipFlags == 1 ? 1 : -1;
      srvDesc.Texture2D.MostDetailedMip = 0;
      srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
      m_pDevice->CreateShaderResourceView(pTexture, &srvDesc, ppSRV);


    }
  }

  if (ppSRV != nullptr) {
    if (bindFlags & D3D11_BIND_RENDER_TARGET) {

      D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = CD3D11_RENDER_TARGET_VIEW_DESC();
      rtvDesc.Format = format;
      rtvDesc.Texture2D.MipSlice = 0;
      rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
      m_pDevice->CreateRenderTargetView(pTexture, &rtvDesc, ppRTV);


    }
  }

  if (ppDSV != nullptr) {
    if (bindFlags & D3D11_BIND_DEPTH_STENCIL) {

      D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = CD3D11_DEPTH_STENCIL_VIEW_DESC();
      dsvDesc.Format = format;
      dsvDesc.Texture2D.MipSlice = 0;
      dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
      m_pDevice->CreateDepthStencilView(pTexture, &dsvDesc, ppDSV);


    }
  }

  return pTexture;


}

}
