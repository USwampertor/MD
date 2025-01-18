#include "mdTexture.h"

namespace MD
{
void 
Texture::adjustTextureAddress(float& u,
                              float& v,
                              const eTextureMode& mode /*= eTextureMode::CLAMP*/) {
  // TODO: Finish this

}

Color 
Texture::getColor(float& u, 
                  float& v, 
                  const eTextureMode& mode /*= eTextureMode::CLAMP*/) {
  adjustTextureAddress(u, v, mode);
  return m_img.getColor(u, v);
}


Color 
Texture::sample(float& u, 
                float& v, 
                const eTextureMode& mode, 
                const eSamplerFilter& sampler) {
  if (eSamplerFilter::POINT == sampler) {
    adjustTextureAddress(u, v, mode);
    return m_img.getColor(u, v);
  }
  else if (eSamplerFilter::LINEAR == sampler) {
    adjustTextureAddress(u, v, mode);
    
    float x = u * m_img.m_width - 1; 
    float y = v * m_img.m_height - 1;

    // TODO: AdjustTextureAddress should do this instead
    uint32_t x0 = std::clamp(static_cast<uint32_t>(x), uint32_t(0), m_img.m_width - 1);
    uint32_t y0 = std::clamp(static_cast<uint32_t>(y), uint32_t(0), m_img.m_height - 1);

    uint32_t x1 = std::min(x0 + 1, m_img.m_width - 1);
    uint32_t y1 = std::min(y0 + 1, m_img.m_height - 1);

    float dx = x - x0;
    float dy = y - y0;

    Color c00(m_img.getPixel(x0, y0));
    Color c01(m_img.getPixel(x1, y0));
    Color c10(m_img.getPixel(x0, y1));
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
              const Rect& rect, 
              const eTextureMode& mode, 
              const eBlendMode& blend) {

  for (uint32_t desty = 0; desty < rect.height; ++desty) {
    for (uint32_t destx = 0; destx < rect.width; ++destx) {

      float u = destx / rect.width;
      float v = desty / rect.height;
      // TODO: Check on samplerFilter
      Color srcColor = sample(u, v, mode, eSamplerFilter::POINT);
      Color dstColor(img.getPixel(rect.x + destx, rect.y + desty));
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
      img.setPixel(rect.x + destx, rect.y + desty, blendedColor);
    }
  }
}

void
Texture::setImage(const Image& img) {
  m_img = img;
}

}
