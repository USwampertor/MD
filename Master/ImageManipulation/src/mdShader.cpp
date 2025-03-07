#include "mdShader.h"

namespace MD {

Shader::~Shader() {
  SAFE_RELEASE(m_pBlob);
}

bool
Shader::compile(const Path& filePath,
                const String& entryFunction,
                const String& shaderModel) {
  if (!std::filesystem::exists(filePath)) {
    return false;
  }
  
  IFStream shaderFile(filePath);

  shaderFile.seekg(0, std::ios::end);
  std::streamsize size = shaderFile.tellg();
  shaderFile.seekg(0, std::ios::beg);

  Vector<char> vShaderCode(size);
  if (size > 0) {
    shaderFile.read(vShaderCode.data(), size);
  }

  shaderFile.close();

  uint32_t flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(_DEBUG)
  flags |= D3DCOMPILE_DEBUG;
#endif

  // ID3DBlob* pShaderBlob = nullptr;
  ID3DBlob* pErrorBlob = nullptr;

  HRESULT hr = D3DCompile(vShaderCode.data(), 
                          vShaderCode.size(),
                          nullptr, 
                          nullptr, 
                          nullptr, 
                          entryFunction.c_str(), 
                          shaderModel.c_str(), 
                          flags, 
                          0, 
                          // &pShaderBlob,
                          &m_pBlob,
                          &pErrorBlob);

  if (FAILED(hr)) {
    MessageBox(nullptr, (LPWSTR)pErrorBlob->GetBufferPointer(), L"Error", MB_OK);
    SAFE_RELEASE(pErrorBlob);
    return false;
  }

  // m_pBlob = pShaderBlob;

  return true;
}

//////////////////////////////////////////////////////////////////////////


VertexShader::~VertexShader() {
  SAFE_RELEASE(m_pVertexShader);
}


//////////////////////////////////////////////////////////////////////////


PixelShader::~PixelShader() {
  SAFE_RELEASE(m_pPixelShader);
}


}
