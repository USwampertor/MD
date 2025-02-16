#include "mdImage.h"

namespace MD
{
void
Image::create(uint32_t width, uint32_t height, uint32_t bpp) {
  m_width = width;
  m_height = height;
  m_bpp = bpp;
  m_pixels.clear();
  m_pixels.resize(width * height, Pixel::CLEAR);
}

void
Image::bitBlt(const Image& src, 
              const Rect& srcRect, 
              const Rect& destRect, 
              const eTextureMode& format) {
    

  for (uint32_t desty = 0; desty < destRect.height; ++desty) {
    for (uint32_t destx = 0; destx < destRect.width; ++destx) {

      uint32_t srcx = 0;
      uint32_t srcy = 0;
      uint32_t desx = 0;
      uint32_t desy = 0;
        
      desx = destRect.x + destx;
      desy = destRect.y + desty;

      if (desx < 0 || desx >= m_width || desy < 0 || desy >= m_height) {
        continue;
      }

      if (eTextureMode::NONE == format) {
        srcx = srcRect.x + destx;
        srcy = srcRect.y + desty;
      }
      else if (eTextureMode::WRAP == format) {
        srcx = (srcRect.x + destx % src.m_width + srcRect.width) % (srcRect.width);
        srcy = (srcRect.y + desty % src.m_height + srcRect.height) % (srcRect.height);
      }
      else if (eTextureMode::CLAMP == format) {
        srcx = std::clamp(srcRect.x + destx, uint32_t(0), src.m_width - 1);
        srcy = std::clamp(srcRect.y + desty, uint32_t(0), src.m_height - 1);
      }
      else if (eTextureMode::MIRROR == format) {
        srcx = mirrorCoord(srcRect.x + destx, src.m_width);
        srcy = mirrorCoord(srcRect.y + desty, src.m_height);
      }
      else if (eTextureMode::STRETCH == format) {
        float u = static_cast<float>(destx) / destRect.width;
        float v = static_cast<float>(desty) / destRect.height;

        srcx = std::clamp(srcRect.x + static_cast<uint32_t>(u * srcRect.width - 1), uint32_t(0), src.m_width - 1);
        srcy = std::clamp(srcRect.y + static_cast<uint32_t>(v * srcRect.height - 1), uint32_t(0), src.m_height - 1);
      }
      int32_t pos = (srcy * srcRect.width) + (srcx);
      Pixel srcPixel = src.m_pixels[pos];
      m_pixels[((desy) * m_width) + (desx)] = srcPixel;

    }
  }

}

uint32_t
Image::mirrorCoord(int32_t coord, int32_t max) {
  if (max <= 0) return 0;

  coord = coord % (2 * max);
  if (coord < 0) coord += 2 * max; 

  return coord >= max ? 2 * max - coord - 1 : coord;
}

  int32_t 
  Image::computeRegionCode(const int32_t x,
                           const int32_t y,
                           const int32_t minX,
                           const int32_t minY,
                           const int32_t maxX,
                           const int32_t maxY) {
    int code = eRegionCode::INSIDE;

    // code = x < minX ? code |= eRegionCode::LEFT : x > maxX ? code |= eRegionCode::RIGHT : code;
    // code = y < minY ? code |= eRegionCode::BOTTOM : y > maxY ? code |= eRegionCode::TOP : code;

    if (x < minX) { code |= eRegionCode::LEFT; }
    else if (x > maxX) { code |= eRegionCode::RIGHT; }
    if (y < minY) { code |= eRegionCode::BOTTOM; }
    else if (y > maxY) { code |= eRegionCode::TOP; }
    
    return code;
  }

