#pragma once

#include "hardware/dcmotor.h"
#include "hardware/encoder.h"
#include "hardware/linesensor.h"

namespace hardware {

class Drivetrain {
public:
  Drivetrain(DCMotor& leftMotor, DCMotor& rightMotor, const Encoder& leftEncoder, const Encoder& rightEncoder, const LineSensor& LineSensor);

  void commandLineFollowDistance();
  void commandLineFollowIntersection();
  void commandDriveStraight(int32_t distance, int16_t speed);
  void commandTurnLeft();
  void commandTurnRight();
  void commandTurnBoth();
  void stop();
  bool readyForCommand();
  void tick();
  void rampMotors();

private:
  DCMotor& m_leftMotor;
  DCMotor& m_rightMotor;

  int16_t m_leftMotorCurrent;
  int16_t m_leftMotorSetpoint;
  int16_t m_rightMotorCurrent;
  int16_t m_rightMotorSetpoint;


  const Encoder& m_leftEncoder;
  const Encoder& m_rightEncoder;
  const LineSensor& m_lineSensor;

  int32_t m_leftEncoderSetpoint;
  int32_t m_rightEncoderSetpoint;

  int32_t leftEncoderError();
  int32_t rightEncoderError();

  enum Command { IDLE, DRIVE_ENCODER };
  Command m_command;
  int16_t m_power;

  class PIDController {
  public:
    float run(float error);
    void setParameters(float P, float I, float D);
    void init(float error);
    float kP;
    float kI;
    float kD;
  private:
    float prevError;
    float integral;
  };

  PIDController m_leftEncoderPID;
  PIDController m_rightEncoderPID;
  //PIDController m_lineFollowPID;

};

}