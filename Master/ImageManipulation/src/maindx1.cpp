#include "mdImageManipulationPrerequisites.h"
#include "mdVector3.h"
#include "mdColor.h"
#include "mdGraphicsAPI.h"
#include "mdCamera.h"
#include "mdModel.h"
#include "mdTexture.h"

// #include "mdVertex.h"


#define SDL_MAIN_USE_CALLBACKS 0
#include <SDL3/SDL.h>
#include <iostream>

using namespace MD;

// struct MODEL_VERTEX
// {
//   Vector3 position;
//   Color c;
// };


struct GameObject
{
  GameObject() = default;
  GameObject(const GameObject& other)
    : position(other.position),
      rotation(other.rotation),
      scale(other.scale),
      speed(other.speed),
      direction(other.direction) {}

  Matrix4 position;
  Matrix4 rotation;
  Matrix4 scale;
  float speed;
  uint32_t direction;
};

SDL_Window* g_pWindow = nullptr;

Camera* g_pCamera = nullptr;

UPtr<GraphicsAPI> g_pGraphicsAPI = nullptr;
UPtr<VertexShader> g_pVertexShader = nullptr;
UPtr<PixelShader> g_pPixelShader = nullptr;
UPtr<PixelShader> g_pReflectPShader = nullptr;

ID3D11InputLayout* g_pInputLayout = nullptr;

ID3D11RasterizerState1* g_pRS_Default = nullptr;
ID3D11RasterizerState1* g_pRS_Wireframe = nullptr;
ID3D11RasterizerState1* g_pRS_WireframeNoCull = nullptr;
ID3D11RasterizerState1* g_pRS_CullFront = nullptr;

ID3D11SamplerState* g_pSS_Point = nullptr;
ID3D11SamplerState* g_pSS_Linear = nullptr;
ID3D11SamplerState* g_pSS_Anisotropic = nullptr;

UPtr<GraphicsBuffer> g_pVertexBuffer = nullptr;
UPtr<GraphicsBuffer> g_pIndexBuffer = nullptr;
UPtr<GraphicsBuffer> g_pConstantBuffer = nullptr;

