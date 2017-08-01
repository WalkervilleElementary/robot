#pragma once
#include <stdint.h>
#include "hardware/encoder.h"
#include "utils/pid.h"

namespace hardware {

class EncoderMotor {
public:
  EncoderMotor(int motor_num, Encoder& encoder, bool backward=false);
  void setPower(int16_t power);
  void setVelocity(int16_t velocity, int16_t power = 255);
  bool setPosition(int32_t position, int16_t speed);
  void stop();
  void hold();
  void tick();
  int32_t getPosition();

private:
  bool m_backward;
  int m_motor_num;
  Encoder& m_encoder;

  const static int32_t deadZone = 8;

  enum ControlMode { IDLE, POWER, VELOCITY, POSITION };
  ControlMode m_controlMode;
  int16_t m_powerTarget;
  int16_t m_velocityTarget;
  int32_t m_positionTarget;
  int16_t m_maxPower;
  int16_t m_maxVelocity;
  PidController m_velocityPid;
  PidController m_positionPid;
  
};

}
