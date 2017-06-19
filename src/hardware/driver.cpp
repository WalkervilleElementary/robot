#include "hardware/driver.h"

#if DEVICE()
#include <phys253.h> 
#else // !DEVICE()
#include <iostream>
#endif  // macro DEVICE()

namespace hardware
{
#if DEVICE()
void Driver::sendMotorCommand(int velocity, int command)
{
  motor.speed(L_MOTOR_, velocity - command);
  // TODO the negative is because the motor is wired backwards
  motor.speed(R_MOTOR_, -(velocity + command));
}
#else  // macro !DEVICE()
void Driver::sendMotorCommand(int velocity, int command)
{
  std::cout << "l_motor: " <<  velocity - command << std::endl;
  std::cout << "r_motor: " <<  velocity + command << std::endl;  
}
#endif  // marco DEVICE()
}  // namespace hardware


