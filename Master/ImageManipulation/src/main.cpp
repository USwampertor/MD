#include "mdImage.h"
#include "mdTexture.h"
#include <iostream>

using namespace MD;

int main(int argc, char* argv[]) {

  std::cout << argv[0] << std::endl;

  Image sourceFile;
  Image destinyFile;
  String sourceFStr = "portrait.bmp";
  String resourcesPath = Utils::format("%s/../../resources",
                                       Path(argv[0]).parent_path().string().c_str());
  // String destFStr = "maiden.bmp";
  String sourceString1 = Utils::format("%s/%s", resourcesPath.c_str(), sourceFStr.c_str());

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
  String destinyString = Utils::format("%s/res_stretch.bmp", resourcesPath.c_str());
  destinyFile.encode(destinyString, eImageFormat::BMP);
  destinyFile.clear(Color::BLACK);

  destinyFile.bitBlt(sourceFile, 
                     Rect(0, 0, sourceFile.m_width, sourceFile.m_height),
                     Rect(0, 0, destinyFile.m_width, destinyFile.m_height),
                     eTextureMode::CLAMP);
  destinyString = Utils::format("%s/res_clamp.bmp", resourcesPath.c_str());
  destinyFile.encode(destinyString, eImageFormat::BMP);
  destinyFile.clear(Color::BLACK);

  destinyFile.bitBlt(sourceFile, 
                     Rect(0, 0, sourceFile.m_width, sourceFile.m_height),
                     Rect(0, 0, sourceFile.m_width * 2, sourceFile.m_height),
                     eTextureMode::MIRROR);
  destinyString = Utils::format("%s/res_mirror.bmp", resourcesPath.c_str());
  destinyFile.encode(destinyString, eImageFormat::BMP);
  destinyFile.clear(Color::BLACK);


  destinyFile.bitBlt(sourceFile, 
                     Rect(0, 0, sourceFile.m_width, sourceFile.m_height),
                     Rect(0, 0, destinyFile.m_width, destinyFile.m_height),
                     eTextureMode::REPEAT);
  destinyString = Utils::format("%s/res_repeat.bmp", resourcesPath.c_str());
  destinyFile.encode(destinyString, eImageFormat::BMP);
  destinyFile.clear(Color::BLACK);

  destinyFile.bitBlt(sourceFile, 
                     Rect(0, 0, sourceFile.m_width, sourceFile.m_height),
                     Rect(0, 0, sourceFile.m_width, sourceFile.m_height),
                     eTextureMode::NONE);
  destinyString = Utils::format("%s/res_none.bmp", resourcesPath.c_str());
  destinyFile.encode(destinyString, eImageFormat::BMP);

  Color filteredColor;
  filteredColor.setUint(184, 184, 184);
  destinyFile.filterPixel(Rect(0, 0, destinyFile.m_width, destinyFile.m_height), filteredColor);
  destinyString = Utils::format("%s/res_filtered.bmp", resourcesPath.c_str());
  destinyFile.encode(destinyString, eImageFormat::BMP);

  destinyFile.clear(Color::BLACK);

  Image imgScreen;
  imgScreen.create(1920, 1080, 24);
  imgScreen.clear(Color::BLACK);

  Image imgTerrain;
  imgTerrain.decode(Utils::format("%s/terrain2.bmp", resourcesPath.c_str()));

  Image imgHorse;
  imgHorse.decode(Utils::format("%s/sprite2.bmp", resourcesPath.c_str()));

  
  imgScreen.bitBlt(imgTerrain, 
                   Rect(0, 0, imgTerrain.m_width, imgTerrain.m_height), 
                   Rect(0, 0, imgScreen.m_width, imgScreen.m_height), 
                   eTextureMode::NONE);


  Texture horseTexture;
  horseTexture.setImage(imgHorse);

  for (uint32_t i = 0; i < imgScreen.m_height; ++i) {
    for (uint32_t j = 0; j < imgScreen.m_width; ++j) {
      float u = static_cast<float>(j) / (imgScreen.m_width - 1);
      float v = static_cast<float>(i) / (imgScreen.m_height - 1);
      imgScreen.setPixel(j, 
                         i, 
                         horseTexture.sample(u * 20.0f, 
                                             v * 20.0f, 
                                             eTextureMode::CLAMP, 
                                             eSamplerFilter::POINT));
    }
  }

  destinyString = Utils::format("%s/res_horse.bmp", resourcesPath.c_str());
  imgScreen.encode(destinyString);


  return 0;
}

