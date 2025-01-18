#pragma once
#include "mdImageManipulationPrerequisites.h"
#include "mdImage.h"


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
       const Rect& srcRect, 
       const eTextureMode& mode = eTextureMode::CLAMP, 
       const eBlendMode& blend = eBlendMode::ALPHABLEND);

  void
  setImage(const Image& img);

  Color sample(float& u, 
               float& v, 
               const eTextureMode& mode = eTextureMode::CLAMP, 
               const eSamplerFilter& sampler = eSamplerFilter::POINT);


public:

  Image m_img;
};
}

