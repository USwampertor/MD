#pragma once
#include "mdImageManipulationPrerequisites.h"
// 
#include "mdColor.h"
#include "mdVector3.h"
#include "mdGraphicsBuffer.h"
// 
namespace MD {
// 
// 

struct MODEL_VERTEX
{
  MODEL_VERTEX() {
    position = Vector3::ZERO;
    c = Color::CLEAR;
    u = v = 0.0f;
  }

  Vector3 position;
  Color c;
  float u;
  float v;

  bool operator==(const MODEL_VERTEX& other) const {
    return position == other.position && c == other.c && u == other.u && v == other.v;
  }

};

struct MODEL_VERTEX_Hash {
  std::size_t operator()(const MODEL_VERTEX& vertex) const {
    std::size_t h1 = std::hash<float>()(vertex.position.x);
    std::size_t h2 = std::hash<float>()(vertex.position.y);
    std::size_t h3 = std::hash<float>()(vertex.position.z);
    std::size_t h4 = std::hash<float>()(vertex.c.m_r);
    std::size_t h5 = std::hash<float>()(vertex.c.m_g);
    std::size_t h6 = std::hash<float>()(vertex.c.m_b);
    std::size_t h7 = std::hash<float>()(vertex.c.m_a);
    std::size_t h8 = std::hash<float>()(vertex.u);
    std::size_t h9 = std::hash<float>()(vertex.v);
    return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ (h5 << 4) ^ (h6 << 5) ^ (h7 << 6) ^ (h8 << 7) ^ (h9 << 8);
  }
};

// struct MODEL_VERTEX_Hash {
//   std::size_t operator()(const MODEL_VERTEX& vertex) const {
//     std::size_t h1 = std::hash<Vector3>()(vertex.position);
//     std::size_t h2 = std::hash<Color>()(vertex.c);
//     std::size_t h3 = std::hash<float>()(vertex.u);
//     std::size_t h4 = std::hash<float>()(vertex.v);
//     return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
//   }
// };

// 
// 
struct Mesh
{

  int topology;
  int baseVertex;
  int numVertices;

  int baseIndex;
  int numIndices;

};
// 
class Model
{
public:
  Model() = default;

  ~Model() = default;

public:

  bool
  loadFromFile(const Path& filePath, const UPtr<GraphicsAPI>& pGraphicsAPI);

  Vector<Mesh> m_meshes;

  UPtr<GraphicsBuffer> m_pVertexBuffer;
  UPtr<GraphicsBuffer> m_pIndexBuffer;
};
// 
}

