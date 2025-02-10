#include "mdTexture.h"

namespace MD
{
void 
Texture::adjustTextureAddress(float& u,
                              float& v,
                              const eTextureMode& mode /*= eTextureMode::CLAMP*/) {
  // TODO: Finish this

  if (eTextureMode::WRAP == mode) {
    u = std::fmodf(u, 1.0f);
    v = std::fmodf(v, 1.0f);
  }
  else if (eTextureMode::MIRROR == mode) {
    u = std::fmodf(u, 2.0f);
    v = std::fmodf(v, 2.0f);
    
    u = u < 0.0f ? 2.0f + u : u > 1.0f ? 2.0f - u : u;
    v = v < 0.0f ? 2.0f + v : v > 1.0f ? 2.0f - v : v;
  }
  else if (eTextureMode::MIRROR_ONCE == mode) {

  }
  else if (eTextureMode::CLAMP == mode) {
    u = std::clamp(u, 0.0f, 1.0f);
    v = std::clamp(v, 0.0f, 1.0f);
  }

}

Color 
Texture::getColor(float& u, 
                  float& v, 
                  const eTextureMode& mode /*= eTextureMode::CLAMP*/) {
  adjustTextureAddress(u, v, mode);
  return m_img.getColor(u, v);
}


Color 
Texture::sample(float u, 
                float v, 
                const eTextureMode& mode, 
                const eSamplerFilter& sampler) {
  adjustTextureAddress(u, v, mode);
  if (eSamplerFilter::POINT == sampler) {
    // TODO: REMOVE REDUNDANCY
    return m_img.getColor(u, v);
  }
  else if (eSamplerFilter::LINEAR == sampler) {
    
    float x = u * (m_img.m_width - 1);
    float y = v * (m_img.m_height - 1);

    int32_t x0 = static_cast<int32_t>(x); // std::clamp(static_cast<uint32_t>(x), uint32_t(0), m_img.m_width - 1);
    int32_t y0 = static_cast<int32_t>(y); // std::clamp(static_cast<uint32_t>(y), uint32_t(0), m_img.m_height - 1);

    int32_t x1 = std::min(x0 + 1, static_cast<int32_t>(m_img.m_width - 1));
    int32_t y1 = std::min(y0 + 1, static_cast<int32_t>(m_img.m_height - 1));

    float dx = x - x0;
    float dy = y - y0;

    Color c00(m_img.getPixel(x0, y0));
    Color c10(m_img.getPixel(x1, y0));
    Color c01(m_img.getPixel(x0, y1));
    Color c11(m_img.getPixel(x1, y1));

    Color c0 = c00 * (1.0f - dx) + c10 * dx;
    Color c1 = c01 * (1.0f - dx) + c11 * dx;

    return c0 * (1.0f - dy) + c1 * dy;
  }
  else if (eSamplerFilter::BILINEAL == sampler) {

  }

}

void
Texture::draw(Image& img,
              int32_t x,
              int32_t y,
              const Rect& srcRect, 
              const eTextureMode& mode, 
              const eBlendMode& blend,
              const eSamplerFilter& filter) {

  int32_t realWidth = srcRect.width - srcRect.x;
  int32_t realHeight = srcRect.height - srcRect.y;

  if (realWidth < 0 || realHeight < 0) {
    return;
  }

  if (x + realWidth > img.m_width) {
    realWidth -= (x + realWidth) - img.m_width;
  }
  if (y + realHeight > img.m_height) {
    realHeight -= (y + realHeight) - img.m_height;
  }

  for (int32_t dsty = 0; dsty < srcRect.height; ++dsty) {
    for (int32_t dstx = 0; dstx < srcRect.width; ++dstx) {

      float u = static_cast<float>(dstx) / srcRect.width;
      float v = static_cast<float>(dsty) / srcRect.height;
      // TODO: Check on samplerFilter
      Color srcColor = sample(u, v, mode, filter);
      Color dstColor(img.getPixel(x + dstx, y + dsty));
      Color blendedColor = Color::CLEAR;

      if (eBlendMode::ALPHABLEND == blend) {
        blendedColor = srcColor * srcColor.m_a + dstColor * (1.0f - srcColor.m_a);
      }
      else if (eBlendMode::ADDITIVE == blend) {
        blendedColor = (srcColor * srcColor.m_a) + dstColor;
        blendedColor.saturate();
      }
      else if (eBlendMode::NONE == blend) {
        blendedColor = srcColor;
      }
      img.setPixel(x + dstx, y + dsty, blendedColor);
    }
  }
}

void
Texture::setImage(const Image& img) {
  m_img = img;
}

}
