#include <phys253.h>

#include "stages/zipline.h"

namespace stages {

int32_t Zipline::checkpoint_ = hardware::Encoder::cmToTicks(ZIPLINE_CHECKPOINT_DISTANCE());
int32_t Zipline::backup_distance_ = hardware::Encoder::cmToTicks(ZIPLINE_BACKUP_DISTANCE());
int16_t Zipline::forward_power_ = ZIPLINE_FORWARD_POWER();
int16_t Zipline::backup_power_ = ZIPLINE_BACKUP_POWER();

bool Zipline::left_surface_ = true;
uint8_t Zipline::state_ = 0;

int8_t Zipline::intersections_ = 2;
int8_t Zipline::count_ = 0;
int32_t Zipline::encoder_start_ = 0;

bool Zipline::loop() {
  switch (state_) {
    case 0:  // initialization
      driver_.commandLineFollow(0);
      state_++;
    case 1:  // follow tape
      driver_.commandLineFollow(0);
      if (qrd_.isIntersection()) {
        if (left_surface_) driver_.commandTurnRight(10);
        else driver_.commandTurnLeft(10);
        count_++;
        state_++;
      }
      break;
    case 2:
      if (driver_.readyForCommand()) {
        if (count_ == intersections_) {
          encoder_start_ = encoder_.getPosition();
          driver_.setPower(forward_power_, forward_power_);
          platform_.raise();
          state_ = 3;
        } else {
          state_ = 1;
        }
      }
      break;
    case 3:  // go straight until checkpoint
      if (platform_.loop()) {
        driver_.setPower(forward_power_, forward_power_);
        state_++;
      } else if (encoder_.getPosition() - encoder_start_ > checkpoint_) {
        // checkpoint reached but platform not ready, need to pause
        driver_.stop();
      } 
      break;
    case 4:  // ram into zipline
      if (!digitalRead(PLATFORM_UPPER_SWITCH())) {
        encoder_start_ = encoder_.getPosition();
        driver_.stop();
        driver_.setPower(backup_power_, backup_power_);
        platform_.lower();
        state_++;
      } else {
        break;
      }
    case 5:  // lower while backing up
      if (platform_.loop() && (abs(encoder_.getPosition() - encoder_start_) > backup_distance_)) {
        driver_.stop();
        return true;
      } else {
        break;
      }
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

void Zipline::side(bool left_surface) {
  left_surface_ = left_surface;
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
    SWITCH_CASES(0, checkpoint_)
    }
  }
}
#endif  // USE_UPDATE()

}  // namespace stages
