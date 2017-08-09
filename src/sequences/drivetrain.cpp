#include "sequences/drivetrain.h"
#include "utils/filter.h"
#include <Arduino.h>

using namespace hardware;

namespace sequences {

Drivetrain::Drivetrain(EncoderMotor& leftMotor, EncoderMotor& rightMotor, Qrd& lineSensor) :
  m_leftMotor(leftMotor), m_rightMotor(rightMotor), m_lineSensor(lineSensor), m_beaconFollowPid(BEACON_GAIN_P(), 0,0)
{
  m_command = IDLE;
  m_turning = false;
}

void Drivetrain::commandDriveStraight(float distance, int16_t power) {
  const int32_t ticks = hardware::Encoder::cmToTicks(distance);
  m_leftMotorTarget = m_leftMotor.getPosition() + ticks;
  m_rightMotorTarget = m_rightMotor.getPosition() + ticks;
  m_positionPower = power;
  m_positionMinPower = 80;
  m_command = DRIVE_ENCODER;
  m_turning = false;
}

void Drivetrain::commandTurnLeft(float angle, int16_t power) {
  const int32_t ticks = hardware::Encoder::degToTicks(angle);
  m_leftMotorTarget = m_leftMotor.getPosition();
  m_rightMotorTarget = m_rightMotor.getPosition() + ticks;
  m_positionPower = power;
  m_positionMinPower = 200;
  m_command = DRIVE_ENCODER;
  m_turning = true;
}

void Drivetrain::commandTurnRight(float angle, int16_t power) {
  const int32_t ticks = hardware::Encoder::degToTicks(angle);
  m_leftMotorTarget = m_leftMotor.getPosition() + ticks;
  m_rightMotorTarget = m_rightMotor.getPosition();
  m_positionPower = power;
  m_positionMinPower = 200;
  m_command = DRIVE_ENCODER;
  m_turning = true;
}

void Drivetrain::commandLineFollow(uint8_t speedSetting, bool useRamp) {
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
  if (!useRamp) m_lineFollowPower = m_lineFollowMaxPower;
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

void Drivetrain::setPower(int16_t left, int16_t right, bool useRamp) {
  m_leftPower = left;
  m_rightPower = right;
  m_leftMotor.setPower(left, useRamp);
  m_rightMotor.setPower(right, useRamp);
  m_command = DRIVE_POWER;
}

void Drivetrain::setVelocity(int16_t left, int16_t right) {
  m_leftVelocity = left;
  m_rightVelocity = right;
  m_command = DRIVE_VELOCITY;
}

void Drivetrain::tick() {
  int16_t tapeError = m_lineSensor.getTapeError();
  if (m_command == IDLE) {
    stop();
  }
  else if (m_command == DRIVE_ENCODER) {
    const bool leftDone = m_leftMotor.setPosition(m_leftMotorTarget, m_positionPower, m_positionMinPower);
    const bool rightDone = m_rightMotor.setPosition(m_rightMotorTarget, m_positionPower, m_positionMinPower);
    if (m_turning) {
      if (leftDone && rightDone) m_command = IDLE;
    }
    else {
      if (leftDone || rightDone) m_command = IDLE;
    }
  }
  else if (m_command == DRIVE_LINE_FOLLOW) {
    m_lineFollowPower += 5;
    if (m_lineFollowPower > m_lineFollowMaxPower) m_lineFollowPower = m_lineFollowMaxPower;
    const int16_t steer = tapeError * m_lineFollowGain;
    const int16_t leftPower = m_lineFollowPower - (steer > 0 ? steer : 0);
    const int16_t rightPower = m_lineFollowPower + (steer < 0 ? steer : 0);
    m_leftMotor.setPower(leftPower < 0 ? 0 : leftPower, false);
    m_rightMotor.setPower(rightPower < 0 ? 0 : rightPower, false);
  }
  else if (m_command == DRIVE_VELOCITY) {
    m_leftMotor.setVelocity(m_leftVelocity);
    m_rightMotor.setVelocity(m_rightVelocity);
  }
  else if (m_command == DRIVE_POWER) {
    //m_leftMotor.setPower(m_leftPower);
    //m_rightMotor.setPower(m_rightPower);
  }
  else if (m_command == DRIVE_BEACON_FOLLOW) {
    const int32_t left10k = detect_10khz(L_BEACON_SENSOR());
    const int32_t right10k = detect_10khz(R_BEACON_SENSOR());
    int16_t error = 0;
    if (left10k - right10k > BEACON_UNCERTAINTY()) error = BEACON_GAIN_P();
    if (right10k - left10k > BEACON_UNCERTAINTY()) error = -BEACON_GAIN_P();
    m_leftMotor.setPower(LINE_FOLLOW_0_SPEED() - error);
    m_rightMotor.setPower(LINE_FOLLOW_0_SPEED() + error);
  }
}

}  // namespace sequences