  bool
  Image::clipLine(Rect srcRect,
                  int32_t& x0,
                  int32_t& y0,
                  int32_t& x1,
                  int32_t& y1,
                  const Color& c) {
    int code0 = computeRegionCode(x0, y0, srcRect.x, srcRect.y, srcRect.width, srcRect.height);
    int code1 = computeRegionCode(x1, y1, srcRect.x, srcRect.y, srcRect.width, srcRect.height);

    while (true) {
      if (!(code0 | code1)) {
        return true;
      }
      else if (code0 & code1) {
        return false;
      }
      else {
        int x;
        int y;
        int codeOut = code0 ? code0 : code1;

        if (codeOut & eRegionCode::TOP) {
          x = x0 + (x1 - x0) * (srcRect.height - y0) / (y1 - y0);
          y = srcRect.height;
        }
        else if (codeOut & eRegionCode::BOTTOM) {
          x = x0 + (x1 - x0) * (srcRect.y - y0) / (y1 - y0);
          y = srcRect.y;
        }
        else if (codeOut & eRegionCode::RIGHT) {
          x = srcRect.width;
          y = y0 + (y1 - y0) * (srcRect.width - x0) / (x1 - x0);
        }
        else if (codeOut & eRegionCode::LEFT) {
          x = srcRect.x;
          y = y0 + (y1 - y0) * (srcRect.x - x0) / (x1 - x0);
        }

        if (codeOut == code0) {
          x0 = x;
          y0 = y;
          code0 = computeRegionCode(x0, y0, srcRect.x, srcRect.y, srcRect.width, srcRect.height);
        }
        else {
          x1 = x;
          y1 = y;
          code1 = computeRegionCode(x1, y1, srcRect.x, srcRect.y, srcRect.width, srcRect.height);
        }
      }
    }
    return false;
  }

void
Image::decode(const String& sourceFile) {
  Path p(sourceFile);
  decode(p);
}

void
Image::decode(const Path& sourceFile) {
  if (!std::filesystem::exists(sourceFile)) {
    return;
  }

  IFStream file(sourceFile, std::ios::binary);

  if (".bmp" == sourceFile.extension()) {
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    if (fileHeader.fileType != 0x4D42) { // 'BM' in little-endian
      throw std::runtime_error("The file is not a valid BMP.");
    }

    m_width = infoHeader.width;
    m_height = infoHeader.height;
    m_bpp = infoHeader.bitCount;
    m_pixels.clear();
    m_pixels.resize(0);
    m_pixels.resize(m_width * m_height, Pixel::BLACK);
    // create(infoHeader.width, infoHeader.height, 24);


    const int rowPadding = (m_width * (m_bpp / 8) + 3) & ~3;// (4 - (m_width * 3) % 4) % 4;
    file.seekg(fileHeader.offsetData, std::ios::beg);
    std::vector<uint8_t> buffer(rowPadding);
    for (int32_t row = m_height - 1; row >= 0; --row) {
      file.read(reinterpret_cast<char*>(buffer.data()), rowPadding);
      for (uint32_t col = 0; col < m_width; ++col) {
        // uint8_t b, g, r, a;
        // file.read(reinterpret_cast<char*>(&b), 1);
        // file.read(reinterpret_cast<char*>(&g), 1);
        // file.read(reinterpret_cast<char*>(&r), 1);
        // file.read(reinterpret_cast<char*>(&r), 1);

        int index = (m_bpp / 8) * col;
        uint8_t b = buffer[index];
        uint8_t g = buffer[index + 1];
        uint8_t r = buffer[index + 2];
        uint8_t a = (m_bpp == 32) ? buffer[index + 3] : 255;

        // Store as normalized floats
        m_pixels[(row * m_width) + col] = {
          r / 255.0f,
          g / 255.0f,
          b / 255.0f,
          a / 255.0f // Alpha is fully opaque
        };

      }
      // file.ignore(rowPadding); // Skip padding
    }

  }



  file.close();
  return;
}

void
Image::encode(const String& sourceFile, const eImageFormat& format) {
  Path p(sourceFile);
  encode(p, format);
}

void
Image::encode(const Path& sourceFile, const eImageFormat& format) {
    
  if (std::filesystem::exists(sourceFile)) {
    std::filesystem::remove(sourceFile);
  }

  OFStream file(sourceFile, std::ios::binary);

  if (eImageFormat::BMP == format) {
    const int rowPadding = (m_width * (m_bpp / 8) + 3) & ~3;// (4 - (m_width * 3) % 4) % 4; // Padding for 4-byte alignment
    // const int rowSize = m_width * 3 + rowPadding;       // Row size with padding
    const int dataSize = rowPadding * m_height;           // Total size of pixel data

    BMPFileHeader fileHeader = {};
    BMPInfoHeader infoHeader = {};

    fileHeader.fileType = 0x4D42; // 'BM'
    fileHeader.fileSize = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + dataSize;
    fileHeader.offsetData = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

    infoHeader.size = sizeof(BMPInfoHeader);
    infoHeader.width = m_width;
    infoHeader.height = m_height;
    infoHeader.planes = 1;
    infoHeader.bitCount = m_bpp; // 24 bits per pixel normally
    infoHeader.compression = 0; // No compression
    infoHeader.sizeImage = dataSize;

    file.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
    file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));

