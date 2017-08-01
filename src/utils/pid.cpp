#include "utils/pid.h"

PidController::PidController(float kP, float kI, float kD, float kDecay) : 
  kP(kP), kI(kI), kD(kD), kDecay(kDecay)
{
  prevError = 0.0;
  integral = 0.0;
}

float PidController::run(float error) {
  float derivative = error - prevError;
  integral += error;
  float output = kP * error + kI * integral * (1. - kDecay) + kD * derivative;
  integral *= kDecay;
  prevError = error;
  prevOutput = output;
  return output;
}

void PidController::printTo(Print& p) {
  p.print(prevError);
  p.print(' ');
  p.print(integral);
  p.print(' ');
  //p.print(prevOutput);
}