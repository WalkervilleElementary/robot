#include "sequences/maneuver.h"

#include <phys253.h>
#include <LiquidCrystal.h>

namespace sequences
{

Maneuver::Maneuver()
{
  state_ = 0;
  distance_to_encoder_ = GEAR_RATIO() * 24 / WHEEL_DIAMETER() / PI;
  degree_to_distance_ = AXLE_LENGTH() * 2.0 * PI / 360.0;
  gain_ = MANEUVER_GAIN();
}

void Maneuver::setup(const hardware::Driver &motor, const hardware::Encoder &encoder)
{
  motor_ = motor;
  encoder_ = encoder;
}

bool Maneuver::straight(int distance)
{
  if (state_ != 0) return false;
  state_ = 1;
  right_limit_ = distance_to_encoder_ * distance;
  left_limit_ = right_limit_;
  right_limit_ += encoder_.get(hardware::R_ENCODER_);
  left_limit_ +=  encoder_.get(hardware::L_ENCODER_);
  return true;
}

bool Maneuver::turn(int degrees)
{
  if (state_ != 0) return false;
  state_ = 2;
  if (degrees > 0)
  {
    right_limit_ = degree_to_distance_ * distance_to_encoder_ * degrees;
    left_limit_ = 0;
  }
  else
  {
    left_limit_ = -degree_to_distance_ * distance_to_encoder_ * degrees;
    right_limit_ = 0;
  }

  right_limit_ += encoder_.get(hardware::R_ENCODER_);
  left_limit_ +=  encoder_.get(hardware::L_ENCODER_);
  return true;
}


bool Maneuver::loop()
{
  switch(state_)
  {
    case 1:  // straight
    case 2:  // turn
    {
      const unsigned int right_encoder = encoder_.get(hardware::R_ENCODER_);
      const unsigned int left_encoder =  encoder_.get(hardware::L_ENCODER_);
      unsigned int right_velocity = 0;
      unsigned int left_velocity = 0;
      if (right_encoder < right_limit_)
        right_velocity = gain_ * (right_limit_ - right_encoder + 30);
      if (left_encoder < left_limit_)
        left_velocity = gain_ * (left_limit_ - left_encoder + 30);

      if ((state_ = 2 && (right_velocity != 0 || left_velocity != 0)) ||
          (state_ = 1 && right_velocity != 0 && left_velocity != 0))
      {
        motor_.sendWheelVelocities(right_velocity, left_velocity);
        return false;
      }
      else
      {
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
