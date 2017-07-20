#include "configs.h"

#include <phys253.h>

#include "stages/gate.h"

namespace stages
{
Gate::Gate()
{
  state_ = 0;
  threshold_ = GATE_IR_STRENGTH_THRESHOLD();
  distance_ = GATE_WAITING_DISTANCE() * GEAR_RATIO() * 24 / WHEEL_DIAMETER() / PI;
#if USE_UPDATE()
  update_state_ = 0;
#endif  // USE_UPDATE()
#if CAUTIOUS_GATE_ROUTINE()
  gate_state_ = 0;
#endif  // CAUTIOUS_GATE_ROUTINE()
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
      // save encoder's start value as reference
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
        state_++;
      }
      LCD.println(encoder_.get(hardware::R_ENCODER_) - encoder_start_);
      break;
    case 2:  // waiting at the gate
    {
      uint32_t left = beacon_.leftIntensity();
      uint32_t right = beacon_.rightIntensity();
      LCD.clear();  LCD.home();
      LCD.setCursor(0,0); LCD.print(left);
      LCD.setCursor(0,1); LCD.print(right);
      if (left + right > threshold_)
      {
#if CAUTIOUS_GATE_ROUTINE()
        if (gate_state_ < 1)
        {
          follower_.stop();
          return false;
        }
#endif  // CAUTIOUS_GATE_ROUTINE()
        follower_.loop();
        return true;
      }
      else
      {
        follower_.stop();
#if CAUTIOUS_GATE_ROUTINE()
        gate_state_++;
#endif  // CAUTIOUS_GATE_ROUTINE()
      }
      break;
    }
    default:
      // this should never happen
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
    if (update_state_ > 2) update_state_ = 0;

    start_val = knob(6);
    delay(100);
    end_val = knob(6);

    change = (start_val - end_val)/4 ;
    LCD.clear();  LCD.home();
    switch (update_state_)
    {
      case 0:  // set distance
        distance_ += (change * GEAR_RATIO() * 24 / WHEEL_DIAMETER() / PI);
        LCD.setCursor(0,0); LCD.print("distance");
        LCD.setCursor(0,1); LCD.print(distance_);
        break;
      case 1:  // set threshold
        threshold_ += change;
        LCD.setCursor(0,0); LCD.print("threshold");
        LCD.setCursor(0,1); LCD.print(threshold_);
        break;
    }
  }
}
#endif  // macro USE_UPDATE()
}
