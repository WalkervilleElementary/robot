#include "sequences/platform.h"
#include <phys253.h>
#include <LiquidCrystal.h>

namespace sequences{

int Platform::raise_speed_ = PLATFORM_RAISE_SPEED();
int Platform::lower_speed_ = PLATFORM_LOWER_SPEED();
int Platform::backup_speed_ = BACKUP_SPEED();

const uint8_t Platform::upper_switch_ = PLATFORM_UPPER_SWITCH();
const uint8_t Platform::lower_switch_ = PLATFORM_LOWER_SWITCH();
const uint8_t Platform::motor_number_ = PLATFORM_MOTOR();

bool Platform::loop(){
  // do nothing
  return true;
}

void Platform::stop() {
  // stop drive train motors & reset platform position
  driver_.stop();
  while (digitalRead(lower_switch_)) {
    motor.speed(motor_number_, (int)(lower_speed_ * 1.2));
    delay(LOOP_DELAY());
  }
}

bool Platform::raise() {
  // raise until switch is hit
  while (digitalRead(upper_switch_)) {
    motor.speed(motor_number_, raise_speed_);
    delay(LOOP_DELAY());
  }

  // slowly lower, just enough so switch is no longer active
  while (!digitalRead(upper_switch_)) {
    motor.speed(motor_number_, (int)(lower_speed_ * 0.8));
    delay((int)(LOOP_DELAY() * 0.5));
  }
  return true;
}

bool Platform::lower() {
  // slowly lower while backing up
  while (digitalRead(lower_switch_)) {
    motor.speed(motor_number_, lower_speed_);
    driver_.sendWheelVelocities(backup_speed_, backup_speed_);
    delay(LOOP_DELAY());
  }
  return true;
}

#if USE_UPDATE()
void Platform::update(){
  int8_t update_state_ = 0;
  stop();
  delay(200);
  while (!startbutton()){
    if (stopbutton()) update_state_ += 1;
    if (update_state_ > 2) update_state_ = 0;
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
#endif  // USE_UPDATE()

}  // namespace sequences
