#pragma once

#include "mdImageManipulationPrerequisites.h"
#include "mdColor.h"
#include "mdTriangle.h"


namespace MD {

BETTER_ENUM(eTextureMode, 
            uint32_t, 
            UNKNOWN, 
            NONE, 
            CLAMP, 
            MIRROR,
            MIRROR_ONCE,
            STRETCH,
            WRAP);

BETTER_ENUM(eImageFormat,
            uint32_t,
            UNKNOWN,
            RAW,
            BMP);

BETTER_ENUM(eBlendMode,
            uint32_t,
            NONE,
            ALPHABLEND,
            ADDITIVE);

BETTER_ENUM(eRegionCode, 
            uint32_t, 
            INSIDE = 0, 
            LEFT = 1, 
            RIGHT = 2, 
            BOTTOM = 4, 
            TOP = 8);

#pragma pack(push, 1)
struct BMPFileHeader {
  uint16_t fileType{ 0x4D42 }; // 'BM'
  uint32_t fileSize{ 0 };
  uint16_t reserved1{ 0 };
  uint16_t reserved2{ 0 };
  uint32_t offsetData{ 54 };
};

struct BMPInfoHeader {
  uint32_t size{ 40 };
  int32_t width{ 0 };
  int32_t height{ 0 };
  uint16_t planes{ 1 };
  uint16_t bitCount{ 24 };
  uint32_t compression{ 0 };
  uint32_t sizeImage{ 0 };
  int32_t xPixelsPerMeter{ 0 };
  int32_t yPixelsPerMeter{ 0 };
  uint32_t colorsUsed{ 0 };
  uint32_t colorsImportant{ 0 };
};
#pragma pack(pop)

struct Rect
{
public:

  Rect() = default;

  Rect(int32_t nx, 
       int32_t ny, 
       int32_t nwidth, 
       int32_t nheight)
    : x(nx),
      y(ny),
      width(nwidth),
      height(nheight) {}

  int32_t x;
  int32_t y;
  int32_t width;
  int32_t height;
};


using Pixel = Color;

// Filter a color
// Set Pixel
// Get Pixel
// Clamp

class Image
{
public:

  Image() = default;

  ~Image() = default;

  Image(uint32_t width, uint32_t height, uint32_t bpp = 32) :
    m_width(width),
    m_height(height),
    m_bpp(bpp) {}

  void
  create(uint32_t width, uint32_t height, uint32_t bpp = 32);

  void
  bitBlt(const Image& src, 
         const Rect& srcRect, 
         const Rect& destRect, 
         const eTextureMode& format = eTextureMode::NONE);

  void
  line(int32_t x0, 
       int32_t y0, 
       int32_t x1, 
       int32_t y1,
       const Color& color);

  void
  bresenhamLine(int32_t x0, 
                int32_t y0, 
                int32_t x1, 
                int32_t y1,
                const Color& color);

  void
  bresenhamCircle();

  // void
  // fillTriangle(const Triangle& t, Image& img, const Color& toColor = Color::CLEAR);

  int32_t 
  computeRegionCode(const int32_t x,
                    const int32_t y,
                    const int32_t minX,
                    const int32_t minY,
                    const int32_t maxX,
                    const int32_t maxY);

  bool
  clipLine(Rect r, 
           int32_t& x0,
           int32_t& y0, 
           int32_t& x1, 
           int32_t& y1, 
           const Color& c);

  // Bresenham circle

  void
  decode(const String& sourceFile);

  void
  decode(const Path& sourceFile);

  void
  encode(const String& sourceFile, const eImageFormat& format = eImageFormat::BMP);

  void
  encode(const Path& sourceFile, const eImageFormat& format = eImageFormat::BMP);

  void
  clear(const Color& color = Color::CLEAR);

  uint32_t
  mirrorCoord(int32_t coord, int32_t max);

  Pixel&
  getPixel(const uint32_t& x, const uint32_t& y);

  void
  setPixel(const uint32_t& x, const uint32_t& y, const Pixel& color = Pixel::CLEAR);

  Pixel&
  getColor(const float& u, const float& v);

  void
  setColor(const uint32_t& u, const uint32_t& v, const Pixel& color = Pixel::CLEAR);

  void
  filterPixel(const Rect& rect, const Pixel& color = Pixel::WHITE, const uint32_t& tolerance = 0);

  // void
  // alphaBlend(const Rect& rect, const eBlendMode& blendMode);

public:
  uint32_t m_width;
  uint32_t m_height;
  uint32_t m_bpp;
  
  Vector<Pixel> m_pixels;
 
};

}

