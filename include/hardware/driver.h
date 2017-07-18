#ifndef WALKERVILLE_ROBOT_HARDWARE_DRIVER_H
#define WALKERVILLE_ROBOT_HARDWARE_DRIVER_H

#include "configs.h"

namespace hardware
{

const int R_MOTOR_ = R_MOTOR();
const int L_MOTOR_ = L_MOTOR();

class Driver
{
private:
  int prev_right;
  int prev_left;
  int acceleration_limit;
public:
  Driver();
  inline ~Driver(){};

  void stop();
  void sendWheelVelocities(int right, int left);
  void sendMotorCommand(int velocity, int command);

#if USE_UPDATE()
  void update();
#endif  // USE_UPDATE()

};  // class Driver

}  // namespace hardware

#endif  // WALKERVILLE_ROBOT_HARDWARE_DRIVER_H
