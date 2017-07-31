#include "hardware/clawmechanism.h"

namespace hardware {

ClawMechanism::ClawMechanism(ServoMotor& liftServo, ServoMotor& grabServo, int16_t liftAngles[5], int16_t grabAngles[5]) :
  m_liftServo(liftServo), m_grabServo(grabServo)
{
  for (size_t i = 0; i < 5; i++) {
    m_liftAngles[i] = liftAngles[i];
  }
  for (size_t i = 0; i < 2; i++) {
    m_grabAngles[i] = grabAngles[i];
  }
}

void ClawMechanism::tick() {

}

bool ClawMechanism::setLift(LiftPosition position) {
  return m_liftServo.setPosition(m_liftAngles[position]);
}

bool ClawMechanism::setGrab(GrabPosition position) {
  return m_grabServo.setPosition(m_grabAngles[position]);
}

}