#pragma once
#include <stdint.h>
#include <ServoTINAH.h>

class ClawController {
public:
  ClawController(ServoTINAH& liftServo, ServoTINAH& grabServo, uint8_t liftAngles[5], uint8_t grabAngles[5]);
  enum LiftPosition { FOLDED = 0, VERTICAL = 1, TOP = 2, MIDDLE = 3, BOTTOM = 4 };
  enum GrabPosition { OPEN = 0, CLOSED = 1 };
  bool setLift(LiftPosition position);
  bool setGrab(GrabPosition position);
  void tick();
private:
  ServoTINAH& m_liftServo;
  ServoTINAH& m_grabServo;
  uint8_t m_liftAngles[5];
  uint8_t m_grabAngles[2]; 

  uint8_t m_currentLiftAngle;
  uint8_t m_liftAngleTarget;
  uint8_t m_currentGrabAngle;
  uint8_t m_grabAngleTarget;
  const uint8_t m_liftSpeed = 10;
  const uint8_t m_grabSpeed = 10;

};