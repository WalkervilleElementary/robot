#include "configs.h"

#include <phys253.h>

#include "stages/gate.h"

namespace stages
{
Gate::Gate()
{
  state_ = 0;
#if USE_UPDATE()
  update_state_ = 0;
#endif  // USE_UPDATE()
}

void Gate::setup(const sequences::Tape& follower, const hardware::Beacon& beacon, const hardware::Encoder& encoder)
{
  beacon_ = beacon;
  follower_ = follower;
  encoder_ = encoder;
}

bool Gate::loop()
{
  switch (state_)
  {
    case 0:
      encoder_start_ = encoder_.get(hardware::R_ENCODER_); 
      state_++;
    case 1:  // going towards gate
      if (encoder_.get(hardware::R_ENCODER_) - encoder_start_ < distance_)
      {
        follower_.loop();
      }
      else
      {
        follower_.stop();
        state_ ++;
      }
      LCD.println(encoder_.get(hardware::R_ENCODER_) - encoder_start_);
      break;
    case 2:  // waiting at the gate
      if (beacon_.leftIntensity() + beacon_.rightIntensity() > 150)
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
    switch (update_state_)
    {
      case 0:
        distance_ += change;
        LCD.setCursor(0,0); LCD.print("distance");
        LCD.setCursor(0,1); LCD.print(distance_);
        break;
    }
  }
}
#endif  // marco USE_UPDATE()
}
