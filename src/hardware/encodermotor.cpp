#include "hardware/encodermotor.h"

namespace hardware {

EncoderMotor::EncoderMotor(DCMotor& motor, Encoder& encoder) :
  m_motor(motor), m_encoder(encoder), m_velocityPid(1,0,0), m_positionPid(1,0,0)
{

}

void EncoderMotor::setPower(int16_t power) {
  m_controlMode = POWER;
  m_powerTarget = power;
}

void EncoderMotor::setVelocity(int16_t velocity, int16_t power) {
  m_controlMode = VELOCITY;
  m_velocityTarget = velocity;
  m_maxPower = power;
}

bool EncoderMotor::setPosition(int32_t position, int16_t speed) {
  m_controlMode = POSITION;
  m_positionTarget = position;
  m_maxVelocity = speed;
  m_maxPower = 255;
  if (m_encoder.getVelocity() == 0 
    && m_encoder.getPosition() == m_positionTarget) {
    return true;
  }
  else return false;
}

void EncoderMotor::stop() {
  m_controlMode = IDLE;
  m_motor.stop();
}

void EncoderMotor::hold() {
  setPosition(m_encoder.getPosition(), 20);
}

#define median(a,b,c) max(min(a,b), min(max(a,b),c))

void EncoderMotor::tick() {
  switch (m_controlMode) {
    case POSITION:
      m_velocityTarget = m_positionPid.run(m_encoder.getPosition() - m_positionTarget);
      if (m_velocityTarget > m_maxVelocity) m_velocityTarget = m_maxVelocity;
      if (m_velocityTarget < -m_maxVelocity) m_velocityTarget = -m_maxVelocity;
      //fall through
    case VELOCITY:
      m_powerTarget += m_velocityPid.run(m_encoder.getVelocity() - m_velocityTarget);
      if (m_powerTarget > m_maxPower) m_powerTarget = m_maxPower;
      if (m_powerTarget < -m_maxPower) m_powerTarget = -m_maxPower;
      //fall through
    case POWER:
      m_motor.setPower(m_powerTarget);
      break;
    case IDLE:
      m_motor.stop();
      break;
  }
}

int32_t EncoderMotor::getPosition() {
  return m_encoder.getPosition();
}

}