#include "hardware/drivetrain.h"

namespace hardware {

Drivetrain::Drivetrain(EncoderMotor& leftMotor, EncoderMotor& rightMotor) :
  m_leftMotor(leftMotor), m_rightMotor(rightMotor)
{
  m_command = IDLE;
}

void Drivetrain::commandDriveStraight(int32_t distance, int16_t speed) {
  m_leftMotorTarget = m_leftMotor.getPosition() + distance;
  m_rightMotorTarget = m_rightMotor.getPosition() + distance;
  m_speed = speed;
  m_command = DRIVE_ENCODER;
}

bool Drivetrain::readyForCommand() {
  return m_command == IDLE;
}

void Drivetrain::stop() {
  m_leftMotor.stop();
  m_rightMotor.stop();
  m_command = IDLE;
}

void Drivetrain::tick() {
  if (m_command == IDLE) {
    stop();
  }
  else if (m_command == DRIVE_ENCODER) {
    bool leftDone = m_leftMotor.setPosition(m_leftMotorTarget, m_speed);
    bool rightDone = m_rightMotor.setPosition(m_rightMotorTarget, m_speed);
    if (leftDone && rightDone) m_command = IDLE;
  } 
}

}