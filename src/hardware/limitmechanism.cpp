#include "hardware/limitmechanism.h"

namespace hardware {

LimitMechanism::LimitMechanism(DCMotor& motor, const Switch& extendLimit, const Switch& retractLimit) :
  m_motor(motor), m_extendLimit(extendLimit), m_retractLimit(retractLimit)
{
  m_state = INACTIVE;
}

bool LimitMechanism::extend(int16_t power) {
  setState(EXTEND, power);
  return m_extendLimit.get();
}

bool LimitMechanism::retract(int16_t power) {
  setState(RETRACT, power);
  return m_retractLimit.get();
}

void LimitMechanism::stop() {
  setState(INACTIVE, 0);
  m_motor.stop();
}

void LimitMechanism::setState(State state, int16_t power) {
  m_motorPower = power;
  m_state = state;
}

void LimitMechanism::tick() {
  if (m_state == INACTIVE) {
    m_motor.stop();
  }
  else if (m_state == EXTEND) {
    if (m_extendLimit.get()) m_motor.stop();
    else m_motor.setPower(m_motorPower);
  }
  else if (m_state == RETRACT) {
    if (m_retractLimit.get()) m_motor.stop();
    else m_motor.setPower(-m_motorPower);
  }
}

}