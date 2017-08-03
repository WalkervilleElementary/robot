#pragma once
#include "configs.h"

#include "hardware/encodermotor.h"
#include "hardware/qrd.h"
#include "utils/pid.h"

#if DEBUG()
#include <Print.h>
#endif  // DEBUG

namespace sequences {

class Drivetrain {
public:
  Drivetrain(hardware::EncoderMotor& leftMotor, hardware::EncoderMotor& rightMotor, hardware::Qrd& lineSensor);
  inline ~Drivetrain() {};
  //void commandLineFollowDistance();
  //void commandLineFollowIntersection();
  void commandDriveStraight(float distance, int16_t speed = 20);
  void commandTurnLeft(float angle, int16_t speed = 20);
  void commandTurnRight(float angle, int16_t speed = 20);
  //void commandTurnPivot(int32_t distance, int16_t speed = 255);
  void commandLineFollow(uint8_t speedSetting);
  void commandBeaconFollow();
  
  void stop();
  bool readyForCommand();
  void setPower(int16_t left, int16_t right);
  void setVelocity(int16_t left, int16_t right);
  void tick();

#if DEBUG()
  void printTo(Print& p);
#endif  // DEBUG

private:
  utils::PidController m_beaconFollowPid;

  hardware::EncoderMotor& m_leftMotor;
  hardware::EncoderMotor& m_rightMotor;
  hardware::Qrd& m_lineSensor;

  int32_t m_leftMotorTarget;
  int32_t m_rightMotorTarget;
  int16_t m_speed;
  int16_t m_lineFollowPower;
  int16_t m_lineFollowMaxPower;
  int16_t m_lineFollowGain;

  int16_t m_leftPower;
  int16_t m_rightPower;
  int16_t m_leftVelocity;
  int16_t m_rightVelocity;

  enum Command { IDLE, DRIVE_VELOCITY, DRIVE_POWER, DRIVE_ENCODER, DRIVE_LINE_FOLLOW, DRIVE_BEACON_FOLLOW };
  Command m_command;

};  // Class Drivetrain

}  // namespace sequences
