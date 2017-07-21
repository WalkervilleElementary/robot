#ifndef WALKERVILLE_ROBOT_HARDWARE_DRIVER_H
#define WALKERVILLE_ROBOT_HARDWARE_DRIVER_H

#include "configs.h"

#include <stdint.h>

namespace hardware{

static const int R_MOTOR_ = R_MOTOR();  // This is int because that is how it is defined in tinah_libraries
static const int L_MOTOR_ = L_MOTOR();

class Driver{

private:
  static int prev_right_;
  static int prev_left_;
  static int accel_limit_;

public:
  inline Driver(){};
  inline ~Driver(){};

  static void stop();
  static void sendWheelVelocities(int right, int left);
  static void sendMotorCommand(int velocity, int command);

#if USE_UPDATE()
  void update();
#endif  // USE_UPDATE()

};  // class Driver

}  // namespace hardware

#endif  // WALKERVILLE_ROBOT_HARDWARE_DRIVER_H
