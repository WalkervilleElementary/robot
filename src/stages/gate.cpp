#include "configs.h"

#include <phys253.h>

#include "stages/gate.h"

namespace stages{

int8_t Gate::state_ = 0;
#if CAUTIOUS_GATE_ROUTINE()
bool Gate::gate_high_ = false;
#endif  // CAUTIOUS_GATE_ROUTINE()

uint32_t Gate::threshold_ = GATE_IR_STRENGTH_THRESHOLD();
int32_t Gate::distance_ = hardware::Encoder::cmToTicks(GATE_WAITING_DISTANCE());

int32_t Gate::encoder_start_;

bool Gate::loop(){
  switch (state_){
    case 0:  // save encoder's start value as reference
      encoder_start_ = encoder_.getPosition();
      ++state_;
    case 1:  // going towards gate
      if (encoder_.getPosition() - encoder_start_ < distance_){
        driver_.commandLineFollow(0);
      }else{
        driver_.stop();
        ++state_;
      }
#if DEBUG()
      // LCD.println(encoder_.getPosition() - encoder_start_);
#endif  // DEBUG()
      break;
    case 2:  // waiting at the gate
    {
      uint32_t left = beacon_.leftIntensity();
      uint32_t right = beacon_.rightIntensity();
#if DEBUG()
      // LCD.setCursor(0,0); LCD.print(left);
      // LCD.setCursor(0,1); LCD.print(right);
#endif  // DEBUG()
      if (left + right < threshold_) {
#if CAUTIOUS_GATE_ROUTINE()
        if (!gate_high_){
          driver_.stop();
          return false;
        }
#endif  // CAUTIOUS_GATE_ROUTINE()
        driver_.commandLineFollow(1);
        return true;
      }else{
        driver_.stop();
#if CAUTIOUS_GATE_ROUTINE()
        gate_high_ = true;
#endif  // CAUTIOUS_GATE_ROUTINE()
      }
      break;
    }
    default:
      // this should never happen
#if DEBUG()
      driver_.stop();
      LCD.clear(); LCD.home();
      LCD.setCursor(0,0); LCD.print("Gate invalid state");
      delay(1000);
#endif  // DEBUG()
      break;
  }
  return false;
}

#if USE_UPDATE()
bool Gate::update(){
  int8_t update_state_ = 0;
  int change = 0;
  while (!startbutton()){
    if(stopbutton()) update_state_ += 1;
    if (update_state_ > 2) update_state_ = 0;

    int tune_val = knob(7);
    int start_val = knob(6);
    delay(100);
    int end_val = knob(6);
    if (tune_val > TUNE_THRESHOLD()){
      change = (start_val - end_val)/4 ;
    }
    LCD.clear();  LCD.home();
    switch (update_state_){
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

}  // namespace stages
