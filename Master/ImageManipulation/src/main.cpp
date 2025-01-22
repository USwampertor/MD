#include "mdImage.h"
#include <iostream>

using namespace MD;

int main(int argc, char* argv[]) {

  std::cout << argv[0] << std::endl;

  Image sourceFile;
  Image destinyFile;
  String sourceFStr = "portrait.bmp";
  // String destFStr = "maiden.bmp";
  String sourceString1 = Utils::format("%s/%s", 
                                      Path(argv[0]).parent_path().string().c_str(),
                                      sourceFStr.c_str());

  // String sourceString2 = Utils::format("%s/%s",
  //                                      Path(argv[0]).parent_path().string().c_str(),
  //                                      destFStr.c_str());

  sourceFile.decode(sourceString1);
  destinyFile.create(3000, 2000, 24);
  destinyFile.clear(Color::BLACK);
  // destinyFile.decode(sourceString2);

  destinyFile.bitBlt(sourceFile, 
                     Rect(0, 0, sourceFile.m_width, sourceFile.m_height),
                     Rect(0, 0, destinyFile.m_width, destinyFile.m_height),
                     eTextureMode::STRETCH);
  String destinyString = Utils::format("%s/res_stretch.bmp", 
                                       Path(argv[0]).parent_path().string().c_str());
  destinyFile.encode(destinyString, eImageFormat::BMP);
  destinyFile.clear(Color::BLACK);

  destinyFile.bitBlt(sourceFile, 
                     Rect(0, 0, sourceFile.m_width, sourceFile.m_height),
                     Rect(0, 0, destinyFile.m_width, destinyFile.m_height),
                     eTextureMode::CLAMP);
  destinyString = Utils::format("%s/res_clamp.bmp", 
                                Path(argv[0]).parent_path().string().c_str());
  destinyFile.encode(destinyString, eImageFormat::BMP);
  destinyFile.clear(Color::BLACK);

  destinyFile.bitBlt(sourceFile, 
                     Rect(0, 0, sourceFile.m_width, sourceFile.m_height),
                     Rect(0, 0, sourceFile.m_width * 2, sourceFile.m_height),
                     eTextureMode::MIRROR);
  destinyString = Utils::format("%s/res_mirror.bmp", 
                                Path(argv[0]).parent_path().string().c_str());
  destinyFile.encode(destinyString, eImageFormat::BMP);
  destinyFile.clear(Color::BLACK);


  destinyFile.bitBlt(sourceFile, 
                     Rect(0, 0, sourceFile.m_width, sourceFile.m_height),
                     Rect(0, 0, destinyFile.m_width, destinyFile.m_height),
                     eTextureMode::REPEAT);
  destinyString = Utils::format("%s/res_repeat.bmp", 
                                Path(argv[0]).parent_path().string().c_str());
  destinyFile.encode(destinyString, eImageFormat::BMP);
  destinyFile.clear(Color::BLACK);

  destinyFile.bitBlt(sourceFile, 
                     Rect(0, 0, sourceFile.m_width, sourceFile.m_height),
                     Rect(0, 0, sourceFile.m_width, sourceFile.m_height),
                     eTextureMode::NONE);
  destinyString = Utils::format("%s/res_none.bmp", 
                                Path(argv[0]).parent_path().string().c_str());
  destinyFile.encode(destinyString, eImageFormat::BMP);

  Color filteredColor;
  filteredColor.setUint(184, 184, 184);
  destinyFile.filterPixel(Rect(0, 0, destinyFile.m_width, destinyFile.m_height), filteredColor);
  destinyString = Utils::format("%s/res_filtered.bmp", 
                                Path(argv[0]).parent_path().string().c_str());
  destinyFile.encode(destinyString, eImageFormat::BMP);

  destinyFile.clear(Color::BLACK);

  return 0;
}

