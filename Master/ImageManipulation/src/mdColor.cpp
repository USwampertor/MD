#include "mdColor.h"

namespace MD
{
  float&
  Color::operator()(const uint32_t& index) {
    return (&m_r)[index];
  }

  const float&
  Color::operator()(const uint32_t& index) const {
    return (&m_r)[index];
  }

  float&
  Color::operator[](const uint32_t& index) {
    return (&m_r)[index];
  }

  const float
  Color::operator[](const uint32_t& index) const {
    return (&m_r)[index];
  }

  bool
  Color::operator==(const Color& other) const {
    return Utils::isNearSame(m_r, other.m_r) &&
           Utils::isNearSame(m_g, other.m_g) &&
           Utils::isNearSame(m_b, other.m_b) &&
           Utils::isNearSame(m_a, other.m_a);
  }

  void
  Color::setFloat(const float& nr, 
                  const float& ng, 
                  const float& nb, 
                  const float& na /* = 1.0f */) {
    m_r = std::clamp(nr, 0.0f, 1.0f);
    m_g = std::clamp(ng, 0.0f, 1.0f);
    m_b = std::clamp(nb, 0.0f, 1.0f);
    m_a = std::clamp(na, 0.0f, 1.0f);
  }

  void
  Color::setUint(const uint32_t& nr, 
                 const uint32_t& ng, 
                 const uint32_t& nb, 
                 const uint32_t& na /* = 255 */) {
    m_r = std::clamp(static_cast<uint32_t>(nr), 
                     static_cast<uint32_t>(0), 
                     static_cast<uint32_t>(255)) * Color::Int2Linear;
    m_g = std::clamp(static_cast<uint32_t>(ng), 
                     static_cast<uint32_t>(0), 
                     static_cast<uint32_t>(255)) * Color::Int2Linear;
    m_b = std::clamp(static_cast<uint32_t>(nb), 
                     static_cast<uint32_t>(0), 
                     static_cast<uint32_t>(255)) * Color::Int2Linear;
    m_a = std::clamp(static_cast<uint32_t>(na), 
                     static_cast<uint32_t>(0), 
                     static_cast<uint32_t>(255)) * Color::Int2Linear;
  }

  String
  Color::toString() {
    return Utils::format("(R: %2.2f, G: %2.2f, B: %2.2f, A: %2.2f)", m_r, m_g, m_b, m_a);
  }

  uint32_t
  Color::toHexValue() {
    int32_t nr = static_cast<int32_t>(m_r * 255.0f);
    int32_t ng = static_cast<int32_t>(m_g * 255.0f);
    int32_t nb = static_cast<int32_t>(m_b * 255.0f);
    int32_t na = static_cast<int32_t>(m_a * 255.0f);

    return (nr << 24) | (ng << 16) | (nb << 8) | na;
  }

  void
  Color::fromHex(const uint32_t& hexValue) {
    m_r = ((hexValue >> 24) & 0xFF) * Color::Int2Linear; // Red
    m_g = ((hexValue >> 16) & 0xFF) * Color::Int2Linear; // Green
    m_b = ((hexValue >> 8) & 0xFF)  * Color::Int2Linear; // Blue
    m_a = (hexValue & 0xFF)         * Color::Int2Linear; // Alpha
  }

  String
  Color::toHexString() {
    uint32_t hex = toHexValue();
    return Utils::format("#%x");
  }

  const Color Color::BLACK    = Color(0.00f, 0.00f, 0.00f, 1.00f);
  const Color Color::CLEAR    = Color(0.00f, 0.00f, 0.00f, 0.50f);
  const Color Color::AZURE    = Color(0.94f, 1.00f, 1.00f);
  const Color Color::BLUE     = Color(0.00f, 0.00f, 1.00f);
  const Color Color::CYAN     = Color(0.00f, 1.00f, 1.00f);
  const Color Color::GRAY     = Color(0.50f, 0.50f, 0.50f);
  const Color Color::GREEN    = Color(0.00f, 1.00f, 0.00f);
  const Color Color::GREY     = Color(0.39f, 0.39f, 0.39f);
  const Color Color::MAGENTA  = Color(1.00f, 0.00f, 1.00f);
  const Color Color::RED      = Color(1.00f, 0.00f, 0.00f);
  const Color Color::ORANGE   = Color(1.00f, 0.39f, 0.00f);
  const Color Color::VIOLET   = Color(0.55f, 0.00f, 0.82f);
  const Color Color::WHITE    = Color(1.00f, 1.00f, 1.00f);
  const Color Color::YELLOW   = Color(1.00f, 1.00f, 0.00f);
  const Color Color::MISSING  = Color(1.00f, 0.00f, 0.86f);

}
