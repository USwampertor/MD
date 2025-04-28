#pragma once
#include "mdImageManipulationPrerequisites.h"
#include "mdImage.h"
#include "mdGraphicsAPI.h"

#include <d3d11_2.h>
#if defined(__WIN32__) || defined(_WIN32)
# undef min
# undef max
# undef isinf
# undef isnan
#endif

namespace MD
{

BETTER_ENUM(eSamplerFilter, 
            uint32_t, 
            POINT, 
            LINEAR, 
            BILINEAL, 
            TRILINEAL);

class Texture
{
public:
  Texture() = default;
  ~Texture();

  void 
  adjustTextureAddress(float& u, 
                       float& v, 
                       const eTextureMode& mode = eTextureMode::CLAMP);

  Color 
  getColor(float& u, 
           float& v, 
           const eTextureMode& mode = eTextureMode::CLAMP);

  void 
  draw(Image& img, 
       int32_t x,
       int32_t y,
       const Rect& srcRect,
       const eTextureMode& mode = eTextureMode::CLAMP, 
       const eBlendMode& blend = eBlendMode::ALPHABLEND,
       const eSamplerFilter& filter = eSamplerFilter::POINT);

  void
  setImage(const Image& img);

  void
  setImage(const Image& img, const UPtr<GraphicsAPI>& pGraphicsAPI);


  Color sample(float u, 
               float v, 
               const eTextureMode& mode = eTextureMode::CLAMP, 
               const eSamplerFilter& sampler = eSamplerFilter::POINT);


public:

  Image m_img;

  class ID3D11Texture2D* m_pTexture = nullptr;
  class ID3D11ShaderResourceView* m_pSRV = nullptr;
  class ID3D11RenderTargetView* m_pRTV = nullptr;
  class ID3D11DepthStencilView* m_pDSV = nullptr;

};
}

