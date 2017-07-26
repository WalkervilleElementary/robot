#include <phys253.h>

#include "stages/zipline.h"

namespace stages{

int Zipline::forward_speed_ = SLOW_FORWARD_SPEED();

uint32_t Zipline::distance_to_turn_ = ZIPLINE_TURN_DISTANCE();
uint32_t Zipline::distance_to_zipline_ = MAX_ZIPLINE_DISTANCE();
uint32_t Zipline::encoder_start_;
uint32_t Zipline::ticks_;

uint32_t Zipline::ir_end_ = ZIPLINE_IR_END();

bool Zipline::left_surface_ = true;  // TODO read this from the switch
uint8_t Zipline::state_ = 0;

bool Zipline::loop() {
  switch (state_) {
    case 0:  // initialize settings
      // ticks_ = hardware::Encoder::cmToTicks(distance_to_turn_);
      // encoder_start_ = encoder_.get(hardware::R_ENCODER_);
      state_ = 1;
    case 1:
    {
      LCD.setCursor(0,0); LCD.print(beacon_.leftIntensity());
      LCD.setCursor(0,0); LCD.print(beacon_.rightIntensity());
      follower_.loop();
      if (beacon_.getTapeError() == 0) {
        follower_.followIr();
        encoder_start_ = encoder_.get(hardware::R_ENCODER_);
        ticks_ = hardware::Encoder::cmToTicks(distance_to_turn_);
        state_ = 2;
      }
      break;
    }
    case 2:  // follow beacon for predetermined number of ticks
      //LCD.setCursor(0,0); LCD.print("toward beacon");
      follower_.loop();
      if (encoder_.get(hardware::R_ENCODER_) - encoder_start_ > ticks_) {
        // turn toward zipline
        maneuver_.turn(left_surface_ ? -75 : 75); // TODO make this configurables
        state_ = 3;
      } else {
        break;
      }
    case 3:  // turn toward zipline
      //LCD.setCursor(0,0); LCD.print("turning");
      if (maneuver_.loop()) {
        // drive backwards so we are not underneath the zipline
        maneuver_.straight(-50);
        platform_.raise();
        state_ = 4;
      } else {
        break;
      }
    case 4:  // backup and raise platform
      //LCD.setCursor(0,0); LCD.print("back raise");
      if (maneuver_.loop() && platform_.loop()) {
        // raise platform
        encoder_start_ = encoder_.get(hardware::R_ENCODER_);
        ticks_ = hardware::Encoder::cmToTicks(distance_to_zipline_);
        state_ = 5;
      } else {
        break;
      }
    case 5:  // dead reckoning stage, drive slowly until zipline is hit
      //LCD.setCursor(0,0); LCD.print("Ramming the zipline");
      driver_.sendWheelVelocities(forward_speed_, forward_speed_);
      if (!digitalRead(PLATFORM_UPPER_SWITCH())) {
        // got the zipline!
        platform_.lower();
        state_ = 6;
      } else if (encoder_.get(hardware::R_ENCODER_) - encoder_start_ > ticks_) {
        state_ = 7;
      }
      break;
    case 6:
      if (platform_.loop()) return true;
      break;
    default:  // oh noes, we didn't find the zipline
      follower_.stop();
      LCD.home(); LCD.setCursor(0,0); LCD.print("SOMETHING WENT WRONG");
      delay(1000);
  }
  return false;
}

void Zipline::stop() {
  driver_.stop();
  follower_.stop();
  platform_.stop();
}

#if DEBUG()
void Zipline::set_state(uint8_t state) {
  state_ = state;
}
#endif  // DEBUG()

#if USE_UPDATE()
void Zipline::update() {
  int8_t update_state_ = 0;
  stop();
  delay(200);
  while (!startbutton()) {
    if (stopbutton()) update_state_++;
    if (update_state_ > 2) update_state_ = 0;
    int tune_val = knob(7);
    if (tune_val < TUNE_THRESHOLD()){
      LCD.clear(); LCD.home();
      LCD.setCursor(0,0); LCD.print("Tuning off");
      LCD.setCursor(0,1); LCD.print(tune_val);
      delay(100);
    }
    else{
      int start_val = knob(6);
      delay(100);
      int end_val = knob(6);

      int change = (start_val - end_val)/4 ;
      LCD.clear();  LCD.home() ;

      switch (update_state_) {
      case 0:
        distance_to_turn_ += change;
        LCD.setCursor(0,0); LCD.print("turn distance cm");
        LCD.setCursor(0,1); LCD.print(distance_to_turn_);
        break;
      case 1:
        distance_to_zipline_ += change;
        LCD.setCursor(0,0); LCD.print("zipline dist cm");
        LCD.setCursor(0,1); LCD.print(distance_to_zipline_);
        break;
      }
    }
  }
}
#endif  // USE_UPDATE()

}  // namespace stages
