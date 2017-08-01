#pragma once
#include <Print.h>

class PidController {
public:
  PidController(float kP, float kI, float kD, float kDecay = 0.9);
  float kP;
  float kI;
  float kD;
  float kDecay;
  float run(float error);
  void printTo(Print& p);
private:
  float prevError;
  float integral;
  float prevOutput;
};