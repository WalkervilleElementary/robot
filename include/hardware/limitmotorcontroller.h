#pragma once
#include <stdint.h>
#include "hardware/motorcontroller.h"
#include "hardware/switch.h"

namespace hardware {

class LimitMotorController {
public:
  LimitMotorController(MotorController& motor, const Switch& extendLimit, const Switch& retractLimit);

  bool extend(int16_t power);
  bool retract(int16_t power);
  void stop();
  void tick();

private:
  MotorController& m_motor;
  const Switch& m_extendLimit;
  const Switch& m_retractLimit;
  int16_t m_motorPower;
  enum state { INACTIVE, EXTEND, RETRACT };
  state m_state;

};

}