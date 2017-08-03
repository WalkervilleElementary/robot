#pragma once
#include "configs.h"

#if DEBUG()
#include <Print.h>
#endif  // DEBUG

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
#if DEBUG()
  void printTo(Print& p);
#endif  // DEBUG

private:
  float prevError;
  float integral;
  float prevOutput;
};  // Class Pid

}  // namespace utils
