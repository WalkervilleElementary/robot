#include "configs.h"

#include "hardware/drivetrain.h"

namespace hardware {

Drivetrain::Drivetrain(EncoderMotor& leftMotor, EncoderMotor& rightMotor, Qrd& lineSensor) :
  m_leftMotor(leftMotor), m_rightMotor(rightMotor), m_lineSensor(lineSensor), m_lineFollowPid(GAIN_P(), GAIN_I(), GAIN_D(), 0.95)
{
  m_command = IDLE;
}

void Drivetrain::commandDriveStraight(int32_t distance, int16_t speed) {
  m_leftMotorTarget = m_leftMotor.getPosition() + distance;
  m_rightMotorTarget = m_rightMotor.getPosition() + distance;
  m_speed = speed;
  m_command = DRIVE_ENCODER;
}

void Drivetrain::commandLineFollow(int16_t power) {
  m_lineFollowPower = power;
  m_command = DRIVE_LINE_FOLLOW;
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
  else if (m_command == DRIVE_LINE_FOLLOW) {
    int32_t result = (int32_t)m_lineFollowPid.run(m_lineSensor.getTapeError());
    m_leftMotor.setPower(m_lineFollowPower - (result > 0 ? abs(result) : 0));
    m_rightMotor.setPower(m_lineFollowPower - (result < 0 ? abs(result) : 0));
  }
}

void Drivetrain::printTo(Print& p) {
  p.print(m_lineFollowPid.kP);
  p.print(' ');
  p.print(m_lineFollowPid.kI);
}

}
