#include "hardware/servomotor.h"

namespace hardware {

ServoMotor::ServoMotor(uint8_t pin, int16_t speed) {
  m_pin = pin;
  m_speed = speed;
  m_attached = false;
}

bool ServoMotor::setPosition(int16_t position) {
  if (!m_attached) {
    m_servo.attach(m_pin);
  }
  m_servo.write(position);
  m_target = position;
  if (m_currentPosition == m_target) return true;
  else return false;
}

void ServoMotor::stop() {
  m_servo.detach();
  m_attached = false;
}

void ServoMotor::tick() {
  if (m_currentPosition < m_target - m_speed) {
    m_currentPosition += m_speed;
  }
  else if (m_currentPosition > m_target + m_speed) {
    m_currentPosition -= m_speed;
  }
  else {
    m_currentPosition = m_target;
  }
}

}