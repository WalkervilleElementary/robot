#pragma once
#include <stdint.h>

namespace hardware {

class DCMotor {
public:
DCMotor(uint8_t directionPin, uint8_t enablePin, bool reversed);
DCMotor(uint8_t output, bool reversed);
  void setPower(int16_t power);
  void stop();

private:
  uint8_t m_directionPin;
  uint8_t m_enablePin;
  bool m_reversed;
};

}