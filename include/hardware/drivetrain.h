#pragma once

#include "hardware/encodermotor.h"
#include "hardware/qrd.h"
#include "utils/pid.h"
#include <Print.h>

namespace hardware {

class Drivetrain {
public:
  Drivetrain(EncoderMotor& leftMotor, EncoderMotor& rightMotor, Qrd& lineSensor);

  //void commandLineFollowDistance();
  //void commandLineFollowIntersection();
  void commandDriveStraight(int32_t distance, int16_t speed = 255);
  void commandTurnLeft(int32_t distance, int16_t speed = 255);
  void commandTurnRight(int32_t distance, int16_t speed = 255);
  void commandTurnPivot(int32_t distance, int16_t speed = 255);
  void commandLineFollow(int16_t power);
  void stop();
  bool readyForCommand();
  void tick();
  void printTo(Print& p);
  PidController m_lineFollowPid;

private:
  EncoderMotor& m_leftMotor;
  EncoderMotor& m_rightMotor;
  Qrd& m_lineSensor;

  int32_t m_leftMotorTarget;
  int32_t m_rightMotorTarget;
  int16_t m_speed;
  int16_t m_lineFollowPower;

  enum Command { IDLE, DRIVE_ENCODER, DRIVE_LINE_FOLLOW };
  Command m_command;

  //PidController m_beaconFollowPid;

};

}
