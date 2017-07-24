#pragma once
#include <stdint.h>

namespace hardware {

class MotorController {
public:
MotorController(uint8_t directionPin, uint8_t enablePin, bool reversed = false);
MotorController(uint8_t output);
  void set(int16_t power);
  void stop();

private:
  uint8_t m_directionPin;
  uint8_t m_enablePin;
  bool m_reversed;
};

}