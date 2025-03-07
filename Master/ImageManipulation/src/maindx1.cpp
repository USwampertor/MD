#include "mdImageManipulationPrerequisites.h"
#include "mdVector3.h"
#include "mdColor.h"
#include "mdGraphicsAPI.h"
// #include "mdVertex.h"


#define SDL_MAIN_USE_CALLBACKS 0
#include <SDL3/SDL.h>
#include <iostream>

using namespace MD;

struct MODEL_VERTEX
{
  Vector3 position;
  Color c;
};


SDL_Window* g_pWindow = nullptr;

UPtr<GraphicsAPI> g_pGraphicsAPI = nullptr;
UPtr<VertexShader> g_pVertexShader = nullptr;
UPtr<PixelShader> g_pPixelShader = nullptr;

ID3D11InputLayout* g_pInputLayout = nullptr;

UPtr<GraphicsBuffer> g_pVertexBuffer = nullptr;
UPtr<GraphicsBuffer> g_pIndexBuffer = nullptr;

int main(int argc, char* argv[]) {


 

  // Initialize SDL3
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return -1;
  }

  // Create an SDL3 window
  SDL_Window* window = SDL_CreateWindow("SDL3 Window", 1280, 720, SDL_WINDOW_RESIZABLE);
  if (!window) {
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return -1;
  }

  auto hwnd = SDL_GetPointerProperty(SDL_GetWindowProperties(window), 
                                     SDL_PROP_WINDOW_WIN32_HWND_POINTER, 
                                     nullptr);

  // GraphicsAPI* api = nullptr;

  if (hwnd) {
    // api = new GraphicsAPI(hwnd);
    
    g_pGraphicsAPI = std::make_unique<GraphicsAPI>(hwnd);
    if (!g_pGraphicsAPI) {
      return -1;
    }
  }

  Path resourceDir = std::filesystem::current_path().string() + "/../resources";

  g_pVertexShader = g_pGraphicsAPI->createVertexShader(Path(resourceDir.string() + "/vertexShader.hlsl"),
                                            "vertex_main");
  g_pPixelShader = g_pGraphicsAPI->createPixelShader(Path(resourceDir.string() + "/pixelShader.hlsl"),
                                          "pixel_main");

  Vector<D3D11_INPUT_ELEMENT_DESC> inputDescs = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
  };

  g_pInputLayout = g_pGraphicsAPI->createInputLayout(inputDescs, g_pVertexShader);

  if (!g_pInputLayout) {
    return -1;
  }

  /*
  MODEL_VERTEX cube[] = {
    {Vector3( 0.0f,  0.5f, 0.0f), Color::RED},     //     o
    {Vector3( 0.5f, -0.5f, 0.0f), Color::GREEN},    //        o
    {Vector3(-0.5f, -0.5f, 0.0f), Color::BLUE}    //  o     
  };
  */

  MODEL_VERTEX cube[] = {
    {Vector3(-1.0f,  1.0f, -1.0f), Color::GREEN},
    {Vector3( 1.0f,  1.0f, -1.0f), Color::GREEN},
    {Vector3( 1.0f,  1.0f,  1.0f), Color::GREEN},
    {Vector3(-1.0f,  1.0f,  1.0f), Color::GREEN},
    {Vector3(-1.0f, -1.0f, -1.0f), Color::GREEN},
    {Vector3( 1.0f, -1.0f, -1.0f), Color::GREEN},
    {Vector3( 1.0f, -1.0f,  1.0f), Color::GREEN},
    {Vector3(-1.0f, -1.0f,  1.0f), Color::GREEN},
    {Vector3(-1.0f, -1.0f,  1.0f), Color::RED},
    {Vector3(-1.0f, -1.0f, -1.0f), Color::RED},
    {Vector3(-1.0f,  1.0f, -1.0f), Color::RED},
    {Vector3(-1.0f,  1.0f,  1.0f), Color::RED},
    {Vector3( 1.0f, -1.0f,  1.0f), Color::RED},
    {Vector3( 1.0f, -1.0f, -1.0f), Color::RED},
    {Vector3( 1.0f,  1.0f, -1.0f), Color::RED},
    {Vector3( 1.0f,  1.0f,  1.0f), Color::RED},
    {Vector3(-1.0f, -1.0f,  1.0f), Color::BLUE},
    {Vector3( 1.0f, -1.0f,  1.0f), Color::BLUE},
    {Vector3( 1.0f,  1.0f,  1.0f), Color::BLUE},
    {Vector3(-1.0f,  1.0f,  1.0f), Color::BLUE},
    {Vector3(-1.0f, -1.0f, -1.0f), Color::BLUE},
    {Vector3( 1.0f, -1.0f, -1.0f), Color::BLUE},
    {Vector3( 1.0f,  1.0f, -1.0f), Color::BLUE},
    {Vector3(-1.0f,  1.0f, -1.0f), Color::BLUE},

  };
  
  Vector<char> vertexData;
  vertexData.resize(sizeof(cube));
  memcpy(vertexData.data(), cube, sizeof(cube));

  g_pVertexBuffer = g_pGraphicsAPI->createVertexBuffer(vertexData);

  if (!g_pVertexBuffer) {
    return -1;
  }

  unsigned short indices[] = {
    3,1,0,    2,1,3,
    6,4,5,    7,4,6,
    11,9,8,   10,9,11,
    14,12,13, 15,12,14,
    19,17,16, 18,17,19,
    22,20,21, 23,20,22
  };

  Vector<char> indexData;
  indexData.resize(sizeof(indices));
  memcpy(indexData.data(), indices, sizeof(indices));

  g_pIndexBuffer = g_pGraphicsAPI->createIndexBuffer(indexData);

  if (!g_pVertexBuffer) {
    return -1;
  }


  // Create an SDL3 renderer
  SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
  if (!renderer) {
    std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
  }


  // Main loop
  bool running = true;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      }
    }

    D3D11_VIEWPORT vp;

    vp.Width = 1280;
    vp.Height = 720;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;

    g_pGraphicsAPI->m_pDeviceContext->RSSetViewports(1, &vp);

    // Fill up input Assembly
    g_pGraphicsAPI->m_pDeviceContext->OMSetRenderTargets(1, 
                                                         &g_pGraphicsAPI->m_pBackBufferRTV, 
                                                         g_pGraphicsAPI->m_pBackBufferDSV);

    float clearColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };

    g_pGraphicsAPI->m_pDeviceContext->ClearRenderTargetView(g_pGraphicsAPI->m_pBackBufferRTV,
                                                            clearColor);

    g_pGraphicsAPI->m_pDeviceContext->ClearDepthStencilView(g_pGraphicsAPI->m_pBackBufferDSV,
                                                            D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 
                                                            1.0f, 
                                                            0);

    g_pGraphicsAPI->m_pDeviceContext->VSSetShader(g_pVertexShader->m_pVertexShader, nullptr, 0);
    g_pGraphicsAPI->m_pDeviceContext->PSSetShader(g_pPixelShader->m_pPixelShader, nullptr, 0);

    g_pGraphicsAPI->m_pDeviceContext->IASetInputLayout(g_pInputLayout);
    g_pGraphicsAPI->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    uint32_t vertexStride = sizeof(MODEL_VERTEX);
    uint32_t vertexOffset = 0;

    g_pGraphicsAPI->m_pDeviceContext->IASetVertexBuffers(0, 
                                                         1, 
                                                         &g_pVertexBuffer->m_pBuffer, 
                                                         &vertexStride, 
                                                         &vertexOffset);

    uint32_t indexStride = sizeof(short);

    g_pGraphicsAPI->m_pDeviceContext->IASetIndexBuffer(g_pIndexBuffer->m_pBuffer, 
                                                       DXGI_FORMAT_R16_UINT, 
                                                       0);

    g_pGraphicsAPI->m_pDeviceContext->DrawIndexed(36, 0, 0);
    g_pGraphicsAPI->m_pSwapChain->Present(0, 0);

    // Present the frame
    // SDL_RenderPresent(renderer);
  }

  // Cleanup
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}