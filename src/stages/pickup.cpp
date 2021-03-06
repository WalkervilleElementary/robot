#include "stages/pickup.h"

#include <phys253.h>
#include <LiquidCrystal.h>

namespace stages {

int8_t Pickup::state_ = -4;
int8_t Pickup::agents_ = 0;

int32_t Pickup::start_encoder_;
int32_t Pickup::current_encoder_;
int32_t Pickup::alignment_distance_;

int32_t Pickup::to_ramp_ = hardware::Encoder::cmToTicks(PICKUP_TO_RAMP());
int32_t Pickup::to_intersection_ = hardware::Encoder::cmToTicks(PICKUP_TO_INTERSECTION());
int32_t Pickup::to_tick_ = 0;
int32_t Pickup::turn_degree_ = PICKUP_TURN_DEGREE();

bool Pickup::side_ = false;

const int8_t Pickup::height[] = PICKUP_HEIGHT();

bool Pickup::loop() {
  // States Description
  // -4     start; initialize values
  // -3     tape follow until we are on the ramp
  // -2     raise claw and tape follow with increased speed
  // -1     go up ramp; decrease speed at the top
  // 0      tape follow until tub
  // 1      turn towards the direction we want
  // 2      backup to increase time to correctly tape follow
  // 3      tape follow until agent
  // 4      align with the claw
  // 5      wait for claw; intialize grab
  // 6      wait for claw; initialize release turn back on tape
  // 7      wait for claw; wait for realign with tape;
  //             wait for release to finish or return to state 2
  // 8      wait for release to finish
  // 9      wait for raise to finish
  LCD.setCursor(0,0); LCD.print(state_);
  switch (state_) {
    case -4:  // start
      start_encoder_ = encoder_.getPosition();
      state_ = -3;
    case -3:  // first turn
    {
      driver_.commandLineFollow(1);
      current_encoder_ = encoder_.getPosition();

      if (current_encoder_ > start_encoder_ + to_ramp_) state_ = -2;
      else break;
    }
    case -2:  // getting ready to go up the ramp
      if (side_) claw_.raise(sequences::LEFT_CLAW);
      else claw_.raise(sequences::RIGHT_CLAW);
      state_ = -1;
    case -1:  // going up the ramp
    {
      driver_.commandLineFollow(2);
      claw_.loop();
      current_encoder_ = encoder_.getPosition();
      if (current_encoder_ > start_encoder_ + to_intersection_) {
        state_ = 0;

      } else {
        break;
      }
    }
    case 0:  // going into the tub
    case 4:  // Travelling to next agent
      claw_.loop();
      if (qrd_.isIntersection()) {
        driver_.stop();
        state_++;
      } else {
        driver_.commandLineFollow(0);
      }
      break;
    case 1:  // Turning at the tub
      if (side_) claw_.release(sequences::RIGHT_CLAW);
      else claw_.release(sequences::LEFT_CLAW);
      if (side_) driver_.commandTurnRight(-turn_degree_);
      else driver_.commandTurnLeft(-turn_degree_);
      state_ = 2;
    case 2:
      claw_.loop();
      if (driver_.readyForCommand()) {
        driver_.commandDriveStraight(PICKUP_TURN_FORWARD_DISTANCE());
        state_ = 100;
      } else {
        break;
      }
    case 100:
      claw_.loop();
      if (driver_.readyForCommand()) {
        driver_.commandLineFollow(0);
        to_tick_ = encoder_.getPosition()
                 + hardware::Encoder::cmToTicks(PICKUP_DISTANCE_FIRST_AGENT());
        state_ = 3;
      } else {
        break;
       }
    case 3:
      claw_.loop();
      if (encoder_.getPosition() > to_tick_) {
        state_ = 4;
        driver_.commandLineFollow(0);
      }
      break;
    case 5:  // Aligning the Claw
      alignment_distance_ = PICKUP_BACKWARD_DISTANCE();
      driver_.commandDriveStraight(alignment_distance_);
      state_ = 6;
    case 6:  // Waiting for claw to be ready
      if (claw_.loop() && driver_.readyForCommand()) {
        state_ = 7;
        if (side_) claw_.grab(sequences::RIGHT_CLAW, height[agents_]);
        else claw_.grab(sequences::LEFT_CLAW, height[agents_]);
      } else {
        break;
      }
    case 7:  // Waiting for claw to grab
      if (claw_.loop()){
        if (side_) claw_.release(sequences::RIGHT_CLAW);
        else claw_.release(sequences::LEFT_CLAW);
        driver_.commandDriveStraight(max(0, PICKUP_FORWARD_DISTANCE() - alignment_distance_));
        agents_++;
        state_ = 8; // TODO 101
        break;
      }else{
        break;
      }
    case 101:
      claw_.loop();
      if (driver_.readyForCommand()) {
        if (side_) driver_.commandTurnRight(13);
        else driver_.commandTurnLeft(13);
        state_ = 8;
      } else {
        break;
      }
    case 8:  // Return back to tape following
      claw_.loop();
      if (driver_.readyForCommand()){
        driver_.commandLineFollow(0);
        if (agents_ == 5) {
          state_ = 9;
        } else if (agents_ == 6) {
          state_ = 11;
        } else {
          state_ = 4;
        }
      }else{
        break;
      }
    case 9:
      claw_.loop();
      if (qrd_.isIntersection()) {
        driver_.commandDriveStraight(5);
        state_ = 10;
      } else {
        break;
      }
    case 10:
      claw_.loop();
      if (driver_.readyForCommand()) {
        driver_.commandLineFollow(0);
        state_ = 4;
      }
      break;
    case 11:  // wait for claw to finish before next stage
      return true;
    /*case 12:
      if (claw_.loop()) return true;
      else break;*/
  }

  return false;
}

void Pickup::side(bool left_surface) {
  side_ = left_surface;
}

#if USE_UPDATE()
bool Pickup::update(){
  int8_t update_state_ = 0;
  int change = 0;
  driver_.stop();
  delay(1000);
  while (startbutton());
  while (!startbutton()){
    if (stopbutton()) update_state_ ++;
    if (update_state_ > 3) update_state_ = 0;

    LCD.clear(); LCD.home();
    int tune_val = knob(7);
    int start_val = knob(6);
    delay(100);
    int end_val = knob(6);
    if (tune_val > TUNE_THRESHOLD()){
      change = (start_val - end_val)/50;
    }
    LCD.clear(); LCD.home();

    switch (update_state_){
      SWITCH_CASES(0, turn_degree_)
        case 1:
          to_ramp_ += change / 50;
          LCD.setCursor(0,0); LCD.print("to_ramp_");
          LCD.setCursor(0,1); LCD.print(to_ramp_);
        case 2:
          to_intersection_ += change / 50;
          LCD.setCursor(0,0); LCD.print("to intersection");
          LCD.setCursor(0,1); LCD.print(to_intersection_);
    }
  }
}
#endif  // USE_UPDATE()

}  // namespace stages
