#include "hardware/drivetrain.h"

namespace hardware {

Drivetrain::Drivetrain(MotorController& leftMotor, MotorController& rightMotor, const Encoder& leftEncoder, const Encoder& rightEncoder, const LineSensor& lineSensor) :
  m_leftMotor(leftMotor), m_rightMotor(rightMotor), m_leftEncoder(leftEncoder), m_rightEncoder(rightEncoder), m_lineSensor(lineSensor)
{
  stop();
  m_leftEncoderPID.setParameters(1,0,0);
  m_rightEncoderPID.setParameters(1,0,0);
}

void Drivetrain::commandDriveStraight(int32_t distance, int16_t speed) {
  m_leftEncoderSetpoint = m_leftEncoder.getDisplacement() + distance;
  m_rightEncoderSetpoint = m_rightEncoder.getDisplacement() + distance;
  m_command == DRIVE_ENCODER;
}

bool Drivetrain::readyForCommand() {
  return m_command == IDLE;
}

void Drivetrain::stop() {
  m_leftMotorCurrent = 0;
  m_rightMotorCurrent = 0;
  m_leftMotorSetpoint = 0;
  m_rightMotorSetpoint = 0;
  m_leftMotor.stop();
  m_rightMotor.stop();
  m_command = IDLE;
}

int16_t rampClamp(int16_t current, int16_t setpoint, int16_t maxDelta) {
  int16_t delta = setpoint - current;
  if (delta > maxDelta) delta = maxDelta;
  if (delta < -maxDelta) delta = -maxDelta;
  return current + delta;
}

void Drivetrain::rampMotors() {
  int16_t rampAmount = 32;
  m_leftMotorCurrent = rampClamp(m_leftMotorCurrent, m_leftMotorSetpoint, rampAmount);
  m_rightMotorCurrent = rampClamp(m_rightMotorCurrent, m_rightMotorSetpoint, rampAmount);
}

void Drivetrain::tick() {
  if (m_command == IDLE) {
    stop();
  }
  else if (m_command == DRIVE_ENCODER) {
    m_leftMotorSetpoint = m_leftEncoderPID.run(leftEncoderError());
    m_rightMotorSetpoint = m_rightEncoderPID.run(rightEncoderError());
    rampMotors();
  }
}

int32_t Drivetrain::leftEncoderError() {
  return m_leftEncoder.getDisplacement() - m_leftEncoderSetpoint;
}

int32_t Drivetrain::rightEncoderError() {
  return m_leftEncoder.getDisplacement() - m_leftEncoderSetpoint;
}

void Drivetrain::PIDController::setParameters(float P, float I, float D) {
  kP = P;
  kI = I;
  kD = D;
}

void Drivetrain::PIDController::init(float error) {
  integral = 0;
  prevError = error;
}

float Drivetrain::PIDController::run(float error) {
  integral = (integral + error) * 0.95;
  float derivative = prevError - error;
  prevError = error;
  return kP * error + kI * integral + kD * derivative;
}

}