    std::vector<uint8_t> rowP(rowPadding, 0);
    for (int32_t row = m_height - 1; row >= 0; --row) {
      for (int32_t col = 0; col < m_width; ++col) {
        const Pixel& pixel = m_pixels[(row * m_width) + col];
          
        // uint8_t b = static_cast<uint8_t>(std::clamp(pixel.m_b, 0.0f, 1.0f) * 255);
        // uint8_t g = static_cast<uint8_t>(std::clamp(pixel.m_g, 0.0f, 1.0f) * 255);
        // uint8_t r = static_cast<uint8_t>(std::clamp(pixel.m_r, 0.0f, 1.0f) * 255);

        // file.write(reinterpret_cast<const char*>(&b), 1);
        // file.write(reinterpret_cast<const char*>(&g), 1);
        // file.write(reinterpret_cast<const char*>(&r), 1);

        rowP[col * (m_bpp / 8)] = static_cast<uint8_t>(pixel.m_b * 255);
        rowP[col * (m_bpp / 8) + 1] = static_cast<uint8_t>(pixel.m_g * 255);
        rowP[col * (m_bpp / 8) + 2] = static_cast<uint8_t>(pixel.m_r * 255);
        if (m_bpp == 32) { 
          rowP[col * 4 + 3] = static_cast<uint8_t>(pixel.m_a * 255);
        }
      }
      file.write(reinterpret_cast<const char*>(rowP.data()), rowPadding);
      // Write padding
      // uint8_t padding[3] = { 0, 0, 0 };
      // file.write(reinterpret_cast<const char*>(padding), rowPadding);
    }

  }

  file.close();
  return;
}

void
Image::clear(const Color& color) {
  std::fill(m_pixels.begin(), m_pixels.end(), color);
}

Pixel&
Image::getPixel(const uint32_t& x, const uint32_t& y) {
  return m_pixels[(y * m_width) + x];
}

void
Image::setPixel(const uint32_t& x, const uint32_t& y, const Pixel& color /*= Pixel::CLEAR */) {
  m_pixels[(y * m_width) + x] = color;
}

Pixel&
Image::getColor(const float& u, const float& v) {

  float u1 = u * (m_width - 1);
  float v1 = v * (m_height - 1);

  uint32_t srcx = std::clamp(static_cast<uint32_t>(u1), uint32_t(0), m_width - 1);
  uint32_t srcy = std::clamp(static_cast<uint32_t>(v1), uint32_t(0), m_height - 1);

  return m_pixels[(srcy * m_width) + srcx];
}

void
Image::setColor(const uint32_t& u, const uint32_t& v, const Pixel& color /*= Pixel::CLEAR */) {
  uint32_t srcx = std::clamp(static_cast<uint32_t>(u * m_width - 1), uint32_t(0), m_width - 1);
  uint32_t srcy = std::clamp(static_cast<uint32_t>(v * m_height - 1), uint32_t(0), m_height - 1);

  m_pixels[(srcy * m_width) + srcx] = color;
}

void
Image::filterPixel(const Rect& rect, const Pixel& color, const uint32_t& tolerance) {
  for (uint32_t desty = 0; desty < rect.height; ++desty) {
    for (uint32_t destx = 0; destx < rect.width; ++destx) {

      uint32_t srcx = 0;
      uint32_t srcy = 0;

      srcx = rect.x + destx;
      srcy = rect.y + desty;

      Pixel srcPixel = m_pixels[(srcy * rect.width) + (srcx)];
      if (srcPixel == color) {
        m_pixels[(srcy * rect.width) + (srcx)] = Color::CLEAR;
      }
    }
  }
}

void
Image::line(int32_t x0,
            int32_t y0,
            int32_t x1,
            int32_t y1,
            const Color& color) {

  clipLine(Rect(0, 0, m_width, m_height), x0, y0, x1, y1, color);

  float dx = x1 - x0;
  float dy = y1 - y0;

  float steps = std::max(std::abs(dx), std::abs(dy));

  float xInc = dx / steps;
  float yInc = dy / steps;

  float x = x0;
  float y = y0;

  for (int i = 0; i < steps; ++i) {
    setPixel(uint32_t(x), uint32_t(y), color);
    x += xInc;
    y += yInc;
  }

}

