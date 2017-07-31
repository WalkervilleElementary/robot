#include "hardware/encoder.h"
#include "utils/encoderinterrupts.h"

namespace hardware {

Encoder::Encoder(uint8_t encoderId, bool reverse) {
  m_encoderId = encoderId;
  m_reverse = reverse;
  setEncoderCount(m_encoderId, 0);
  m_previousPosition = 0;
  m_currentPosition = 0;
}

int32_t Encoder::getPosition() const {
  return m_currentPosition;
}

int32_t Encoder::getVelocity() const {
  return m_currentPosition - m_previousPosition;
}

void Encoder::tick() {
  m_previousPosition = m_currentPosition;
  if (m_reverse) {
    m_currentPosition = -getEncoderCount(m_encoderId);
  }
  else {
    m_currentPosition = getEncoderCount(m_encoderId);
  }
}

void Encoder::printTo(Print& p) {
  p.print(getEncoderPin(m_encoderId, 0));
  p.print(getEncoderPin(m_encoderId, 1));
  p.print(' ');
  p.print(getPosition());
  p.print(' ');
  p.print(getVelocity());
}



}

