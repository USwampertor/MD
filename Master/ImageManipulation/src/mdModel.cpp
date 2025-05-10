#include "mdModel.h"
#include "mdGraphicsAPI.h"

namespace MD {
// 


struct float2
{
  float u; 
  float v;
  bool operator==(const float2& rhs) const { return u == rhs.u && v == rhs.v; }
  bool operator!=(const float2& rhs) const { return !(*this == rhs); }
};

struct FaceVertex
{
  int vertexIndex;
  int uvIndex;
  int normalIndex;
  bool operator==(const FaceVertex& rhs) const { return vertexIndex == rhs.vertexIndex && uvIndex == rhs.uvIndex && normalIndex == rhs.normalIndex; }
  bool operator!=(const FaceVertex& rhs) const { return !(*this == rhs); }
};

struct FaceVertex_Hash {
  std::size_t operator()(const FaceVertex& vertex) const {
    std::size_t h1 = std::hash<int>()(vertex.vertexIndex);
    std::size_t h2 = std::hash<int>()(vertex.uvIndex);
    std::size_t h3 = std::hash<int>()(vertex.normalIndex);
    return h1 ^ (h2 << 1) ^ (h3 << 2);
  }
};

// namespace std
// {
//   template<>
//   struct hash<MODEL_VERTEX>
//   {
//     size_t operator()(const MODEL_VERTEX& vertex) const {
//       return hash<float>()(vertex.position.x) ^
//         hash<float>()(vertex.position.y)^
//         hash<float>()(vertex.position.z)^
//         hash<float>()(vertex.c.m_r)^
//         hash<float>()(vertex.c.m_g)^
//         hash<float>()(vertex.c.m_b)^
//         hash<float>()(vertex.c.m_a)^
//         hash<float>()(vertex.u)^
//         hash<float>()(vertex.v)^
//     }
//   };
// }

bool
Model::loadFromFile(const Path& filePath, const UPtr<GraphicsAPI>& pGraphicsAPI) {
  if (!std::filesystem::exists(filePath)) {
    return false;
  }
  IFStream file(filePath, std::ios::ate);


  auto fileSize = file.tellg();
  file.seekg(std::ios::beg);

  Vector<String> data;
  
  String fileData;
  fileData.resize(fileSize);
  file.read(&fileData[0], fileSize);

  Vector<String> lines = Utils::split(fileData, '\n');
  Vector<MODEL_VERTEX> vertices;

  Vector<unsigned short> indices;
  Vector<Vector3> tmpPos;
  Vector<float2> tmpUV;
  Vector<Vector3> tmpNorm;

  UnorderedMap<FaceVertex, unsigned short, FaceVertex_Hash> tmpMap;
  
  int vt_index = 0; 


  for (const auto& line : lines) {
    Vector<String> tokens = Utils::split(line, ' ');
    if (tokens.empty()) {
      continue;
    }
    if (tokens[0] == "v") {
      Vector3 pos;
      pos.x = std::stof(tokens[1]);
      pos.y = std::stof(tokens[2]);
      pos.z = std::stof(tokens[3]);
      tmpPos.push_back(pos);
    }
    else if (tokens[0] == "vt") {

      float2 uv;
      uv.u = (std::stof(tokens[1]));
      uv.v = 1.0f - (std::stof(tokens[2]));
      tmpUV.push_back(uv);

      // ++vt_index;
    }
    else if (tokens[0] == "vn") {

      Vector3 norm;
      norm.x = std::stof(tokens[1]);
      norm.y = std::stof(tokens[2]);
      norm.z = std::stof(tokens[3]);
      tmpNorm.push_back(norm);

      // ++vt_index;
    }
    else if (tokens[0] == "f") {

      Vector<unsigned short> faceIndices;

      for (size_t i = 1; i < tokens.size(); ++i) {
        Vector<String> faceIndex = Utils::split(tokens[i], '/');

        unsigned int vertexIndex = std::stoi(faceIndex[0]) - 1;
        unsigned int uvIndex = std::stoi(faceIndex[1]) - 1;
        unsigned int normIndex = std::stoi(faceIndex[1]) - 2;

        FaceVertex faceVertex;

        faceVertex.vertexIndex = vertexIndex;
        faceVertex.uvIndex = uvIndex;
        faceVertex.normalIndex = normIndex;

        // MODEL_VERTEX vertex;
        // vertex.position = tmpPos[vertexIndex];
        // vertex.c = Color::MISSING;
        // vertex.u = tmpUV[uvIndex].u;
        // vertex.v = tmpUV[uvIndex].v;

        if (tmpMap.find(faceVertex) == tmpMap.end()) {
          tmpMap[faceVertex] = static_cast<unsigned short>(vertices.size());

          MODEL_VERTEX vertex;
          vertex.position = tmpPos[faceVertex.vertexIndex];
          vertex.c = Color::MISSING;
          vertex.u = tmpUV[faceVertex.uvIndex].u;
          vertex.v = tmpUV[faceVertex.uvIndex].v;
          vertex.normal = tmpNorm[faceVertex.normalIndex];
          vertices.push_back(vertex);
        }

        faceIndices.push_back(tmpMap[faceVertex]);

      }
      indices.push_back(faceIndices[0]);
      indices.push_back(faceIndices[1]);
      indices.push_back(faceIndices[2]);
    }
 
  
  }

  m_meshes.resize(1);
  auto& mesh = m_meshes[0];
  mesh.baseVertex = 0;
  mesh.numVertices = vertices.size();
  mesh.baseIndex = 0;
  mesh.numIndices = indices.size();
  mesh.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
  Vector<char> vertex_data;
  vertex_data.resize(vertices.size() * sizeof(MODEL_VERTEX));
  memcpy(vertex_data.data(), vertices.data(), vertices.size() * sizeof(MODEL_VERTEX));
  m_pVertexBuffer = pGraphicsAPI->createVertexBuffer(vertex_data);
  if (!m_pVertexBuffer) {
    return false;
  }
  Vector<char> index_data;
  index_data.resize(indices.size() * sizeof(unsigned short));
  memcpy(index_data.data(), indices.data(), indices.size() * sizeof(unsigned short));
  m_pIndexBuffer = pGraphicsAPI->createIndexBuffer(index_data);
  if (!m_pIndexBuffer) {
    return false;
  }

  return true;
}

}
