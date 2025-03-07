#include "mdGraphicsBuffer.h"

namespace MD {
  GraphicsBuffer::~GraphicsBuffer() {
    SAFE_RELEASE(m_pBuffer);
  }
}
