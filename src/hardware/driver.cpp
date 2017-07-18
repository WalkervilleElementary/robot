#include "configs.h"

#include <phys253.h>
#include "hardware/driver.h"

namespace hardware
{

Driver::Driver()
{
  acceleration_limit = ACCEL_LIMIT();
  prev_right = 0;
  prev_left = 0;
}

void Driver::sendWheelVelocities(int right, int left)
{
  prev_right = constrain(right, prev_right - acceleration_limit, prev_right + acceleration_limit);
  prev_left = constrain(left, prev_left - acceleration_limit, prev_left + acceleration_limit);

  motor.speed(L_MOTOR_, -prev_left);
  motor.speed(R_MOTOR_, prev_right);
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

#if USE_UPDATE()
void Driver::update()
{
  delay(1000);
  while (startbutton());
  while (!startbutton())
  {
    int start_val = knob(6);
    delay(100);
    int end_val = knob(6);
    acceleration_limit += (end_val - start_val) / 8;
    LCD.clear(); LCD.home();
    LCD.setCursor(0,0); LCD.print("accel limit");
    LCD.setCursor(0,1); LCD.print(acceleration_limit);
  }
}
#endif  // USE_UDPATE()

}  // namespace hardware


