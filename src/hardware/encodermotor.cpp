#include "hardware/encodermotor.h"
#include "phys253.h"
#include "configs.h"

using namespace utils;

namespace hardware {

EncoderMotor::EncoderMotor(int motor_num, Encoder& encoder, bool backward) :
  m_motor_num(motor_num), m_encoder(encoder), m_backward(backward), 
  m_velocityPid(-0.4,0.0,0.0), m_positionPid(-0.2,0.0,-0.2), // TODO make configurable
  m_deadZone(5), m_accelerationLimit(5)
{
  m_currentPower = 0;
  m_powerTarget = 0;
  m_velocityTarget = 0;
  m_positionTarget = m_encoder.getPosition();
}

void EncoderMotor::setPower(int16_t power, bool useRamp) {
  if (m_controlMode != POWER) m_currentPower = 0;
  m_controlMode = POWER;
  m_powerTarget = power;
  if (!useRamp) m_currentPower = m_powerTarget;
}

void EncoderMotor::setVelocity(int16_t velocity, int16_t power) {
  if (m_controlMode != VELOCITY) m_powerTarget = 0;
  m_controlMode = VELOCITY;
  m_velocityTarget = velocity;
  m_maxPower = power;
}

bool EncoderMotor::setPosition(int32_t position, int16_t speed) {
  if (m_controlMode != POSITION) m_powerTarget = 0;
  m_controlMode = POSITION;
  m_positionTarget = position;
  m_maxVelocity = speed;
  m_maxPower = 255;
  if (abs(m_encoder.getPosition() - m_positionTarget) < m_deadZone) return true;
  else return false;
}

void EncoderMotor::stop() {
  m_controlMode = IDLE;
  motor.stop(m_motor_num);
}

void EncoderMotor::hold() {
  setPosition(m_encoder.getPosition(), 20);
}

void EncoderMotor::tick() {
  switch (m_controlMode) {
    case POSITION:
    {
      int32_t error = m_encoder.getPosition() - m_positionTarget;
      int16_t velocity = m_maxVelocity;
      if (abs(error) < 100) velocity = 5;
      if (abs(error) < 5) velocity = 0;
      if (error < 0) m_velocityTarget = velocity;
      if (error > 0) m_velocityTarget = -velocity;
      //fall through
    }
    case VELOCITY:
      m_powerTarget += m_velocityPid.run(m_encoder.getVelocity() - m_velocityTarget);
      m_powerTarget = constrain(m_powerTarget, -m_maxPower, m_maxPower);
      m_currentPower = m_powerTarget; //ignore motor ramping
      //fall through
    case POWER:
      if (m_currentPower < m_powerTarget - m_accelerationLimit) m_currentPower += m_accelerationLimit;
      else if (m_currentPower > m_powerTarget + m_accelerationLimit) m_currentPower -= m_accelerationLimit;
      else m_currentPower = m_powerTarget;
      if (m_backward) motor.speed(m_motor_num, -m_currentPower);
      else  motor.speed(m_motor_num, m_currentPower);
      break;
    case IDLE:
      motor.stop(m_motor_num);
      break;
  }
}

int32_t EncoderMotor::getPosition() {
  return m_encoder.getPosition();
}

} // namespace hardware
