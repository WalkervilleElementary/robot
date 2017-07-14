#include "configs.h"

#include <phys253.h>
#include "hardware/driver.h"

namespace hardware
{
    
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

}  // namespace hardware


