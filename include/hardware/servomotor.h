#pragma once
#include <stdint.h>
#include <ServoTINAH.h>
#include <Print.h>

namespace hardware {

class ServoMotor {
public:
  ServoMotor(uint8_t pin, int16_t speed);
  bool setPosition(int16_t position);
  void stop();
  void tick();
  void printTo(Print& p);
private:
  uint8_t m_pin;
  int16_t m_speed;
  bool m_attached;
  int16_t m_target;
  int16_t m_currentPosition;
  ServoTINAH m_servo;
};

}