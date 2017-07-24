#pragma once
#include <stdint.h>
#include <ServoTINAH.h>

class ClawController {
public:
  ClawController(ServoTINAH& liftServo, ServoTINAH& grabServo, uint8_t liftAngles[5], uint8_t grabAngles[5]);
  enum LiftPosition { FOLDED, VERTICAL, TOP, MIDDLE, BOTTOM };
  enum GrabPosition { OPEN, CLOSED };
  void setLift(LiftPosition position);
  void setGrab(GrabPosition position);
private:
  ServoTINAH& m_liftServo;
  ServoTINAH& m_grabServo;
  uint8_t m_liftAngles[5];
  uint8_t m_grabAngles[2]; 

};