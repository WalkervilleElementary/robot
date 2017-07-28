#pragma once
#include <stdint.h>
#include "hardware/servomotor.h"

namespace hardware {

class ClawMechanism {
public:
  ClawMechanism(ServoMotor& liftServo, ServoMotor& grabServo, int16_t liftAngles[5], int16_t grabAngles[5]);
  enum LiftPosition { FOLDED = 0, VERTICAL = 1, TOP = 2, MIDDLE = 3, BOTTOM = 4 };
  enum GrabPosition { OPEN = 0, CLOSED = 1 };
  bool setLift(LiftPosition position);
  bool setGrab(GrabPosition position);
  void tick();
private:
  ServoMotor& m_liftServo;
  ServoMotor& m_grabServo;
  int16_t m_liftAngles[5];
  int16_t m_grabAngles[2]; 

};

}