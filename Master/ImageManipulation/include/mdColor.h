#pragma once

#include "mdImageManipulationPrerequisites.h"

namespace MD
{

class Color
{
public:

  Color() = default;

  ~Color() = default;

  Color(float r, float g = 0, float b = 0, float a = 1.0f)
    : m_r(r),
      m_g(g),
      m_b(b),
      m_a(a) {}


  Color(const Color& copy) 
    : m_r(copy.m_r),
      m_g(copy.m_g),
      m_b(copy.m_b),
      m_a(copy.m_a) {}

  const float&
  operator()(const uint32_t& index) const;
    

  float&
  operator()(const uint32_t& index);
    

  const float
  operator[](const uint32_t& index) const;

  float&
  operator[](const uint32_t& index);

  bool
  operator==(const Color& other) const;

  void
  setFloat(const float& nr, 
           const float& ng, 
           const float& nb, 
           const float& na = 1.0f);

  void
  setUint(const uint32_t& nr, 
          const uint32_t& ng, 
          const uint32_t& nb, 
          const uint32_t& na = 255);

  String
  toString();

  uint32_t
  toHexValue();

  void
  fromHex(const uint32_t& hexValue);

  String
  toHexString();

  static const Color AZURE;
  static const Color BLACK;
  static const Color BLUE;
  static const Color CLEAR;
  static const Color CYAN;
  static const Color GRAY;
  static const Color GREEN;
  static const Color GREY;
  static const Color MAGENTA;
  static const Color RED;
  static const Color ORANGE;
  static const Color VIOLET;
  static const Color WHITE;
  static const Color YELLOW;
  static const Color MISSING;

  static constexpr float Int2Linear = 1.0f / 255.0f;
  static constexpr float Linear2Int = 255.0f;

public:

  float m_r;
  float m_g;
  float m_b;
  float m_a;

};

}

