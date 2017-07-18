#include "configs.h"

#include <phys253.h>
#include "hardware/driver.h"

namespace hardware
{

void Driver::sendWheelVelocities(int right, int left)
{
  motor.speed(L_MOTOR_, -left);
  motor.speed(R_MOTOR_, right);
}

void Driver::sendMotorCommand(int velocity, int command)
{
  sendWheelVelocities(velocity + command, velocity - command);
}

void Driver::stop()
{
  motor.stop(L_MOTOR_);
  motor.stop(R_MOTOR_);
}

}  // namespace hardware


