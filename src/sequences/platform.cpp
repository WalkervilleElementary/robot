#include "sequences/platform.h"
#include <phys253.h>
#if DEBUG()
#include <LiquidCrystal.h>
#endif  // DEBUG()

namespace sequences {

int8_t Platform::state_ = 0;
bool Platform::raised_ = false;

int Platform::raise_speed_ = PLATFORM_RAISE_SPEED();
int Platform::lower_speed_top_ = PLATFORM_LOWER_SPEED_TOP();
int Platform::lower_speed_bottom_ = PLATFORM_LOWER_SPEED_BOTTOM();
int Platform::maintain_power_ = PLATFORM_MAINTAIN_POWER();

// these variables are used in state 2
unsigned long Platform::lower_time_;
int Platform::lower_speed_modifier_;

const uint8_t Platform::upper_switch_ = PLATFORM_UPPER_SWITCH();
const uint8_t Platform::lower_switch_ = PLATFORM_LOWER_SWITCH();
const uint8_t Platform::motor_number_ = PLATFORM_MOTOR();

bool Platform::loop() {
  switch (state_) {
    case 0:
      break;
    case 1:  // raising platform
#if DEBUG()
      LCD.setCursor(0, 0); LCD.print("Raise ");
#endif  // DEBUG()
      if (digitalRead(upper_switch_)) {
        motor.speed(motor_number_, raise_speed_);
      } else {
        lower_speed_modifier_ = 0;  // start at 0
        lower_time_ = millis();  // record the time state 2 began
        state_++;
      }
      return false;
    case 2:  // lower just enough so switch is no longer active
#if DEBUG()
      LCD.setCursor(0, 0); LCD.print("Lower");
#endif  // DEBUG()
      if (!digitalRead(upper_switch_)){
        // every 1 seconds, increment the lowering speed by 1
        if (millis() - lower_time_ > 1000) {
          lower_speed_modifier_+=2;
          lower_time_ = millis();
        }
        motor.speed(motor_number_, lower_speed_top_ + lower_speed_modifier_);
        return false;
      }else{
        motor.speed(motor_number_, maintain_power_);
        raised_ = true;
        break;
      }
    case 3:  // lower fully
      if (digitalRead(lower_switch_)){
        motor.speed(motor_number_, lower_speed_bottom_);
        return false;
      } else {
        raised_ = false;
        stop();
      }
  }
  state_ = 0;
  return true;
}

void Platform::stop() {
  state_ = 0;
  motor.stop(motor_number_);
}

void Platform::raise() {
  if (!raised_) state_ = 1;
}

void Platform::lower() {
  state_ = 3;
}

#if USE_UPDATE()
void Platform::update() {
  int8_t update_state_ = 0;
  int change = 0;
  stop();
  delay(200);
  while (!startbutton()) {
    if (stopbutton()) update_state_ += 1;
    if (update_state_ > 5) update_state_ = 0;
    int tune_val = knob(7);
    int start_val = knob(6);
    delay(100);
    int end_val = knob(6);
    if (tune_val > TUNE_THRESHOLD()) {
      change = (start_val - end_val)/4 ;
    }
    LCD.clear();  LCD.home() ;

    switch (update_state_) {
    SWITCH_CASES(0, raise_speed_)
    SWITCH_CASES(1, lower_speed_top_)
    SWITCH_CASES(2,backup_speed_)
    SWITCH_CASES(3, maintain_power_)
    SWITCH_CASES(4, backup_time_)
    SWITCH_CASES(5, lower_speed_bottom_)
    }
  }
}
#endif  // USE_UPDATE()

}  // namespace sequences
