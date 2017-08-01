#include "configs.h"

#include "sequences/drivetrain.h"
#include "utils/filter.h"
#include <Arduino.h>

using namespace hardware;

namespace sequences {

Drivetrain::Drivetrain(EncoderMotor& leftMotor, EncoderMotor& rightMotor, Qrd& lineSensor) :
  m_leftMotor(leftMotor), m_rightMotor(rightMotor), m_lineSensor(lineSensor), m_lineFollowPid(GAIN_P(), 0., 0., 0.95), m_beaconFollowPid(BEACON_GAIN_P(), 0,0)
{
  m_command = IDLE;
}

void Drivetrain::commandDriveStraight(float distance, int16_t speed) {
  int32_t ticks = hardware::Encoder::cmToTicks(distance);
  m_leftMotorTarget = m_leftMotor.getPosition() + ticks;
  m_rightMotorTarget = m_rightMotor.getPosition() + ticks;
  m_speed = speed;
  m_command = DRIVE_ENCODER;
}

void Drivetrain::commandTurnLeft(float angle, int16_t speed) {
  int32_t ticks = hardware::Encoder::degToTicks(angle);
  m_leftMotorTarget = m_leftMotor.getPosition();
  m_rightMotorTarget = m_rightMotor.getPosition() + ticks;
  m_speed = speed;
  m_command = DRIVE_ENCODER;
}

void Drivetrain::commandTurnRight(float angle, int16_t speed) {
  int32_t ticks = hardware::Encoder::degToTicks(angle);
  m_leftMotorTarget = m_leftMotor.getPosition() + ticks;
  m_rightMotorTarget = m_rightMotor.getPosition();
  m_speed = speed;
  m_command = DRIVE_ENCODER;
}

void Drivetrain::commandLineFollow(uint8_t speedSetting) {
  if (speedSetting == 2) {
    m_lineFollowMaxPower = LINE_FOLLOW_2_SPEED();
    m_lineFollowGain = LINE_FOLLOW_2_GAIN();
  }
  else if (speedSetting == 1) {
    m_lineFollowMaxPower = LINE_FOLLOW_1_SPEED();
    m_lineFollowGain = LINE_FOLLOW_1_GAIN();
  }
  else {
    m_lineFollowMaxPower = LINE_FOLLOW_0_SPEED();
    m_lineFollowGain = LINE_FOLLOW_0_GAIN();
  }
  if (m_command != DRIVE_LINE_FOLLOW) m_lineFollowPower = 0;
  m_command = DRIVE_LINE_FOLLOW;
}

void Drivetrain::commandBeaconFollow() {
  m_command = DRIVE_BEACON_FOLLOW;
}

bool Drivetrain::readyForCommand() {
  return m_command == IDLE;
}

void Drivetrain::stop() {
  m_leftMotor.stop();
  m_rightMotor.stop();
  m_command = IDLE;
}

void Drivetrain::setPower(int16_t left, int16_t right) {
  m_leftPower = left;
  m_rightPower = right;
  m_command = DRIVE_POWER;
}

void Drivetrain::setVelocity(int16_t left, int16_t right) {
  m_leftVelocity = left;
  m_rightVelocity = right;
  m_command = DRIVE_VELOCITY;
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
    m_lineFollowPower += 5;
    if (m_lineFollowPower > m_lineFollowMaxPower) m_lineFollowPower = m_lineFollowMaxPower;
    int16_t error = m_lineSensor.getTapeError();
    int16_t steer = error * m_lineFollowGain;
    int16_t leftPower = m_lineFollowPower - (steer > 0 ? abs(steer) : 0);
    int16_t rightPower = m_lineFollowPower - (steer < 0 ? abs(steer) : 0);
    m_leftMotor.setPower(leftPower < 0 ? 0 : leftPower);
    m_rightMotor.setPower(rightPower < 0 ? 0 : rightPower);
  }
  else if (m_command == DRIVE_VELOCITY) {
    m_leftMotor.setVelocity(m_leftVelocity);
    m_rightMotor.setVelocity(m_rightVelocity);
  }
  else if (m_command == DRIVE_POWER) {
    m_leftMotor.setPower(m_leftPower);
    m_rightMotor.setPower(m_rightPower);
  }
  else if (m_command == DRIVE_BEACON_FOLLOW) {
    int32_t left10k = detect_10khz(L_BEACON_SENSOR());
    int32_t right10k = detect_10khz(R_BEACON_SENSOR());
    int16_t error = 0;
    if (left10k - right10k > BEACON_UNCERTAINTY()) error = BEACON_GAIN_P();
    if (right10k - left10k > BEACON_UNCERTAINTY()) error = -BEACON_GAIN_P();
    m_leftMotor.setPower(LINE_FOLLOW_0_SPEED() - error);
    m_rightMotor.setPower(LINE_FOLLOW_0_SPEED() + error);
  }
}

void Drivetrain::printTo(Print& p) {
  p.print(m_lineFollowPid.kP);
  p.print(' ');
  p.print(m_lineFollowPid.kI);
}

}
