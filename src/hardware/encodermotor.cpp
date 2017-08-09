#include "hardware/encodermotor.h"
#include "phys253.h"
#include "configs.h"

using namespace utils;

namespace hardware {

EncoderMotor::EncoderMotor(int motor_num, Encoder& encoder, bool backward) :
  m_motor_num(motor_num), m_encoder(encoder), m_backward(backward), 
  m_velocityPid(-0.4,0.0,0.0), m_positionPid(-0.8,0,2), // TODO make configurable
  m_deadZone(10), m_accelerationLimit(7)
{
  m_currentPower = 0;
  m_powerTarget = 0;
  m_velocityTarget = 0;
  m_minPower = 0;
  m_maxPower = 255;
  m_positionTarget = m_encoder.getPosition();
}

void EncoderMotor::setPower(int16_t power, bool useRamp) {
  m_controlMode = POWER;
  m_powerTarget = power;
  m_minPower = 0;
  m_maxPower = 255;
  if (!useRamp) m_currentPower = m_powerTarget;
}

void EncoderMotor::setVelocity(int16_t velocity, int16_t power) {
  m_controlMode = VELOCITY;
  m_velocityTarget = velocity;
  m_maxPower = power;
  m_minPower = 0;
}

bool EncoderMotor::setPosition(int32_t position, int16_t maxPower, int16_t minPower) {
  m_controlMode = POSITION;
  m_positionTarget = position;
  m_maxPower = maxPower;
  m_minPower = minPower;
  if (abs(m_encoder.getPosition() - m_positionTarget) < m_deadZone) return true;
  else return false;
}

void EncoderMotor::stop() {
  m_controlMode = OFF;
  motor.stop(m_motor_num);
}

void EncoderMotor::tick() {
  switch (m_controlMode) {
    case POSITION:
    case VELOCITY:
      if (m_controlMode == POSITION) {
        int32_t error = m_encoder.getPosition() - m_positionTarget;
        m_powerTarget = m_positionPid.run(error);
        if (abs(error) < m_deadZone) m_powerTarget = 0;
      }
      else if (m_controlMode == VELOCITY) {
        m_powerTarget += m_velocityPid.run(m_encoder.getVelocity() - m_velocityTarget);
      }
      m_powerTarget = constrain(m_powerTarget, -m_maxPower, m_maxPower);
      //position or velocity sets power target then falls through to power case
    case POWER: {
      /*if ((m_currentPower > 0 && m_powerTarget < 0) || (m_currentPower > 0 && m_powerTarget < 0)) {
        m_currentPower = 0;
      }
      else {
        if (abs(m_powerTarget) - abs(m_currentPower) > 0) {
          constrain(m_powerTarget, m_currentPower - m_accelerationLimit, m_currentPower + m_accelerationLimit);
        }
        else {
          m_currentPower = m_powerTarget;
        }
      }*/
      if ((m_currentPower > 0 && m_powerTarget < m_currentPower) || (m_currentPower < 0 && m_powerTarget > m_currentPower)) {
        m_currentPower = constrain(m_powerTarget, m_currentPower - 3*m_accelerationLimit, m_currentPower + 3*m_accelerationLimit);
      }
      else {
        m_currentPower = constrain(m_powerTarget, m_currentPower - m_accelerationLimit, m_currentPower + m_accelerationLimit);
      }
      int16_t m_outputPower = m_currentPower;
      if (m_currentPower > 0 && abs(m_currentPower) < m_minPower) m_outputPower = m_minPower;
      if (m_currentPower < 0 && abs(m_currentPower) < m_minPower) m_outputPower = -m_minPower;
      if (m_backward) {
        motor.speed(m_motor_num, -m_outputPower);
      }
      else {
        motor.speed(m_motor_num, m_outputPower);
      }
      break;
    }
    case OFF:
      motor.stop(m_motor_num);
      break;
  }
}

int32_t EncoderMotor::getPosition() {
  return m_encoder.getPosition();
}

} // namespace hardware
