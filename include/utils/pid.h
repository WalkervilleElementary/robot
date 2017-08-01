#pragma once
#include <Print.h>

namespace utils {

class PidController {
public:
  PidController(float kP, float kI, float kD, float kDecay = 0.9);
  inline ~PidController() {};
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

}