int main(int argc, char* argv[]) {

  struct  MatrixCollection
  {
    Matrix4 world;
    Matrix4 view;
    Matrix4 projection;
  } matrices;


  // Initialize SDL3
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return -1;
  }

  // Create an SDL3 window
  /*SDL_Window* window*/
  g_pWindow = SDL_CreateWindow("SDL3 Window", 1280, 720, SDL_WINDOW_RESIZABLE);
  if (!g_pWindow) {
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return -1;
  }

  auto hwnd = SDL_GetPointerProperty(SDL_GetWindowProperties(g_pWindow),
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

  g_pReflectPShader = g_pGraphicsAPI->createPixelShader(Path(resourceDir.string() + "/reflectionPShader.hlsl"),
                                          "pixel_main");

  Vector<D3D11_INPUT_ELEMENT_DESC> inputDescs = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
  };

  g_pInputLayout = g_pGraphicsAPI->createInputLayout(inputDescs, g_pVertexShader);

  if (!g_pInputLayout) {
    return -1;
  }

  float pi = 3.141592653;
  g_pCamera = new Camera();
  g_pCamera->setLookAt(Vector3(0, 0, -30), Vector3(0, 0, 0), Vector3(0, 1, 0));
  g_pCamera->setPerspective(1280, 720, 0.1f, 200.0f, pi * 0.25f);

  matrices.world.identity();
  matrices.view = g_pCamera->view;
  matrices.projection = g_pCamera->projection;

  matrices.world.transpose();
  matrices.view.transpose();
  matrices.projection.transpose();

  Vector<char> matrixData;
  matrixData.resize(sizeof(matrices));
  memcpy(matrixData.data(), &matrices, sizeof(matrices));

  g_pConstantBuffer = g_pGraphicsAPI->createConstantBuffer(matrixData);

  Model newObject;
  newObject.loadFromFile(Path(resourceDir.string() + "/rex_norm.obj"), g_pGraphicsAPI);

  Model floorObject;
  floorObject.loadFromFile(Path(resourceDir.string() + "/disc.obj"), g_pGraphicsAPI);

  Image newImage;
  newImage.decode(Path(resourceDir.string() + "/rex_tex.bmp"));
  Texture newTexture;
  newTexture.setImage(newImage, g_pGraphicsAPI);

  Image floorImage;
  floorImage.decode(Path(resourceDir.string() + "/terrain.bmp"));
  Texture floorTexture;
  floorTexture.setImage(floorImage, g_pGraphicsAPI);

  Vector<GameObject> gameObjects;
  for (int i = 0; i < 10; ++i) {
    GameObject go;
    go.rotation = Matrix4::IDENTITY;
    go.scale = Matrix4::IDENTITY;
    go.position = Matrix4::IDENTITY;
    // go.rotation.rotateX(Utils::getRandom(360));
    // go.rotation.rotateY(Utils::getRandom(360));
    // go.rotation.rotateZ(Utils::getRandom(360));

    float a = static_cast<float>(Utils::getRandom(50, -25));
    float b = static_cast<float>(Utils::getRandom(50, -25));
    float c = static_cast<float>(Utils::getRandom(50, -25));

    go.position.translate(Vector3(a, b, c));

    float s = static_cast<float>(Utils::getRandom(100) * 0.001f);

    go.scale.scale(Vector3(s, s, s));

    float sp = static_cast<float>(Utils::getRandom(10, -5));
    if (std::abs(sp) < 0.5f) { sp += 1.0f; }
    go.speed = sp;

    uint32_t di = static_cast<uint32_t>(Utils::getRandom(7, 1));
    go.direction = di;
    
    gameObjects.push_back(go);
  }

  Texture rtReflection;
  rtReflection.m_pTexture = g_pGraphicsAPI->createTexture(1280, 
                                                          720, 
                                                          DXGI_FORMAT_R32G32B32A32_FLOAT, 
                                                          D3D11_USAGE_DEFAULT, 
                                                          D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, 
                                                          0, 
                                                          1, 
                                                          &rtReflection.m_pSRV,
                                                          &rtReflection.m_pRTV);

  Texture dsReflection;
  dsReflection.m_pTexture = g_pGraphicsAPI->createTexture(1280, 
                                                          720, 
                                                          DXGI_FORMAT_D24_UNORM_S8_UINT, 
                                                          D3D11_USAGE_DEFAULT, 
                                                          D3D11_BIND_DEPTH_STENCIL, 
                                                          0, 
                                                          1, 
                                                          nullptr,
                                                          nullptr,
                                                          &dsReflection.m_pDSV);




  CD3D11_RASTERIZER_DESC1 descRD(D3D11_DEFAULT);
  
  g_pGraphicsAPI->m_pDevice->CreateRasterizerState1(&descRD, &g_pRS_Default);

  descRD.FillMode = D3D11_FILL_WIREFRAME;
  g_pGraphicsAPI->m_pDevice->CreateRasterizerState1(&descRD, &g_pRS_Wireframe);

  descRD.FillMode = D3D11_FILL_WIREFRAME;
  descRD.CullMode = D3D11_CULL_NONE;
  g_pGraphicsAPI->m_pDevice->CreateRasterizerState1(&descRD, &g_pRS_WireframeNoCull);

  descRD = CD3D11_RASTERIZER_DESC1(D3D11_DEFAULT);
  descRD.CullMode = D3D11_CULL_FRONT;
  g_pGraphicsAPI->m_pDevice->CreateRasterizerState1(&descRD, &g_pRS_CullFront);


  CD3D11_SAMPLER_DESC descSS(D3D11_DEFAULT);
  descSS.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
  descSS.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
  descSS.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
  g_pGraphicsAPI->m_pDevice->CreateSamplerState(&descSS, &g_pSS_Point);

  descSS.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  g_pGraphicsAPI->m_pDevice->CreateSamplerState(&descSS, &g_pSS_Linear);

  descSS.Filter = D3D11_FILTER_ANISOTROPIC;
  descSS.MaxAnisotropy = 16;
  g_pGraphicsAPI->m_pDevice->CreateSamplerState(&descSS, &g_pSS_Anisotropic);


