#include "stages/pickup.h"

#include <phys253.h>
#include <LiquidCrystal.h>

namespace stages{

int8_t Pickup::state_ = -4;
int8_t Pickup::agents_ = 0;

uint32_t Pickup::start_encoder_;
uint32_t Pickup::current_encoder_;

uint32_t Pickup::to_ramp_ = hardware::Encoder::cmToTicks(200);
uint32_t Pickup::to_intersection_ = hardware::Encoder::cmToTicks(550);

uint32_t Pickup::drive_distance_ = 20;  // TODO make this configurable
uint32_t Pickup::turn_degree_ = 24;  // TODO make this configurable
bool Pickup::side_ = false;  // TODO

const int8_t Pickup::height[] = {2,0,1,2,0,1,1}; // TODO actual values

bool Pickup::loop(){
  // States Description
  // -4     start; initialize values
  // -3     tape follow until we are on the ramp
  // -2     raise claw and tape follow with increased speed
  // -1     go up ramp; decrease speed at the top
  // 0      tape follow until tub
  // 1      turn towards the direction we want
  // 2      tape follow until agent
  // 3      align with the claw
  // 4      wait for claw; intialize grab
  // 5      wait for claw; initialize release turn back on tape
  // 6      wait for claw; wait for realign with tape;
  //             wait for release to finish or return to state 2
  // 7      tape follow until intersection
  // 8

  switch (state_){
    case -4:  // start
      // side_ = digitalRead(0); TODO
      if (side_) start_encoder_ = encoder_.get(hardware::L_ENCODER_);
      else start_encoder_ = encoder_.get(hardware::R_ENCODER_);
      state_ = -3;
    case -3:  // first turn
    {
      LCD.setCursor(0,1); LCD.print("first turn");
      follower_.loop();
      if (side_) current_encoder_ = encoder_.get(hardware::L_ENCODER_);
      else current_encoder_ = encoder_.get(hardware::R_ENCODER_);

      if (current_encoder_ > start_encoder_ + to_ramp_)
        state_ = -2;
      else
        break;
    }
    case -2:  // getting ready to go up the ramp
      follower_.update_vel(110); // TODO update value
      if (side_) claw_.raise(sequences::LEFT_CLAW);
      else claw_.raise(sequences::RIGHT_CLAW);
      state_ = -1;
    case -1:  // going up the ramp
    {
      LCD.setCursor(0,1); LCD.print("the ramp");
      follower_.loop();
      claw_.loop();
      if (side_) current_encoder_ = encoder_.get(hardware::L_ENCODER_);
      else current_encoder_ = encoder_.get(hardware::R_ENCODER_);

      if (current_encoder_ > start_encoder_ + to_intersection_){
        follower_.update_vel(60);  // TODO update value
        follower_.update_p(11);
        qrd_.isIntersection();
        state_ = 0;
      }else{
        break;
      }
    }
    case 0:  // going into the tub
    case 2:  // Travelling to next agent
    case 7:  // ignore the the intersection
      LCD.setCursor(0,1); LCD.print("tape follow");
      claw_.loop();
      if (qrd_.isIntersection()){
        follower_.stop();
        state_++;
      }else{
        follower_.loop();
      }
      break;
    case 1:  // Turning at the tub
      LCD.setCursor(0,0); LCD.print("turning");
      if (side_) maneuver_.turn(turn_degree_);
      else maneuver_.turn(-turn_degree_);
      if (maneuver_.loop()){
        state_ = 2;
        follower_.loop();
      }
      break;
    case 3:  // Aligning the Claw
      LCD.setCursor(0,0); LCD.print("straight");
      //if (side_) maneuver_.turn(-drive_distance_);
      //else maneuver_.turn(drive_distance_);
      if (side_) maneuver_.straight(-7);
      else maneuver_.straight(-7);
      if (maneuver_.loop()){
        state_ = 4;
      }else{
        break;
      }
    case 4:  // Waiting for claw to be ready
      LCD.setCursor(0,0); LCD.print("waiting");
      if (claw_.loop()){
        state_ = 5;
        if (side_) claw_.grab(sequences::RIGHT_CLAW, height[agents_]);  // TODO This should be the claw on the right side
        else claw_.grab(sequences::LEFT_CLAW, height[agents_]);
      }else{
        break;
      }
    case 5:  // Waiting for claw to grab
      LCD.setCursor(0,0); LCD.print("grab");
      if (claw_.loop()){
        if (side_) {
          claw_.release(sequences::RIGHT_CLAW);
  //        maneuver_.turn(drive_distance_);
     maneuver_.straight(10);

        } else {
          claw_.release(sequences::LEFT_CLAW);
    //      maneuver_.turn(-drive_distance_);
    maneuver_.straight(10);

        }
        agents_++;
        state_ = 6;
      }else{
        break;
      }
    case 6:  // Return back to tape followin
      LCD.setCursor(0,0); LCD.print("going straight");
      claw_.loop();
      if (maneuver_.loop()){
        if (agents_ == 5) state_ = 7;
        else if (agents_ == 7) state_ = 9;
        else state_ = 2;
      }
      break;
    case 8:
      maneuver_.straight(5);
      agents_++;
      state_ = 6;
      break;
    case 9:
      if (claw_.loop()) {
        if (side_) claw_.raise(sequences::RIGHT_CLAW);
        else claw_.raise(sequences::LEFT_CLAW);
        state_ = 10;
      }
      break;
    case 10:
      if (claw_.loop()) return true;
      break;
  }

  return false;
}

#if USE_UPDATE()
bool Pickup::update(){
  int8_t update_state_ = 0;
  int change = 0;
  follower_.stop();
  delay(1000);
  while (startbutton());
  while (!startbutton()){
    if (stopbutton()) update_state_ ++;
    if (update_state_ > 4) update_state_ = 0;

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
      SWITCH_CASES(0, drive_distance_)
      SWITCH_CASES(1, turn_degree_)
        case 2:
          to_ramp_ += change / 50;
          LCD.setCursor(0,0); LCD.print("to_ramp_");
          LCD.setCursor(0,1); LCD.print(to_ramp_);
        case 3:
          to_intersection_ += change / 50;
          LCD.setCursor(0,0); LCD.print("to intersection");
          LCD.setCursor(0,1); LCD.print(to_intersection_);
    }
  }
}
#endif  // USE_UPDATE()

}  // namespace stages
