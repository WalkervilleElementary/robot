#include "stages/pickup.h"

namespace stages
{

Pickup::Pickup()
{
  // TODO initialize configurable variables
}

void setup(hardware::Ir ir, sequences::Claw claw,
            sequnces::Maneuver maneuver, sequences::Tape follower)
{
  ir_ = ir;
  claw_ = claw;
  maneuver_ = maneuver;
  follower_ = follower;
}

bool loop()
{
  switch(state_)
  {
    // tape follow until intersection
    case 0:  // right after the gate
    case 3:  // inside the circle
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
      // TODO change behavouir based on LEFT or RIGHT field
      maneuver_.turn(90) // TODO fix actual value
      state_ 2:
    case 2:  // turning at the intersection of the tub
      if (maneuver_.loop())
        follower_.loop();
        state_ = 3;
      break;
    case 4:  // the line following sensors are at the agents
      maneuver_.straight(10) // TODO fix actual value
      state_ = 5;
    case 5:  // aligning the claw
      if (maneuver_.loop())
      {
        claw_.grab(0);  // TODO change to left and right
        claw_.loop();
        state_ = 6;
      }
      break;
    case 6:  // grabbing the agent
      if (claw_.loop())
      {
        claw_.release(0);  // TODO change to left and right
        claw_.loop();
        state_ = 3;
        // TODO count number of agents grabbed.
        // if agents grabbed = 6 return true
      }
      break;
  }
  return false;
}

}  // namespace stages
