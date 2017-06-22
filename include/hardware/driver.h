#ifndef WALKERVILLE_ROBOT_HARDWARE_DRIVER_H
#define WALKERVILLE_ROBOT_HARDWARE_DRIVER_H

#include "configs.h"

namespace hardware
{

const int R_MOTOR_ = R_MOTOR();
const int L_MOTOR_ = L_MOTOR();

class Driver
{
public:
  inline Driver(){};
  inline ~Driver(){};

  void stop();
  void sendMotorCommand(int velocity, int command);

};  // class Driver

}  // namespace hardware

#endif  // WALKERVILLE_ROBOT_HARDWARE_DRIVER_H