void
Image::bresenhamLine(int32_t x0,
                     int32_t y0,
                     int32_t x1,
                     int32_t y1,
                     const Color& color) {

  clipLine(Rect(0, 0, m_width, m_height), x0, y0, x1, y1, color);

  int32_t dx = std::abs(static_cast<int32_t>(x1 - x0));
  int32_t dy = std::abs(static_cast<int32_t>(y1 - y0));

  int32_t sx = x0 < x1 ? 1 : -1;
  int32_t sy = y0 < y1 ? 1 : -1;

  int32_t err = dx - dy;
  int32_t e2;
  
  while (x0 != x1 || y0 != y1) {
    setPixel(x0, y0, color);
    e2 = 2 * err;

    if (e2 > -dy) {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y0 += sy;
    }
  }

}

float implicitLineEquation(const Vertex& p, const Vertex& p0, const Vertex& p1) {
  return (p0.y - p1.y) * p.x + (p1.x - p0.x) * p.y + (p0.x * p1.y - p1.x * p0.y);
}

// void
// Image::fillTriangle(const Triangle& t, Image& img, const Color& toColor) {
// 
//   Vertex p0 = t.v0;
//   Vertex p1 = t.v1;
//   Vertex p2 = t.v2;
// 
//   Color c0 = Color::RED;
//   Color c1 = Color::BLUE;
//   Color c2 = Color::GREEN;
// 
//   int x_min = static_cast<int>(std::floor(std::min({ p0.x, p1.x, p2.x })));
//   int x_max = static_cast<int>(std::ceil(std::max({ p0.x, p1.x, p2.x })));
//   int y_min = static_cast<int>(std::floor(std::min({ p0.y, p1.y, p2.y })));
//   int y_max = static_cast<int>(std::ceil(std::max({ p0.y, p1.y, p2.y })));
// 
//   float f_alpha = implicitLineEquation(p0, p1, p2);
//   float f_beta = implicitLineEquation(p1, p2, p0);
//   float f_gamma = implicitLineEquation(p2, p0, p1);
// 
//   for (int y = y_min; y <= y_max; ++y) {
//     for (int x = x_min; x <= x_max; ++x) {
//       if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
//         continue;
//       }
// 
//       Vertex p = { static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f }; // Pixel center
// 
//       float alpha = implicitLineEquation(p, p1, p2) / f_alpha;
//       float beta  = implicitLineEquation(p, p2, p0) / f_beta;
//       float gamma = implicitLineEquation(p, p0, p1) / f_gamma;
// 
//       if (alpha >= 0 && beta >= 0 && gamma >= 0) {
//         Vertex off_screen = { -1.0f, -1.0f };
//         bool edge_test_alpha = (alpha > 0) || (implicitLineEquation(off_screen, p1, p2) * implicitLineEquation(p, p1, p2) > 0);
//         bool edge_test_beta = (beta > 0) || (implicitLineEquation(off_screen, p2, p0) * implicitLineEquation(p, p2, p0) > 0);
//         bool edge_test_gamma = (gamma > 0) || (implicitLineEquation(off_screen, p0, p1) * implicitLineEquation(p, p0, p1) > 0);
// 
//         if (edge_test_alpha && edge_test_beta && edge_test_gamma) {
//           Pixel c;
// 
//           float u = alpha * 0 + beta * 0 + gamma * 1;
//           float v = alpha * 0 + beta * 1 + gamma * 0;
// 
//           uint32_t tex_x = static_cast<uint32_t>(u * (img.m_width - 1));
//           uint32_t tex_y = static_cast<uint32_t>(v * (img.m_height - 1));
// 
//           tex_x = std::clamp(tex_x, uint32_t(0), static_cast<uint32_t>(img.m_width - 1));
//           tex_y = std::clamp(tex_y, uint32_t(0), static_cast<uint32_t>(img.m_height - 1));
// 
//           setPixel(x, y, img.getPixel(tex_x, tex_y));
//         }
//       }
//     }
//   }
// }

}
