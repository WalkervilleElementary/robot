#include "hardware/encoder.h"
#include "utils/encoderinterrupts.h"
#include "math.h"

namespace hardware {

float Encoder::distance_to_ticks = GEAR_RATIO() * ENCODER_TICKS_PER_REVOLUTION() / (WHEEL_DIAMETER() * M_PI);
float Encoder::degrees_to_ticks = AXLE_LENGTH() * M_PI / 180.0;

int32_t Encoder::cmToTicks(float cm) {
  return cm * distance_to_ticks;
}

int32_t Encoder::degToTicks(float deg) {
  return cmToTicks(deg * degrees_to_ticks);
}

Encoder::Encoder(uint8_t encoderId, bool reverse) {
  m_encoderId = encoderId;
  m_reverse = reverse;
  setEncoderCount(m_encoderId, 0);
  m_previousPosition = getPositionPrivate();
  m_currentPosition = getPositionPrivate();
  for (int i = 0; i < m_numVelocitySamples; i++) {
    m_velocitySamples[i] = 0;
  }
}

int32_t Encoder::getPosition() const {
  return m_currentPosition;
}

int16_t Encoder::getVelocity() const {
  int16_t velocity = 0;
  for (int i = 0; i < m_numVelocitySamples; i++) velocity += m_velocitySamples[i];
  return velocity;
}

void Encoder::tick() {
  m_previousPosition = m_currentPosition;
  m_currentPosition = getPositionPrivate();
  m_velocitySamples[m_currentVelocitySample++] = m_currentPosition - m_previousPosition;
  if (m_currentVelocitySample >= m_numVelocitySamples) m_currentVelocitySample = 0;
}

#if DEBUG()
void Encoder::printTo(Print& p) {
  p.print(getEncoderPin(m_encoderId, 0));
  p.print(getEncoderPin(m_encoderId, 1));
  p.print(' ');
  p.print(getPosition());
  p.print(' ');
  p.print(getVelocity());
}
#endif  // DEBUG

int32_t Encoder::getPositionPrivate() {
  if (m_reverse) return -getEncoderCount(m_encoderId);
  else return getEncoderCount(m_encoderId);
}


}  // namespace hardawre

