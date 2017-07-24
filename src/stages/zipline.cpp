/*#include <phys253.h>

#include "stages/zipline.h"

namespace stages{

uint8_t Zipline::beacon_error_ = IR_ERROR_MARGIN();
int Zipline::forward_speed_ = SLOW_FORWARD_SPEED();
int Zipline::reverse_speed_ = BACKUP_SPEED();

uint32_t Zipline::distance_to_turn_ = ZIPLINE_TURN_DISTANCE();
uint32_t Zipline::distance_to_zipline_ = MAX_ZIPLINE_DISTANCE();
uint32_t Zipline::encoder_start_;
uint32_t Zipline::ticks_;

bool Zipline::left_surface_ = true;  // TODO read this from the switch
uint8_t Zipline::state_ = 0;

bool Zipline::loop() {
  switch (state_) {
    case 0:  // initialize settings
      ticks_ = hardware::Encoder::cmToTicks(distance_to_turn_);
      encoder_start_ = encoder_.get(hardware::R_ENCODER_);
      state_++;
    case 1:
    {
      follower_.loop();
      int32_t difference = abs(beacon_.leftIntensity() - beacon_.rightIntensity());
      if (difference < beacon_error_) {
        follower_.followIr();
        encoder_start_ = encoder_.get(hardware::R_ENCODER_);
        state_++;
      }
      break;
    }
    case 2:  // follow beacon for predetermined number of ticks
      follower_.loop();
      if (encoder_.get(hardware::R_ENCODER_) - encoder_start_ > ticks_) {
        // turn toward zipline, raise platform
        maneuver_.turn(left_surface_ ? -90 : 90);
        platform_.raise();
        encoder_start_ = encoder_.get(hardware::R_ENCODER_);
        ticks_ = hardware::Encoder::cmToTicks(MAX_ZIPLINE_DISTANCE());
        state_++;
      }
      break;
    case 3:  // dead reckoning stage, drive slowly until zipline is hit
      driver_.sendWheelVelocities(forward_speed_, forward_speed_);
      if (!digitalRead(PLATFORM_UPPER_SWITCH())) {
        // got the zipline!
        platform_.lower();
        return true;
      } else if (encoder_.get(hardware::R_ENCODER_) - encoder_start_ > ticks_) {
        state_ = 3;
      }
      break;
    case 4:  // oh noes, we didn't find the zipline
      LCD.clear();  LCD.home();  LCD.setCursor(0, 0);
      LCD.print("SOMETHING WENT WRONG");
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
  if (state > 3) {
    state_ = 3;
  } else {
    state_ = state;
  }
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
      case 2:
        beacon_error_ += change;
        LCD.setCursor(0,0); LCD.print("beacon error");
        LCD.setCursor(0,1); LCD.print(beacon_error_);
    }
  }
}
#endif  // USE_UPDATE()

}  // namespace stages*/
