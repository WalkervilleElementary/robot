#include "stages/pickup.h"

#include <phys253.h>
#include <LiquidCrystal.h>

namespace stages{

int8_t Pickup::state_ = 0;
int8_t Pickup::agents_ = 0;
uint32_t Pickup::drive_distance_ = 6;  // TODO make this configurable
uint32_t Pickup::turn_degree_ = 90;  // TODO make this configurable
bool Pickup::side_ = digitalRead(0);  // TODO set this up in qrd

bool Pickup::loop(){
  // States Description
  // 0      tape follow until we get to the tub while raising the claws
  // 1      turn towards the direction we want
  // 2      tape follow until intersection
  // 3      driving straight
  // 4      make sure claw is ready for action
  // 5      grabbing
  // 6      release claw and return to state 2
  claw_.loop();
  switch (state_){
    case 0:  // Travelling from Gate to tub
      //LCD.setCursor(0,0); LCD.print("going to tub");
      claw_.raise(0);  // TODO change to values defined by claw
      claw_.raise(1);
    case 2:  // Travelling to next agent
      //LCD.setCursor(0,1); LCD.print("tape follow");
      if (qrd_.isIntersection()){
        follower_.stop();
        state_++;
      }else{
        follower_.loop();
      }
      break;
   case 1:   // Turning at the tub
      //LCD.setCursor(0,0); LCD.print("turning");
      if (side_) maneuver_.turn(turn_degree_);
      else maneuver_.turn(-turn_degree_);
      if (maneuver_.loop()){
        state_ = 2;
        follower_.loop();
      }
      break;
    case 3:  // Aligning the Claw
      //LCD.setCursor(0,0); LCD.print("straight");
      maneuver_.straight(drive_distance_);
      if (maneuver_.loop()){
        state_ = 4;
      }else{
        break;
      }
     case 4:  // Waiting for claw to be ready
      //LCD.setCursor(0,0); LCD.print("waiting");
      if (claw_.loop()){
        state_ = 5;
        if (side_) claw_.grab(0);  // TODO This should be the claw on the right side
        else claw_.grab(1);
      }else{
        break;
      }
    case 5:  // Waiting for claw to grab
      //LCD.setCursor(0,0); LCD.print("grab");
      if (claw_.loop()){
        state_ = 2;
        if (side_) claw_.release(0);  // TODO THis should be the claw on the right side
        else claw_.release(1);
        agents_++;
        if (agents_ == 6) return true;
      }else{
        break;
      }
  }

  return false;
}

#if USE_UPDATE()
bool Pickup::update(){
  int8_t update_state_ = 0;
  follower_.stop();
  delay(1000);
  while (startbutton());
  while (!startbutton()){
    if (stopbutton()) update_state_ ++;
    if (update_state_ > 2) update_state_ = 0;

    int tune_val = knob(7);
    if (tune_val < TUNE_THRESHOLD()){
      LCD.clear(); LCD.home();
      LCD.setCursor(0,0); LCD.print("Tuning Off");
      LCD.setCursor(0,1); LCD.print(tune_val);
      delay(100);
    }
    else{
      int start_val = knob(6);
      delay(100);
      int end_val = knob(6);

      int change = start_val - end_val;

      LCD.clear(); LCD.home();

      switch (update_state_){
        case 0:
          drive_distance_ += change / 50;
          LCD.setCursor(0,0); LCD.print("drive distance");
          LCD.setCursor(0,1); LCD.print(drive_distance_);
        case 1:
          turn_degree_ += change / 50;
          LCD.setCursor(0,0); LCD.print("turn degree");
          LCD.setCursor(0,1); LCD.print(turn_degree_);
      }
    }

  }
}
#endif  // USE_UPDATE()

}  // namespace stages
