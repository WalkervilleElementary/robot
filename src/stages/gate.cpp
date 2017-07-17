#include "configs.h"

#include <phys253.h>

#include "stages/gate.h"

namespace stages
{
Gate::Gate()
{
  threshold_ = GATE_IR_STRENGTH_THRESHOLD();
  state_ = 0;
#if USE_UPDATE()
  update_state_ = 0;
#endif  // USE_UPDATE()
}

void Gate::setup(sequences::Tape follower, hardware::Ir ir)
{
  ir_ = ir;
  follower_ = follower;
}

bool Gate::loop()
{
  switch (state_)
  {
    case 0:  // going towards gate
      if (ir_.strength() < threshold_)
      {
        follower_.loop();
      }
      else
      {
        follower_.stop();
        state_ ++;
      }
      break;
    case 1:  // waiting at the gate
      if (ir_.tenKHZ())
      {
        follower_.loop();
        return true;
      }
      else
      {
        follower_.stop();
      }
      break;
    default:
      // TODO this should never happen
      break;
  }
  return false;
}

#if USE_UPDATE()
bool Gate::update()
{
  while (!startbutton())
  {
    if(stopbutton()) update_state_ += 1;
    if (update_state_ > 1) update_state_ = 0;

    start_val = knob(6);
    delay(100);
    end_val = knob(6);

    change = (start_val - end_val)/4 ;
    LCD.clear();  LCD.home();
    switch (state_)
    {
    case 0:
      threshold_ += change;
      LCD.setCursor(0,0); LCD.print("threshold");
      LCD.setCursor(0,1); LCD.print(threshold_);
      break;
     }
  }
}
#endif  // marco USE_UPDATE()
}
