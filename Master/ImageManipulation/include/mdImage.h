#pragma once

#include "mdImageManipulationPrerequisites.h"
#include "mdColor.h"


namespace MD {

BETTER_ENUM(eTextureMode, 
            uint32_t, 
            UNKNOWN, 
            NONE, 
            REPEAT, 
            CLAMP, 
            MIRROR, 
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

#pragma pack(push, 1)
struct BMPFileHeader 
{
  uint16_t fileType;     // Siempre 'BM'
  uint32_t fileSize;     // Tamaño total del archivo
  uint16_t reserved1;    // Reservado, debe ser 0
  uint16_t reserved2;    // Reservado, debe ser 0
  uint32_t offsetData;   // Offset donde comienzan los datos de píxeles
};

struct BMPInfoHeader 
{
  uint32_t size;         // Tamaño del encabezado DIB
  int32_t width;         // Anchura de la imagen en píxeles
  int32_t height;        // Altura de la imagen en píxeles
  uint16_t planes;       // Siempre 1
  uint16_t bitCount;     // Bits por píxel (24 en este caso)
  uint32_t compression;  // Tipo de compresión (0 = sin compresión)
  uint32_t sizeImage;    // Tamaño de los datos de la imagen
  int32_t xPixelsPerMeter; // Resolución horizontal
  int32_t yPixelsPerMeter; // Resolución vertical
  uint32_t colorsUsed;   // Número de colores en la paleta (0 = todos)
  uint32_t colorsImportant; // Número de colores importantes (0 = todos)
};
#pragma pack(pop)

struct Rect
{
public:

  Rect() = default;

  Rect(uint32_t nx, uint32_t ny, uint32_t nwidth, uint32_t nheight)
    : x(nx),
      y(ny),
      width(nwidth),
      height(nheight) {}

  uint32_t x;
  uint32_t y;
  uint32_t width;
  uint32_t height;
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
         const eTextureMode& format);

  void
  line(const uint32_t& x0, 
       const uint32_t& y0, 
       const uint32_t& x1, 
       const uint32_t& y1,
       const Color& color);

  void
  bresenhamLine(uint32_t x0, 
                uint32_t y0, 
                uint32_t x1, 
                uint32_t y1,
                const Color& color);

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
  mirrorCoord(uint32_t coord, const uint32_t max);

  Pixel&
  getPixel(const uint32_t& x, const uint32_t& y);

  void
  setPixel(const uint32_t& x, const uint32_t& y, const Pixel& color = Pixel::CLEAR);

  Pixel&
  getColor(const uint32_t& u, const uint32_t& v);

  void
  setColor(const uint32_t& u, const uint32_t& v, const Pixel& color = Pixel::CLEAR);

  void
  filterPixel(const Rect& rect, const Pixel& color = Pixel::WHITE, const uint32_t& tolerance = 0);

  void
  alphaBlend(const Rect& rect, const eBlendMode& blendMode);

public:
  uint32_t m_width;
  uint32_t m_height;
  uint32_t m_bpp;
  
  Vector<Pixel> m_pixels;
 
};

}

