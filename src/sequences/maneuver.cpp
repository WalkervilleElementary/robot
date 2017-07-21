#include "sequences/maneuver.h"

#include <phys253.h>
#include <LiquidCrystal.h>

namespace sequences{

int8_t   Maneuver::state_ = 0;
uint32_t Maneuver::right_limit_;
uint32_t Maneuver::left_limit_;

const uint32_t Maneuver::distance_to_encoder_ = GEAR_RATIO() * 24.0 / WHEEL_DIAMETER() / PI; // TODO use float?
const uint32_t Maneuver::degree_to_distance_ = AXLE_LENGTH() * 2.0 * PI / 360.0;
const uint32_t Maneuver::gain_ = MANEUVER_GAIN();


bool Maneuver::straight(int distance){
  if (state_ != 0) return false;
  state_ = 1;
  right_limit_ = distance_to_encoder_ * distance;
  left_limit_ = right_limit_;
  right_limit_ += encoder_.get(hardware::R_ENCODER_);
  left_limit_  += encoder_.get(hardware::L_ENCODER_);
  return true;
}

bool Maneuver::turn(int degrees){
  if (state_ != 0) return false;
  state_ = 2;
  if (degrees > 0){
    right_limit_ = degree_to_distance_ * distance_to_encoder_ * degrees;
    left_limit_ = 0;
  }else{
    left_limit_ = degree_to_distance_ * distance_to_encoder_ * degrees;
    right_limit_ = 0;
  }

  right_limit_ += encoder_.get(hardware::R_ENCODER_);
  left_limit_  += encoder_.get(hardware::L_ENCODER_);
  return true;
}


bool Maneuver::loop(){
  switch(state_){
    case 1:  // straight
    case 2:  // turn
    {
      const uint32_t right_encoder = encoder_.get(hardware::R_ENCODER_);
      const uint32_t left_encoder  = encoder_.get(hardware::L_ENCODER_);
      int right_velocity = 0;
      int left_velocity = 0;
      if (right_encoder < right_limit_)
        right_velocity = gain_ * (right_limit_ - right_encoder) + 30;
      if (left_encoder < left_limit_)
        left_velocity = gain_ * (left_limit_ - left_encoder) + 30;

      if ((state_ == 2 && (right_velocity != 0 || left_velocity != 0)) ||
          (state_ == 1 && right_velocity != 0 && left_velocity != 0)){
        motor_.sendWheelVelocities(right_velocity, left_velocity);
        return false;
      }else{
        state_ = 0;
        motor_.stop();
      }
    }
    case 0:
      return true;
    default:
      // TODO throw error
      motor_.stop();
      LCD.clear(); LCD.home();
      LCD.setCursor(0,0); LCD.print("MANUEVER ENTERED WEIRD STATE");
      delay(1000);
  }
  return false;
}

}  // namespace sequnces
