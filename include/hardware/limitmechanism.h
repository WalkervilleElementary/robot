#pragma once
#include <stdint.h>
#include "hardware/dcmotor.h"
#include "hardware/switch.h"

namespace hardware {

class LimitMechanism {
public:
  LimitMechanism(DCMotor& motor, const Switch& extendLimit, const Switch& retractLimit);

  enum State { INACTIVE, EXTEND, RETRACT, BACKOFF };
  bool extend(int16_t power);
  bool retract(int16_t power);
  bool backOff(int16_t power);
  void setState(State state, int16_t power);
  void stop();
  void tick();

private:
  DCMotor& m_motor;
  const Switch& m_extendLimit;
  const Switch& m_retractLimit;
  int16_t m_motorPower;
  State m_state;

};

}