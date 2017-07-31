#pragma once
#include <stdint.h>

namespace hardware {

class Encoder {
public:
  Encoder(uint8_t encoderId, bool reverse = false);
  int32_t getPosition() const;
  int32_t getVelocity() const;
  void tick();
private:
  uint8_t m_encoderId;
  bool m_reverse;
  int32_t m_currentPosition;
  int32_t m_previousPosition;
};

}