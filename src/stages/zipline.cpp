#include <phys253.h>

#include "stages/zipline.h"

namespace stages{

float Zipline::pause_distance_ = ZIPLINE_PAUSE_DISTANCE();
float Zipline::backup_distance_ = ZIPLINE_BACKUP_DISTANCE();
int16_t Zipline::forward_speed_ = ZIPLINE_FORWARD_SPEED();
int16_t Zipline::backup_speed_ = ZIPLINE_BACKUP_SPEED();
int16_t Zipline::ram_speed_ = ZIPLINE_RAM_SPEED();

bool Zipline::left_surface_ = true;
uint8_t Zipline::state_ = 0;

int8_t Zipline::intersections_ = 2;
int8_t Zipline::count_ = 0;

bool Zipline::loop() {
  switch (state_) {
    case 0:  // initialization
      // LCD.setCursor(0,0); LCD.print("initialize");
      driver_.commandLineFollow(0);
      state_++;
    case 1:  // follow tape
      // LCD.setCursor(0,0); LCD.print("follow tape");
      platform_.loop();
      driver_.commandLineFollow(0);
      if (qrd_.isIntersection()) {
//        driver_.commandDriveStraight(5); // TODO make this configurable
        if (left_surface_) driver_.commandTurnRight(10);
        else driver_.commandTurnLeft(10);
        count_++;
        state_++;
      }
      break;
    case 2:
      platform_.loop();
      if (driver_.readyForCommand()) {
        if (count_ == intersections_) state_ = 3;
        else state_ = 1;
      }
      break;
    case 3:  // go straight
      platform_.loop();
      driver_.commandDriveStraight(pause_distance_, forward_speed_);
      state_++;
      break;
    case 4:  // check to make sure platform is ready before going further
      if (platform_.loop() && driver_.readyForCommand() ) {
        driver_.setVelocity(ram_speed_, ram_speed_);
        state_++;
      } else {
        break;
      }
    case 5:  // ram into zipline
      if (!digitalRead(PLATFORM_UPPER_SWITCH())) {
        driver_.stop();
        driver_.commandDriveStraight(backup_distance_, backup_speed_);
        platform_.lower();
        state_++;
      } else {
        break;
      }
    case 6:  // lower while backing up
      if (platform_.loop() && driver_.readyForCommand()) return true;
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
    SWITCH_CASES(0, check_distance_)
    SWITCH_CASES(1, ram_speed_)
    }
  }
}
#endif  // USE_UPDATE()

}  // namespace stages
