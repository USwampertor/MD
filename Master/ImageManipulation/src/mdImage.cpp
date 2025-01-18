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
      else if (eTextureMode::REPEAT == format) {
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

        srcx = std::clamp(srcRect.x + static_cast<int>(u * srcRect.width), uint32_t(0), src.m_width - 1);
        srcy = std::clamp(srcRect.y + static_cast<int>(v * srcRect.height), uint32_t(0), src.m_height - 1);
      }

      Pixel srcPixel = src.m_pixels[(srcy * srcRect.width) + (srcx)];
      m_pixels[((desy) * m_width) + (desx)] = srcPixel;

    }
  }

  // if (eTextureMode::NONE == format) {
  // 
  //   Just copy it from src to dst in given rects
  //   for (int y = 0; y < srcRect.height; ++y) {
  //     for (int x = 0; x < srcRect.width; ++x) {
  //   
  //       if ((y + srcRect.y) < src.m_height && (x + srcRect.x) < src.m_width) {
  //         
  //         Pixel srcPixel = src.m_pixels[((y + srcRect.y) * srcRect.width) + (x + srcRect.x)];
  //         
  //         if ((y + destRect.y) < m_height && (x + destRect.x) < m_width) {
  //           m_pixels[((y + destRect.y) * m_width) + (x + destRect.x)] = srcPixel;
  //         }
  //       }
  //     }
  //   }
  // 
  // 
  // 
  // 
  // }
  // else if (eTextureMode::REPEAT == format) {
  // 
  // 
  //   for (uint32_t desty = 0; desty < destRect.height; ++desty) {
  //     for (uint32_t destx = 0; destx < destRect.width; ++destx) {
  // 
  //       // porcentual del valor del tamaño de src
  //       // i.e 65 % 50 de un destiny de 75, significa que es 16 px de source
  //       
  // 
  //       uint32_t loopX = (srcRect.x + destx % src.m_width + srcRect.width) % (srcRect.width);
  //       uint32_t loopY = (srcRect.y + desty % src.m_height + srcRect.height) % (srcRect.height);
  // 
  //       if (destRect.x + destx < 0 || 
  //           destRect.x + destx >= m_width ||
  //           destRect.y + desty < 0 ||
  //           destRect.y + desty >= m_height) {
  //         continue;  
  //       }
  // 
  //       // TODO: Check when cuts are different
  //       Pixel srcPixel = src.m_pixels[(loopY * srcRect.width) + (loopX)];
  //       m_pixels[((desty + destRect.y) * m_width) + (destx + destRect.x)] = srcPixel;
  //       
  //     }
  //   }
  // 
  // }
  // else if (eTextureMode::CLAMP == format) {
  //   for (uint32_t desty = 0; desty < destRect.height; ++desty) {
  //     for (uint32_t destx = 0; destx < destRect.width; ++destx) {
  // 
  // 
  // 
  //       int srcx = std::clamp(srcRect.x + destx, uint32_t(0), src.m_width - 1);
  //       int srcy = std::clamp(srcRect.y + desty, uint32_t(0), src.m_height - 1);
  // 
  //       if (destRect.x + destx < 0 || 
  //           destRect.x + destx >= m_width ||
  //           destRect.y + desty < 0 ||
  //           destRect.y + desty >= m_height) {
  //         continue;  
  //       }
  // 
  //       Pixel srcPixel = src.m_pixels[srcy * src.m_width + srcx];
  //       m_pixels[((desty + destRect.y) * m_width) + (destx + destRect.x)] = srcPixel;
  //     }
  //   }
  // }
  // else if (eTextureMode::MIRROR == format) {
  //   for (uint32_t desty = 0; desty < destRect.height; ++desty) {
  //     for (uint32_t destx = 0; destx < destRect.width; ++destx) {
  // 
  //       int srcx = mirrorCoord(srcRect.x + destx, src.m_width);
  //       int srcy = mirrorCoord(srcRect.y + desty, src.m_height);
  // 
  //       if (destRect.x + destx < 0 ||
  //           destRect.x + destx >= m_width ||
  //           destRect.y + desty < 0 ||
  //           destRect.y + desty >= m_height) {
  //           continue;
  //       }
  // 
  //       Pixel srcPixel = src.m_pixels[srcy * src.m_width + srcx];
  //       m_pixels[((desty + destRect.y) * m_width) + (destx + destRect.x)] = srcPixel;
  //     }
  //   }
  // }
  // else if (eTextureMode::STRETCH == format) {
  //   for (uint32_t desty = 0; desty < destRect.height; ++desty) {
  //     for (uint32_t destx = 0; destx < destRect.width; ++destx) {
  // 
  //       if (destRect.x + destx < 0 ||
  //           destRect.x + destx >= m_width ||
  //           destRect.y + desty < 0 ||
  //           destRect.y + desty >= m_height) {
  //           continue;
  //       }
  // 
  // 
  //       float u = static_cast<float>(destx) / destRect.width;
  //       float v = static_cast<float>(desty) / destRect.height;
  // 
  //       int srcx = std::clamp(srcRect.x + static_cast<int>(u * srcRect.width), uint32_t(0), src.m_width - 1);
  //       int srcy = std::clamp(srcRect.y + static_cast<int>(v * srcRect.height), uint32_t(0), src.m_height - 1);
  // 
  //       Pixel srcPixel = src.m_pixels[srcy * src.m_width + srcx];
  //       m_pixels[((desty + destRect.y) * m_width) + (destx + destRect.x)] = srcPixel;
  //     }
  //   }
  // }
    

}

