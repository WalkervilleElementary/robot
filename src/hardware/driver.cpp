#include "configs.h"


#if DEVICE()
#include "driver.h"
#include <phys253.h>
#else // !DEVICE()
#include "hardware/driver.h"
#include <iostream>
#endif  // macro DEVICE()

namespace hardware
{
#if DEVICE()
void Driver::sendMotorCommand(int velocity, int command)
{
  motor.speed(L_MOTOR_, velocity - command);
  motor.speed(R_MOTOR_, velocity + command);
}

void Driver::stop()
{
  motor.stop(L_MOTOR_);
  motor.stop(R_MOTOR_);
}
#else  // end DEVICE(); start !DEVICE()
void Driver::sendMotorCommand(int velocity, int command)
{
  std::cout << "l_motor: " <<  velocity - command << std::endl;
  std::cout << "r_motor: " <<  velocity + command << std::endl;
}

void Driver::stop()
{
  std::cout << "motor is stopped" << std::endl;
}
#endif  // end !DEVICE()
}  // namespace hardware


