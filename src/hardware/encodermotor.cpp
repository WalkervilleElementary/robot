#include "hardware/encodermotor.h"
#include "phys253.h"

namespace hardware {

EncoderMotor::EncoderMotor(int motor_num, Encoder& encoder, bool backward) :
  m_motor_num(motor_num), m_encoder(encoder), m_backward(backward), 
  m_velocityPid(-2,0.5,-0.25), m_positionPid(-0.5,0.0,-0.5)
{}

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
    && abs(m_encoder.getPosition() - m_positionTarget) < deadZone) {
    return true;
  }
  else return false;
}

void EncoderMotor::stop() {
  m_controlMode = IDLE;
  motor.stop(m_motor_num);
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
      if (m_backward) motor.speed(m_motor_num, -m_powerTarget);
      else  motor.speed(m_motor_num, m_powerTarget);
      break;
    case IDLE:
      motor.stop(m_motor_num);
      break;
  }
}

int32_t EncoderMotor::getPosition() {
  return m_encoder.getPosition();
}

}
