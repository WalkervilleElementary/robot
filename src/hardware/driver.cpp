#include "configs.h"

#include <phys253.h>
#include "hardware/driver.h"

namespace hardware{

int Driver::accel_limit_ = ACCEL_LIMIT();
int Driver::prev_right_ = 0;
int Driver::prev_left_ = 0;

void Driver::sendWheelVelocities(int right, int left){
  prev_right_ = constrain(right, prev_right_ - accel_limit_, prev_right_ + accel_limit_);
  prev_left_ = constrain(left, prev_left_ - accel_limit_, prev_left_ + accel_limit_);

  motor.speed(L_MOTOR_, -prev_left_);
  motor.speed(R_MOTOR_, prev_right_);
}

void Driver::sendMotorCommand(int velocity, int command){
  sendWheelVelocities(velocity + command, velocity - command);
}

void Driver::stop(){
  motor.stop(L_MOTOR_);
  motor.stop(R_MOTOR_);
  prev_right_ = 0;
  prev_left_ = 0;
}

#if USE_UPDATE()
void Driver::update(){
  delay(1000);
  while (startbutton());
  while (!startbutton())
  {
    int tune_val = knob(7);
    if (tune_val < TUNE_THRESHOLD()){
      LCD.clear(); LCD.home();
      LCD.setCursor(0,0); LCD.print("Tuning Off");
      LCD.setCursor(0,1); LCD.print(tune_val);
      delay(100);
     }
     else{
       int start_val = knob(6);
       delay(100);
       int end_val = knob(6);
       accel_limit_ += (end_val - start_val) / 8;
       LCD.clear(); LCD.home();
       LCD.setCursor(0,0); LCD.print("accel limit");
       LCD.setCursor(0,1); LCD.print(accel_limit_);
     }
  }
}
#endif  // USE_UDPATE()

}  // namespace hardware
