#include "sequences/platform.h"
#include <phys253.h>
#include <LiquidCrystal.h>

namespace sequences{

int8_t Platform::state_ = 0;
int Platform::raise_speed_ = PLATFORM_RAISE_SPEED();
int Platform::lower_speed_ = PLATFORM_LOWER_SPEED();
int Platform::backup_speed_ = BACKUP_SPEED();

const uint8_t Platform::upper_switch_ = PLATFORM_UPPER_SWITCH();
const uint8_t Platform::lower_switch_ = PLATFORM_LOWER_SWITCH();
const uint8_t Platform::motor_number_ = PLATFORM_MOTOR();

bool Platform::loop(){
  switch (state_){
    case 0:
      break;
    case 1:  // raising platform
      if (digitalRead(upper_switch_))
        motor.speed(motor_number_, raise_speed_);
      else
        state_++;
      return false;
    case 2:  // lower just enough so switch is no longer active
      if (!digitalRead(upper_switch_)){
        motor.speed(motor_number_, (int)(lower_speed_ * 0.6));
        return false;
      }else{
        break;
      }
    case 3:  // slowly lower while backing up
      if (digitalRead(lower_switch_)){
        motor.speed(motor_number_, lower_speed_);
        driver_.sendWheelVelocities(backup_speed_, backup_speed_);
        return false;
      }
  }
  state_ = 0;
  stop();
  return true;
}

void Platform::stop() {
  state_ = 0;
  motor.stop(motor_number_);
  driver_.stop();
}

void Platform::raise() {
  state_ = 1;
}

void Platform::lower() {
  state_ = 3;
}

#if USE_UPDATE()
void Platform::update(){
  int8_t update_state_ = 0;
  stop();
  delay(200);
  while (!startbutton()){
    if (stopbutton()) update_state_ += 1;
    if (update_state_ > 2) update_state_ = 0;
    int tune_val = knob(7);
    if (tune_val < TUNE_THRESHOLD){
      LCD.clear(); LCD.home();
      LCD.setCursor(0,0); LCD.print("Tuning Off");
      LCD.setCursor(0,1); LCD.print(tune_val);
      delay(100);
    }
    else{
      int start_val = knob(6);
      delay(100);
      int end_val = knob(6);

      int change = (start_val - end_val)/4 ;
      LCD.clear();  LCD.home() ;

      switch (update_state_){
      case 0:
          raise_speed_ += change;
          LCD.setCursor(0,0); LCD.print("raise speed");
          LCD.setCursor(0,1); LCD.print(raise_speed_);
          break;
      case 1:
          lower_speed_ += change;
          LCD.setCursor(0,0); LCD.print("lower speed");
          LCD.setCursor(0,1); LCD.print(lower_speed_ * 0.5);
          break;
      case 2:
          backup_speed_ += change;
          LCD.setCursor(0,0); LCD.print("backup speed");
          LCD.setCursor(0,1); LCD.print(backup_speed_);
          break;
      }
    }
  }
}
#endif  // USE_UPDATE()

}  // namespace sequences
