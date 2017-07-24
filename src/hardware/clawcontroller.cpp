#include "hardware/clawcontroller.h"

ClawController::ClawController(ServoTINAH& liftServo, ServoTINAH& grabServo, int16_t liftAngles[5], int16_t grabAngles[5]) :
  m_liftServo(liftServo), m_grabServo(grabServo)
{
  for (size_t i = 0; i < 5; i++) {
    m_liftAngles[i] = liftAngles[i];
  }
  for (size_t i = 0; i < 2; i++) {
    m_grabAngles[i] = grabAngles[i];
  }
  m_currentLiftAngle = 0;
  m_currentGrabAngle = 0;
}

void ClawController::tick() {
  if (m_currentLiftAngle > m_liftAngleTarget + m_liftSpeed) {
    m_currentLiftAngle -= liftSpeed;
  }
  else if (m_currentLiftAngle < m_liftAngleTarget - m_liftSpeed)
}

void ClawController::setLift(LiftPosition position) {
  m_liftServo.write(m_liftAngles[position]);
  m_liftAngleTarget = m_liftAngles[position];
}

void ClawController::setGrab(GrabPosition position) {
  m_grabServo.write(m_grabAngles[position]);
  m_grabAngleTarget = m_grabAngles[position];
}