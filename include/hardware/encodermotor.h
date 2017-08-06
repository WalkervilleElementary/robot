#pragma once
#include <stdint.h>
#include "hardware/encoder.h"
#include "utils/pid.h"

namespace hardware {

class EncoderMotor {
public:
  EncoderMotor(int motor_num, Encoder& encoder, bool backward=false);
  void setPower(int16_t power, bool useRamp = true);
  void setVelocity(int16_t velocity, int16_t power = 255);
  bool setPosition(int32_t position, int16_t maxPower = 255, int16_t minPower = 40);
  void stop();
  void tick();
  int32_t getPosition();

private:
  int m_motor_num;
  Encoder& m_encoder;
  bool m_backward;

  utils::PidController m_velocityPid;
  utils::PidController m_positionPid;

  const int32_t m_deadZone;
  const int16_t m_accelerationLimit;

  enum ControlMode { OFF, POWER, VELOCITY, POSITION };
  ControlMode m_controlMode;
  int16_t m_currentPower;
  int16_t m_powerTarget;
  int16_t m_velocityTarget;
  int32_t m_positionTarget;
  int16_t m_maxPower;
  int16_t m_minPower;

};  // Class Encodermotor

}  // namespace hardware
