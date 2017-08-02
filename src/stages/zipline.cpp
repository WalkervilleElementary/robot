#include <phys253.h>

#include "stages/zipline.h"

namespace stages{

int Zipline::forward_speed_ = SLOW_FORWARD_SPEED();

int32_t Zipline::turn_distance_ = ZIPLINE_TURN_DISTANCE();
int32_t Zipline::turn_degrees_ = ZIPLINE_TURN_DEGREES();
int32_t Zipline::tape_distance_ = ZIPLINE_TAPE_DISTANCE();
int32_t Zipline::backup_distance_ = ZIPLINE_BACKUP_DISTANCE();
int32_t Zipline::encoder_start_;
int32_t Zipline::ticks_;

bool Zipline::left_surface_ = true;  // TODO read this from the switch
uint8_t Zipline::state_ = 0;

bool Zipline::loop() {
  switch (state_) {
    case 0:  // initialization
      // LCD.setCursor(0,0); LCD.print("initialize");
      driver_.commandLineFollow(0);
      qrd_.isIntersection();  // clear intersection state
      state_++;
      break;
    case 1:  // follow tape until first intersection
    case 3:
    case 5:
      // ticks_ = hardware::Encoder::cmToTicks(turn_distance_);
      // encoder_start_ = encoder_.get(hardware::R_ENCODER_);
      // LCD.setCursor(0,0); LCD.print("follow tape");
      driver_.commandLineFollow(0);
      if (qrd_.isIntersection()) {
        encoder_start_ = encoder_.getPosition();
        ticks_ = hardware::Encoder::cmToTicks(tape_distance_);
        driver_.commandDriveStraight(5); // TODO make this configurable
        state_++;
      }
      break;
    case 2:
    case 4:
    case 6:
      if (driver_.readyForCommand()) state_++;
      else break;
    case 7:  // follow tape a little further
      // LCD.setCursor(0,0); LCD.print("follow tape");
      driver_.commandLineFollow(0);
      if (encoder_.getPosition() - encoder_start_ > ticks_) {
        encoder_start_ = encoder_.getPosition();
        ticks_ = hardware::Encoder::cmToTicks(turn_distance_);
        state_++;
      }
      break;
    case 8:  // follow beacon for predetermined number of ticks
      //LCD.setCursor(0,0); LCD.print("toward beacon");
      // LCD.setCursor(0,0); LCD.print("follow beacon");
      driver_.commandBeaconFollow();
      if (encoder_.getPosition() - encoder_start_ > ticks_) {
        // turn toward zipline
        if (left_surface_) driver_.commandTurnRight(turn_degrees_);
        else driver_.commandTurnLeft(turn_degrees_);
        state_++;
      } else {
        break;
      }
    case 9:  // turn toward zipline
      //LCD.setCursor(0,0); LCD.print("turning");
      if (driver_.readyForCommand()) {
        // drive backwards so we are not underneath the zipline
        driver_.commandDriveStraight(-1 * backup_distance_);
        platform_.raise();
        state_++;
      } else {
        break;
      }
    case 10:  // backup and raise platform
      //LCD.setCursor(0,0); LCD.print("back raise");
      if (driver_.readyForCommand() && platform_.loop()) state_++;
      else break;
    case 11:  // dead reckoning stage, drive slowly until zipline is hit
      //LCD.setCursor(0,0); LCD.print("Ramming the zipline");
      driver_.setPower(forward_speed_, forward_speed_); // TODO use send velocity
      if (!digitalRead(PLATFORM_UPPER_SWITCH())) {
        platform_.lower();
        state_++;
      }
      break;
    case 12:  // lower while backing up
      if (platform_.loop()) return true;
      break;
    default:  // oh noes, we didn't find the zipline
      driver_.stop();
      LCD.home(); LCD.setCursor(0,0); LCD.print("SOMETHING WENT WRONG");
      delay(1000);
  }
  return false;
}

void Zipline::stop() {
  driver_.stop();
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
    SWITCH_CASES(0, turn_distance_)
    // SWITCH_CASES(1, distance_to_zipline_)
    SWITCH_CASES(1, turn_degrees_)
    SWITCH_CASES(2, tape_distance_)
    SWITCH_CASES(3, backup_distance_)

    /*case 0:
        turn_distance_ += change;
        LCD.setCursor(0,0); LCD.print("turn distance cm");
        LCD.setCursor(0,1); LCD.print(turn_distance_);
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
