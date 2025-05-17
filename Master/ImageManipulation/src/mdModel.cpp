#include "mdModel.h"
#include "mdGraphicsAPI.h"
#include "mdVector2.h"

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
  // Vector<MODEL_VERTEX> m_vertices;
  // Vector<unsigned short> m_indices;
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
        unsigned int normIndex = std::stoi(faceIndex[2]) - 1;

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
          tmpMap[faceVertex] = static_cast<unsigned short>(m_vertices.size());

          MODEL_VERTEX vertex;
          vertex.position = tmpPos[faceVertex.vertexIndex];
          vertex.c = Color::MISSING;
          vertex.u = tmpUV[faceVertex.uvIndex].u;
          vertex.v = tmpUV[faceVertex.uvIndex].v;
          vertex.normal = tmpNorm[faceVertex.normalIndex];
          m_vertices.push_back(vertex);
        }

        faceIndices.push_back(tmpMap[faceVertex]);

      }
      m_indices.push_back(faceIndices[0]);
      m_indices.push_back(faceIndices[1]);
      m_indices.push_back(faceIndices[2]);
    }
 
  
  }

  m_meshes.resize(1);
  auto& mesh = m_meshes[0];
  mesh.baseVertex = 0;
  mesh.numVertices = m_vertices.size();
  mesh.baseIndex = 0;
  mesh.numIndices = m_indices.size();
  mesh.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
  Vector<char> vertex_data;
  vertex_data.resize(m_vertices.size() * sizeof(MODEL_VERTEX));
  memcpy(vertex_data.data(), m_vertices.data(), m_vertices.size() * sizeof(MODEL_VERTEX));
  m_pVertexBuffer = pGraphicsAPI->createVertexBuffer(vertex_data);
  if (!m_pVertexBuffer) {
    return false;
  }
  Vector<char> index_data;
  index_data.resize(m_indices.size() * sizeof(unsigned short));
  memcpy(index_data.data(), m_indices.data(), m_indices.size() * sizeof(unsigned short));
  m_pIndexBuffer = pGraphicsAPI->createIndexBuffer(index_data);
  if (!m_pIndexBuffer) {
    return false;
  }

  return true;
}


void
Model::computeTangentSpance() {
  Vector<MODEL_VERTEX>& vertices = m_vertices;
  Vector<unsigned short>& indices = m_indices;

  Vector<Vector3> tan1(vertices.size(), Vector3(0, 0, 0));
  Vector<Vector3> tan2(vertices.size(), Vector3(0, 0, 0));

  for (auto& mesh : m_meshes) {
    if (mesh.topology != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) {
      continue;
    }

    int numTriangles = mesh.numIndices / 3;

    for (int t = 0; t < numTriangles; ++t) {
      int i = mesh.baseIndex + (t * 3);
      unsigned int i1 = indices[i];
      unsigned int i2 = indices[i + 1];
      unsigned int i3 = indices[i + 2];
    
      const Vector3& v1 = vertices[i1].position;
      const Vector3& v2 = vertices[i2].position;
      const Vector3& v3 = vertices[i3].position;

      const Vector2& w1 = Vector2(vertices[i1].u, vertices[i1].v);
      const Vector2& w2 = Vector2(vertices[i2].u, vertices[i2].v);
      const Vector2& w3 = Vector2(vertices[i3].u, vertices[i3].v);
    
      float x1 = v2.x - v1.x;
      float x2 = v3.x - v1.x;
      float y1 = v2.y - v1.y;
      float y2 = v3.y - v1.y;
      float z1 = v2.z - v1.z;
      float z2 = v3.z - v1.z;


      float s1 = w2.x - w1.x;
      float s2 = w3.x - w1.x;
      float t1 = w2.y - w1.y;
      float t2 = w3.y - w1.y;

      float denominator = s1 * s2 - s2 * t1;
      if (denominator == 0) {
        continue;
      }

      float r = 1.0f / denominator;

      Vector3 sdir = Vector3((t1 * x1 - t1 * x2) * r,
                             (t1 * y1 - t1 * y2) * r,
                             (t1 * z1 - t1 * z2) * r);

      Vector3 tdir = Vector3((s1 * x2 - s1 * x1) * r,
                             (s1 * y2 - s1 * y1) * r,
                             (s1 * z2 - s1 * z1) * r);

      tan1[i1] += sdir;
      tan1[i2] += sdir;
      tan1[i3] += sdir;

      tan2[i1] += tdir;
      tan2[i2] += tdir;
      tan2[i3] += tdir;

      for (int i = mesh.baseVertex; i < mesh.baseVertex + mesh.numVertices; ++i) {
        const Vector3& n = vertices[i].normal;
        if (std::abs(n.y) > 0.999f) {
          Vector3 helper = Vector3::dot(n, Vector3(0, 1, 0)) < 0.999f ? Vector3(1, 0, 0) : 
                                                                        Vector3(0, 0, 1);
          vertices[i].tangent = Vector3::cross(helper, n);
          vertices[i].tangent.normalize();
          continue;
        }

        const Vector3& t = tan1[i];
        auto tmpTang = t - n * Vector3::dot(n, t);

        if (tmpTang == Vector3(0, 0, 0)) {
          Vector3 helper = n.z < 0.999f ? Vector3(0, 0, 1) :
                                          Vector3(1, 0, 0);
        
          vertices[i].tangent = Vector3::cross(helper, n);
          vertices[i].tangent.normalize();
          continue;
        }
        vertices[i].tangent = tmpTang;
        vertices[i].tangent.normalize();
      }

    }

  }

}

}
