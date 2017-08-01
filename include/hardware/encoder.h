#pragma once
#include "configs.h"
#include <stdint.h>
#include "Print.h"

namespace hardware {

class Encoder {
public:
  Encoder(uint8_t encoderId, bool reverse = false);
  inline ~Encoder() {};
  int32_t getPosition() const;
  int16_t getVelocity() const;
  void tick();
  void printTo(Print& p);
  static int32_t cmToTicks(float cm);
  static int32_t degToTicks(float deg);

private:
  int32_t getPositionPrivate();
  uint8_t m_encoderId;
  bool m_reverse;
  int32_t m_currentPosition;
  int32_t m_previousPosition;
  static const int16_t m_numVelocitySamples = 5;
  int16_t m_velocitySamples[m_numVelocitySamples];
  uint8_t m_currentVelocitySample;
};

}
