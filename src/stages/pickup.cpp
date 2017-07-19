#include "stages/pickup.h"

#include <phys253.h>
#include <LiquidCrystal.h>

namespace stages
{

Pickup::Pickup()
{
  state_ = 0;
  agents_ = 0;
  drive_distance_ = 6;  // TODO make this configurable
  turn_distance_ = 90;  // TODO make this configurable
  side_ = digitalRead(0);  // TODO set this up in ir
}

void Pickup::setup(hardware::Ir ir, sequences::Claw claw,
            sequences::Maneuver maneuver, sequences::Tape follower)
{
  ir_ = ir;
  claw_ = claw;
  maneuver_ = maneuver;
  follower_ = follower;
}

bool Pickup::loop()
{
  // States Description
  // 0      tape follow until we get to the tub while raising the claws
  // 1      turn towards the direction we want
  // 2      tape follow until intersection
  // 3      driving straight
  // 4      make sure claw is ready for action
  // 5      grabbing
  // 6      release claw and return to state 2
  claw_.loop();
  switch (state_)
  {
    case 0:  // Travelling from Gate to tub
      claw_.raise(0);  // TODO change to values defined by claw
      claw_.raise(1);
    case 2:  // Travelling to next agent
      if (ir_.isIntersection())
      {
        follower_.stop();
        state_++;
      }
      else
      {
        follower_.loop();
      }
      break;
   case 1:   // Turning at the tub
      if (side_) maneuver_.turn(turn_distance_);
      else maneuver_.turn(-turn_distance_);
      if (maneuver_.loop())
      {
        state_ = 2;
        follower_.loop();
      }
      break;
    case 3:  // Aligning the Claw
      maneuver_.straight(drive_distance_);
      if (maneuver_.loop())
      {
        state_ = 4;
      }
      else
      {
        break;
      }
     case 4:  // Waiting for claw to be ready
      if (claw_.loop())
      {
        state_ = 5;
        if (side_) claw_.grab(0);  // TODO This should be the claw on the right side
        else claw_.grab(1);
      }
      else
      {
        break;
      }
    case 5:  // Waiting for claw to grab
      if (claw_.loop())
      {
        state_ = 2;
        if (side_) claw_.release(0);  // TODO THis should be the claw on the right side
        else claw_.release(1);
        agents_++;
        if (agents_ == 6)
          return true;
      }
      else
      {
        break;
      }
  }

/*
  switch(state_)
  {
    // tape follow until intersection
    case 7:
      claw_.loop();
    case 0:  // right after the gate
    case 3:  // Just turn into the circle
      if (ir_.isIntersection())
      {
        follower_.stop();
        state_++;
      }
      else
      {
        follower_.loop();
      }
      break;
    case 1:  // at intersection entering tub
      if (side_) maneuver_.turn(turn_distance_)  // TODO fix actual value
      else maneuver_.turn(-turn_distance_)
      state_ = 2;
    case 2:  // turning at the intersection of the tub
      if (maneuver_.loop())
        follower_.loop();
        state_ = 3;
      break;
    case 4:  // the line following sensors are at the agents
      maneuver_.straight(drive_distance_)  // TODO fix actual value
      state_ = 5;
    case 5:  // aligning the claw
      if (maneuver_.loop())
      {
        if (side_) claw_.grab(0);  // TODO update to use variable in Claw
        if claw_.grab(1);  // TODO
        state_ = 6;
      }
    case 6:  // grabbing the agent
      if (claw_.loop())
      {
        if (side_) claw_.release(0);  // TODO change to left and right
        else claw_.release(0);
        claw_.loop();
        state_ = 7;

      }
      break;
    case 8:
      if (claw_.loop)
      {
        agents ++;
        if (agents == 6)
        {
          return true;
        }
        else
        {
          state_ = 4;
        }
      }
  }
*/
  return false;
}

#if USE_UPDATE()
bool Pickup::update()
{
  follower_.stop();
  delay(1000);
  while (startbutton());
  while (!startbutton())
  {
    if (stopbutton()) update_state_ ++;
    if (update_state_ > 2) update_state_ = 0;

    start_val = knob(6);
    delay(100);
    end_val = knob(6);

    change = start_val - end_val;

    LCD.clear(); LCD.home();

    switch (update_state_)
    {
      case 0:
        drive_distance_ += change / 50;
        LCD.setCursor(0,0); LCD.print("drive distance");
        LCD.setCursor(0,1); LCD.print(drive_distance_);
      case 1:
        turn_distance_ += change / 50;
        LCD.setCursor(0,0); LCD.print("turn distance");
        LCD.setCursor(0,1); LCD.print(turn_distance_);
    }
  }
}
#endif  // USE_UPDATE()

}  // namespace stages
