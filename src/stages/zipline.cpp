#include <phys253.h>

#include "stages/zipline.h"

namespace stages{

int Zipline::forward_speed_ = SLOW_FORWARD_SPEED();

uint32_t Zipline::distance_to_turn_ = ZIPLINE_TURN_DISTANCE();
uint32_t Zipline::distance_to_zipline_ = MAX_ZIPLINE_DISTANCE();
uint32_t Zipline::encoder_start_;
uint32_t Zipline::ticks_;

bool Zipline::left_surface_ = true;  // TODO read this from the switch
uint8_t Zipline::state_ = 0;

bool Zipline::loop() {
  switch (state_) {
    case 0:  // initialization
      LCD.setCursor(0,0); LCD.print("initialize");
      follower_.loop();
      qrd_.isIntersection();  // clear intersection state
      state_++;
      break;
    case 1:  // follow tape until first intersection
      // ticks_ = hardware::Encoder::cmToTicks(distance_to_turn_);
      // encoder_start_ = encoder_.get(hardware::R_ENCODER_);
      LCD.setCursor(0,0); LCD.print("follow tape");
      follower_.loop();
      if (qrd_.isIntersection()) {
        encoder_start_ = encoder_.get(hardware::R_ENCODER_);
        ticks_ = hardware::Encoder::cmToTicks(25);
        state_++;
      }
      break;
    case 2:
      LCD.setCursor(0,0); LCD.print("follow tape");
      follower_.loop();
      if (encoder_.get(hardware::R_ENCODER_) - encoder_start_ > ticks_) {
        encoder_start_ = encoder_.get(hardware::R_ENCODER_);
        follower_.followIr();
        ticks_ = hardware::Encoder::cmToTicks(distance_to_turn_);
        state_++;
      }
      break;
    case 3:  // follow beacon for predetermined number of ticks
      //LCD.setCursor(0,0); LCD.print("toward beacon");
      LCD.setCursor(0,0); LCD.print("follow beacon");
      follower_.loop();
      if (encoder_.get(hardware::R_ENCODER_) - encoder_start_ > ticks_) {
        // turn toward zipline
        maneuver_.turn(left_surface_ ? -75 : 75); // TODO make this configurables
        state_++;
      } else {
        break;
      }
    case 4:  // turn toward zipline
      //LCD.setCursor(0,0); LCD.print("turning");
      if (maneuver_.loop()) {
        // drive backwards so we are not underneath the zipline
        maneuver_.straight(-50);
        platform_.raise();
        state_++;
      } else {
        break;
      }
    case 5:  // backup and raise platform
      //LCD.setCursor(0,0); LCD.print("back raise");
      if (maneuver_.loop() && platform_.loop()) {
        // raise platform
        encoder_start_ = encoder_.get(hardware::R_ENCODER_);
        ticks_ = hardware::Encoder::cmToTicks(distance_to_zipline_);
        state_++;
      } else {
        break;
      }
    case 6:  // dead reckoning stage, drive slowly until zipline is hit
      //LCD.setCursor(0,0); LCD.print("Ramming the zipline");
      driver_.sendWheelVelocities(forward_speed_, forward_speed_);
      if (!digitalRead(PLATFORM_UPPER_SWITCH())) {
        // got the zipline!
        platform_.lower();
        state_++;
      } else if (encoder_.get(hardware::R_ENCODER_) - encoder_start_ > ticks_) {
        state_++;
      }
      break;
    case 7:
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
  int change = 0;
  stop();
  delay(200);
  while (!startbutton()) {
    if (stopbutton()) update_state_++;
    if (update_state_ > 1) update_state_ = 0;
    int tune_val = knob(7);
    int start_val = knob(6);
    delay(100);
    int end_val = knob(6);

    if (tune_val > TUNE_THRESHOLD()){
       change = (start_val - end_val)/4 ;
    }
    LCD.clear();  LCD.home() ;

    switch (update_state_) {
    SWITCH_CASES(0, distance_to_turn_)
    SWITCH_CASES(1, distance_to_zipline_)

    /*case 0:
        distance_to_turn_ += change;
        LCD.setCursor(0,0); LCD.print("turn distance cm");
        LCD.setCursor(0,1); LCD.print(distance_to_turn_);
        break;
    case 1:
        distance_to_zipline_ += change;
        LCD.setCursor(0,0); LCD.print("zipline dist cm");
        LCD.setCursor(0,1); LCD.print(distance_to_zipline_);
        break;
    */
    }
  }
}
#endif  // USE_UPDATE()

}  // namespace stages