//   // Create an SDL3 renderer
//   SDL_Renderer* renderer = SDL_CreateRenderer(g_pWindow, nullptr);
//   if (!renderer) {
//     std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
//     SDL_DestroyWindow(g_pWindow);
//     SDL_Quit();
//     return -1;
//   }

  // Main loop
  bool running = true;
  float cameraRotationAngle = 0.0f;

  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      }
      
      Vector3 camPos = Vector3(0, 0, 0);

      if (event.type == SDL_EVENT_KEY_DOWN) {
        
        if (event.key.key == SDLK_1) {
          auto pVertexShader = g_pGraphicsAPI->createVertexShader(Path(resourceDir.string() + "/vertexShader.hlsl"),
                                                                  "vertex_main");
          if (pVertexShader) {
            g_pVertexShader = std::move(pVertexShader);
          }
          auto pPixelShader = g_pGraphicsAPI->createPixelShader(Path(resourceDir.string() + "/pixelShader.hlsl"),
                                                                "pixel_main");
          if (pVertexShader) {
            g_pPixelShader = std::move(pPixelShader);
          }
          auto pReflectPShader = g_pGraphicsAPI->createPixelShader(Path(resourceDir.string() + "/reflectionPShader.hlsl"),
                                                                    "pixel_main");
          if (pReflectPShader) {
            g_pReflectPShader = std::move(pReflectPShader);
          }
        }

        if (event.key.key == SDLK_R) {
          g_pCamera->setLookAt(Vector3(0, 0, -30), Vector3(0, 0, 0), Vector3(0, 1, 0));

          matrices.view = g_pCamera->view;
          matrices.projection = g_pCamera->projection;
          matrices.view.transpose();
          matrices.projection.transpose();
        }
        
        if (event.key.key == SDLK_W) {
          // g_pCamera->setLookAt(g_pCamera->position + Vector3(0,0,0.1), g_pCamera->target + Vector3(0, 0, 0.1), Vector3(0, 1, 0));
          camPos = -g_pCamera->view.getForwardVector() * 0.1f;

          matrices.view = g_pCamera->view;
          matrices.projection = g_pCamera->projection;
          matrices.view.transpose();
          matrices.projection.transpose();
        }
        if (event.key.key == SDLK_S) {
          // g_pCamera->setLookAt(g_pCamera->position - Vector3(0, 0, 0.1), g_pCamera->target - Vector3(0, 0, 0.1), Vector3(0, 1, 0));
          camPos = g_pCamera->view.getForwardVector() * 0.1f;

          matrices.view = g_pCamera->view;
          matrices.projection = g_pCamera->projection;
          matrices.view.transpose();
          matrices.projection.transpose();
        }

        if (event.key.key == SDLK_Q) {
          // g_pCamera->setLookAt(g_pCamera->position + Vector3(0, 0.1, 0), g_pCamera->target + Vector3(0, 0.1, 0), Vector3(0, 1, 0));
          camPos = g_pCamera->view.getUpVector() * 0.1f;

          matrices.view = g_pCamera->view;
          matrices.projection = g_pCamera->projection;
          matrices.view.transpose();
          matrices.projection.transpose();
        }
        if (event.key.key == SDLK_E) {
          // g_pCamera->setLookAt(g_pCamera->position - Vector3(0, 0.1, 0), g_pCamera->target - Vector3(0, 0.1, 0), Vector3(0, 1, 0));
          camPos = -g_pCamera->view.getUpVector() * 0.1f;

          matrices.view = g_pCamera->view;
          matrices.projection = g_pCamera->projection;
          matrices.view.transpose();
          matrices.projection.transpose();
        }

        if (event.key.key == SDLK_A) {
          // g_pCamera->setLookAt(g_pCamera->position + Vector3(0, 0.1, 0), g_pCamera->target + Vector3(0, 0.1, 0), Vector3(0, 1, 0));
          cameraRotationAngle += .1;

          matrices.view = g_pCamera->view;
          matrices.projection = g_pCamera->projection;
          matrices.view.transpose();
          matrices.projection.transpose();
        }
        if (event.key.key == SDLK_D) {
          // g_pCamera->setLookAt(g_pCamera->position - Vector3(0, 0.1, 0), g_pCamera->target - Vector3(0, 0.1, 0), Vector3(0, 1, 0));
          cameraRotationAngle -= .1;

          matrices.view = g_pCamera->view;
          matrices.projection = g_pCamera->projection;
          matrices.view.transpose();
          matrices.projection.transpose();
        }
        g_pCamera->view.rotateY(cameraRotationAngle);
        g_pCamera->view.translate(camPos);


        matrixData.resize(sizeof(matrices));
        memcpy(matrixData.data(), &matrices, sizeof(matrices));
        g_pGraphicsAPI->writeToBuffer(g_pConstantBuffer, matrixData);
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
    float blackColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

    Color cColor = Color::GRAY;
    Color bColor = Color::CLEAR;

    g_pGraphicsAPI->m_pDeviceContext->ClearRenderTargetView(g_pGraphicsAPI->m_pBackBufferRTV,
                                                            clearColor);

    g_pGraphicsAPI->m_pDeviceContext->ClearRenderTargetView(rtReflection.m_pRTV,
                                                            blackColor);

    g_pGraphicsAPI->m_pDeviceContext->ClearDepthStencilView(g_pGraphicsAPI->m_pBackBufferDSV,
                                                            D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 
                                                            1.0f, 
                                                            0);
    g_pGraphicsAPI->m_pDeviceContext->ClearDepthStencilView(dsReflection.m_pDSV,
                                                            D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 
                                                            1.0f, 
                                                            0);

    g_pGraphicsAPI->m_pDeviceContext->VSSetShader(g_pVertexShader->m_pVertexShader, nullptr, 0);
    g_pGraphicsAPI->m_pDeviceContext->PSSetShader(g_pPixelShader->m_pPixelShader, nullptr, 0);

    uint32_t vertexStride = sizeof(MODEL_VERTEX);
    uint32_t indexStride = sizeof(unsigned short);
    uint32_t vertexOffset = 0;

    g_pGraphicsAPI->m_pDeviceContext->IASetInputLayout(g_pInputLayout);
    g_pGraphicsAPI->m_pDeviceContext->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(newObject.m_meshes[0].topology));
    g_pGraphicsAPI->m_pDeviceContext->IASetVertexBuffers(0, 
                                                         1, 
                                                         &newObject.m_pVertexBuffer->m_pBuffer, 
                                                         &vertexStride, 
                                                         &vertexOffset);


    g_pGraphicsAPI->m_pDeviceContext->IASetIndexBuffer(newObject.m_pIndexBuffer->m_pBuffer,
                                                       DXGI_FORMAT_R16_UINT, 
                                                       0);

    g_pGraphicsAPI->m_pDeviceContext->PSSetShaderResources(0, 1, &newTexture.m_pSRV);
    g_pGraphicsAPI->m_pDeviceContext->PSSetSamplers(0, 1, &g_pSS_Point);
    g_pGraphicsAPI->m_pDeviceContext->PSSetSamplers(1, 1, &g_pSS_Linear);
    g_pGraphicsAPI->m_pDeviceContext->PSSetSamplers(2, 1, &g_pSS_Anisotropic);

    static float rotationAngle = 0.0f;
    rotationAngle += 0.0001f;

    Matrix4 pos0 = Matrix4::IDENTITY;
    pos0.translate(Vector3(0, 0, 0));
    Matrix4 pos1 = Matrix4::IDENTITY;
    pos1.translate(Vector3(0, 3, 0));
    Matrix4 pos2 = Matrix4::IDENTITY;
    pos2.translate(Vector3(0, -3, 0));

    Matrix4 rot0 = Matrix4::IDENTITY;
    rot0.rotateY(rotationAngle);
    Matrix4 rot1 = Matrix4::IDENTITY;
    rot1.rotateY(-rotationAngle);

    Matrix4 potScale = Matrix4::IDENTITY;
    potScale.scale(Vector3(1, 1, 1));

    //////////////////////////////////////////////////////////////////////////

    matrices.world = potScale * rot0 * pos1;
    matrices.world.transpose();
    matrixData.clear();
    matrixData.resize(sizeof(matrices));
    memcpy(matrixData.data(), &matrices, sizeof(matrices));

    g_pGraphicsAPI->writeToBuffer(g_pConstantBuffer, matrixData);
    g_pGraphicsAPI->m_pDeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer->m_pBuffer);

    // Draw first object
    g_pGraphicsAPI->m_pDeviceContext->DrawIndexed(newObject.m_meshes[0].numIndices, 
                                                  newObject.m_meshes[0].baseIndex, 
                                                  newObject.m_meshes[0].baseVertex);



    

    //////////////////////////////////////////////////////////////////////////

    for (int i = 0; i < gameObjects.size(); ++i) {
     
      if (gameObjects[i].direction & (1 << 0)) {
        gameObjects[i].rotation.rotateX(rotationAngle * gameObjects[i].speed);
      }
      if (gameObjects[i].direction & (1 << 1)) {
        gameObjects[i].rotation.rotateY(rotationAngle * gameObjects[i].speed);
      }
      // if (gameObjects[i].direction & (1 << 2)) {
      //   gameObjects[i].rotation.rotateZ(rotationAngle * gameObjects[i].speed);
      // }
      matrices.world = gameObjects[i].scale * gameObjects[i].rotation * gameObjects[i].position;
      matrices.world.transpose();
      matrixData.clear();
      matrixData.resize(sizeof(matrices));
      memcpy(matrixData.data(), &matrices, sizeof(matrices));
      
      g_pGraphicsAPI->writeToBuffer(g_pConstantBuffer, matrixData);
      g_pGraphicsAPI->m_pDeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer->m_pBuffer);
      
      // Draw first object
      g_pGraphicsAPI->m_pDeviceContext->DrawIndexed(newObject.m_meshes[0].numIndices, 
                                                    newObject.m_meshes[0].baseIndex, 
                                                    newObject.m_meshes[0].baseVertex);
    }


    //////////////////////////////////////////////////////////////////////////

    // Modify data so model is reflected

    Matrix4 invertedScale = Matrix4::IDENTITY;
    invertedScale.scale(Vector3(1, -1, 1));

    matrices.world = invertedScale * rot0 * pos2;
    matrices.world.transpose();
    matrixData.clear();
    matrixData.resize(sizeof(matrices));
    memcpy(matrixData.data(), &matrices, sizeof(matrices));

    g_pGraphicsAPI->writeToBuffer(g_pConstantBuffer, matrixData);

    // Change render target and depth stencil to store it

    ID3D11ShaderResourceView* nullSRV = nullptr;
    g_pGraphicsAPI->m_pDeviceContext->PSSetShaderResources(1, 1, &nullSRV);

    g_pGraphicsAPI->m_pDeviceContext->RSSetState(g_pRS_CullFront);
    g_pGraphicsAPI->m_pDeviceContext->OMSetRenderTargets(1, 
                                                         &rtReflection.m_pRTV, 
                                                         dsReflection.m_pDSV);
    // Draw second same object but we are going to use it for reflection
    g_pGraphicsAPI->m_pDeviceContext->DrawIndexed(newObject.m_meshes[0].numIndices, 
                                                  newObject.m_meshes[0].baseIndex, 
                                                  newObject.m_meshes[0].baseVertex);

    //////////////////////////////////////////////////////////////////////////

    // Reset everything to draw next object
    
    
    // We want to draw again on our backbuffer
    
    g_pGraphicsAPI->m_pDeviceContext->OMSetRenderTargets(1, 
                                                         &g_pGraphicsAPI->m_pBackBufferRTV, 
                                                         g_pGraphicsAPI->m_pBackBufferDSV);

    g_pGraphicsAPI->m_pDeviceContext->PSSetShader(g_pReflectPShader->m_pPixelShader, nullptr, 0);

    g_pGraphicsAPI->m_pDeviceContext->RSSetState(g_pRS_Default);
    g_pGraphicsAPI->m_pDeviceContext->PSSetShaderResources(0, 1, &floorTexture.m_pSRV);
    g_pGraphicsAPI->m_pDeviceContext->PSSetShaderResources(1, 1, &rtReflection.m_pSRV);

    g_pGraphicsAPI->m_pDeviceContext->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(floorObject.m_meshes[0].topology));
    g_pGraphicsAPI->m_pDeviceContext->IASetVertexBuffers(0,
                                                         1,
                                                         &floorObject.m_pVertexBuffer->m_pBuffer,
                                                         &vertexStride,
                                                         &vertexOffset);
    g_pGraphicsAPI->m_pDeviceContext->IASetIndexBuffer(floorObject.m_pIndexBuffer->m_pBuffer,
                                                       DXGI_FORMAT_R16_UINT,
                                                       0);
    Matrix4 floorScale = Matrix4::IDENTITY;
    floorScale.scale(Vector3(.1, .1, .1));
    matrices.world = floorScale * rot1 * pos0;
    matrices.world.transpose();
    matrixData.clear();
    matrixData.resize(sizeof(matrices));
    memcpy(matrixData.data(), &matrices, sizeof(matrices));

    g_pGraphicsAPI->writeToBuffer(g_pConstantBuffer, matrixData);
    g_pGraphicsAPI->m_pDeviceContext->DrawIndexed(floorObject.m_meshes[0].numIndices,
                                                  floorObject.m_meshes[0].baseIndex, 
                                                  floorObject.m_meshes[0].baseVertex);


    

    g_pGraphicsAPI->m_pSwapChain->Present(0, 0);

    // Present the frame
    // SDL_RenderPresent(renderer);
  }

  // Cleanup
  // SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(g_pWindow);
  SDL_Quit();

  return 0;
}