uint32_t
Image::mirrorCoord(uint32_t coord, const uint32_t max) {
  if (max <= 0) return 0;

  coord = coord % (2 * max);
  if (coord < 0) coord += 2 * max; 

  return coord >= max ? 2 * max - coord - 1 : coord;
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

    if (infoHeader.bitCount != 24) {
      throw std::runtime_error("Only 24 bits per pixel BMP files are supported.");
    }

    m_width = infoHeader.width;
    m_height = infoHeader.height;
    m_bpp = 24;
    m_pixels.clear();
    m_pixels.resize(0);
    m_pixels.resize(m_width * m_height, Pixel::BLACK);
    // create(infoHeader.width, infoHeader.height, 24);


    const int rowPadding = (4 - (m_width * 3) % 4) % 4;
    file.seekg(fileHeader.offsetData, std::ios::beg);

    for (int32_t row = m_height - 1; row >= 0; --row) {
      for (uint32_t col = 0; col < m_width; ++col) {
        uint8_t b, g, r;
        file.read(reinterpret_cast<char*>(&b), 1);
        file.read(reinterpret_cast<char*>(&g), 1);
        file.read(reinterpret_cast<char*>(&r), 1);

        // Store as normalized floats
        m_pixels[(row * m_width) + col] = {
          r / 255.0f,
          g / 255.0f,
          b / 255.0f,
          1.0f // Alpha is fully opaque
        };

      }
      file.ignore(rowPadding); // Skip padding
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
    const int rowPadding = (4 - (m_width * 3) % 4) % 4; // Padding for 4-byte alignment
    const int rowSize = m_width * 3 + rowPadding;       // Row size with padding
    const int dataSize = rowSize * m_height;           // Total size of pixel data

    BMPFileHeader fileHeader = {};
    BMPInfoHeader infoHeader = {};

    fileHeader.fileType = 0x4D42; // 'BM'
    fileHeader.fileSize = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + dataSize;
    fileHeader.offsetData = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

    infoHeader.size = sizeof(BMPInfoHeader);
    infoHeader.width = m_width;
    infoHeader.height = m_height;
    infoHeader.planes = 1;
    infoHeader.bitCount = 24; // 24 bits per pixel
    infoHeader.compression = 0; // No compression
    infoHeader.sizeImage = dataSize;

    file.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
    file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));

    for (int32_t row = m_height - 1; row >= 0; --row) {
      for (int32_t col = 0; col < m_width; ++col) {
        const Pixel& pixel = m_pixels[(row * m_width) + col];
          
        uint8_t b = static_cast<uint8_t>(std::clamp(pixel.m_b, 0.0f, 1.0f) * 255);
        uint8_t g = static_cast<uint8_t>(std::clamp(pixel.m_g, 0.0f, 1.0f) * 255);
        uint8_t r = static_cast<uint8_t>(std::clamp(pixel.m_r, 0.0f, 1.0f) * 255);

        file.write(reinterpret_cast<const char*>(&b), 1);
        file.write(reinterpret_cast<const char*>(&g), 1);
        file.write(reinterpret_cast<const char*>(&r), 1);
      }
      // Write padding
      uint8_t padding[3] = { 0, 0, 0 };
      file.write(reinterpret_cast<const char*>(padding), rowPadding);
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
Image::getColor(const uint32_t& u, const uint32_t& v) {
  uint32_t srcx = std::clamp(static_cast<uint32_t>(u * m_width), uint32_t(0), m_width - 1);
  uint32_t srcy = std::clamp(static_cast<uint32_t>(v * m_height), uint32_t(0), m_height - 1);

  return m_pixels[(srcy * m_width) + srcx];
}

void
Image::setColor(const uint32_t& u, const uint32_t& v, const Pixel& color /*= Pixel::CLEAR */) {
  uint32_t srcx = std::clamp(static_cast<uint32_t>(u * m_width), uint32_t(0), m_width - 1);
  uint32_t srcy = std::clamp(static_cast<uint32_t>(v * m_height), uint32_t(0), m_height - 1);

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


}
