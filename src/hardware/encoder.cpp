#include "hardware/encoder.h"
#include "utils/encoderinterrupts.h"

namespace hardware {

Encoder::Encoder(uint8_t encoderId, bool reverse) {
  m_encoderId = encoderId;
  m_reverse = reverse;
  setEncoderCount(m_encoderId, 0);
  m_previousDisplacement = 0;
  m_currentDisplacement = 0;
}

int32_t Encoder::getDisplacement() const {
  return m_currentDisplacement;
}

int32_t Encoder::getVelocity() const {
  return m_currentDisplacement - m_previousDisplacement;
}

void Encoder::tick() {
  m_previousDisplacement = m_currentDisplacement;
  if (m_reverse) {
    m_currentDisplacement = -getEncoderCount(m_encoderId);
  }
  else {
    m_currentDisplacement = getEncoderCount(m_encoderId);
  }
}



}

