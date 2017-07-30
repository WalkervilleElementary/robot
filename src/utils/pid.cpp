#include "utils/pid.h"

PidController::PidController(float kP, float kI, float kD, float kDecay, float maxIntegral) : 
  kP(kP), kI(kI), kD(kD), kDecay(kDecay), maxIntegral(maxIntegral)
{
  prevError = 0.0;
  integral = 0.0;
}

float PidController::run(float error) {
  float derivative = error - prevError;
  integral += error;
  if (integral > maxIntegral) integral = maxIntegral;
  if (integral < -maxIntegral) integral = -maxIntegral;
  float output = kP * error + kI * integral + kD * derivative;
  integral *= kDecay;
  prevError = error;
  return output;
}