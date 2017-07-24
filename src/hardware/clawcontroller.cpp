#include "hardware/clawcontroller.h"

ClawController::ClawController(ServoTINAH& liftServo, ServoTINAH& grabServo, uint8_t liftAngles[5], uint8_t grabAngles[5]) :
  m_liftServo(liftServo), m_grabServo(grabServo)
{
  for (size_t i = 0; i < 5; i++) {
    m_liftAngles[i] = liftAngles[i];
  }
  for (size_t i = 0; i < 2; i++) {
    m_grabAngles[i] = grabAngles[i];
  }
}

void ClawController::setLift(LiftPosition position) {
  m_liftServo.write(m_liftAngles[position]);
}

void ClawController::setGrab(GrabPosition position) {
  m_grabServo.write(m_grabAngles[position]);
}