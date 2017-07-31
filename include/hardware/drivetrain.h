#pragma once

#include "hardware/encodermotor.h"
#include "hardware/linesensor.h"
#include "utils/pid.h"

namespace hardware {

class Drivetrain {
public:
  Drivetrain(EncoderMotor& leftMotor, EncoderMotor& rightMotor);

  //void commandLineFollowDistance();
  //void commandLineFollowIntersection();
  void commandDriveStraight(int32_t distance, int16_t speed = 255);
  void commandTurnLeft(int32_t distance, int16_t speed = 255);
  void commandTurnRight(int32_t distance, int16_t speed = 255);
  void commandTurnPivot(int32_t distance, int16_t speed = 255);
  void stop();
  bool readyForCommand();
  void tick();

private:
  EncoderMotor& m_leftMotor;
  EncoderMotor& m_rightMotor;

  int32_t m_leftMotorTarget;
  int32_t m_rightMotorTarget;
  int16_t m_speed;

  enum Command { IDLE, DRIVE_ENCODER };
  Command m_command;

  //PidController m_lineFollowPid;
  //PidController m_beaconFollowPid;

};

}