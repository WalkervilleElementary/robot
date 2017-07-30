#pragma once
#include <stdint.h>
#include "hardware/dcmotor.h"
#include "hardware/encoder.h"
#include "utils/pid.h"

namespace hardware {

class EncoderMotor {
public:
  EncoderMotor(DCMotor& motor, Encoder& encoder);
  void setPower(int16_t power);
  void setVelocity(int16_t velocity, int16_t power = 255);
  bool setDisplacement(int32_t position, int16_t speed);
  void stop();
  void hold();
  void tick();

private:
  DCMotor& m_motor;
  Encoder& m_encoder;

  enum ControlMode { IDLE, POWER, VELOCITY, DISPLACEMENT };
  ControlMode m_controlMode;
  int16_t m_powerTarget;
  int16_t m_velocityTarget;
  int32_t m_displacementTarget;
  int16_t m_maxPower;
  int16_t m_maxVelocity;
  PidController m_velocityPid;
  PidController m_positionPid;
  
};

}