#pragma once

class PidController {
public:
  PidController(float kP, float kI, float kD, float kDecay = 0.95, float maxIntegral = 10.);
  float kP;
  float kI;
  float kD;
  float kDecay;
  float maxIntegral;
  float run(float error);
private:
  float prevError;
  float integral;
};