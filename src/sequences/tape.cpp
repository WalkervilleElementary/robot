#include "sequences/tape.h"

#if DEVICE()
#include <phys253.h>
#include <LiquidCrystal.h>
#endif  // marco DEVICE()

namespace sequences
{
Tape::Tape()
{
  gain_t_ = GAIN_T();
  gain_p_ = GAIN_P();
  gain_i_ = GAIN_I();
  gain_d_ = GAIN_D();
  velocity_ =  VELOCITY();
#if USE_UPDATE()
  state_ = 0;
#endif  // USE_UPDATE()
}

void Tape::setup (hardware::Ir ir, hardware::Driver motor)
{
  ir_ = ir;
  motor_ = motor;
}

bool Tape::loop()
{
  error_ = ir_.getTapeError();
  command_ = computeCommand(error_, 100);
#if DEBUG() 
#if DEVICE()
  LCD.clear();  LCD.home() ;
  LCD.setCursor(0,0); LCD.print("motor commands");
  LCD.setCursor(0,1); LCD.print(velocity - command_);  
  LCD.setCursor(7,1); LCD.print( -(velocity + command_));
  LCD.setCursor(15,1); LCD.print(error_);
#endif  // marco DEVICE()
#endif  // marco DEBUG()
  motor_.sendMotorCommand(velocity_, command_);
}

int Tape::computeCommand(int error, int dt)
{
  i_error_ += error*1000/dt;
  i_error_ = (i_error_ > 0)? 5 : ((i_error_ < -5)? -5 : i_error_);
  kp_ = gain_p_ * error;
  ki_ = gain_i_ * i_error_;
  kd_ = gain_d_ * (error - prev_error_)*1000/dt;
  prev_error_ = error;
  return gain_t_*(kp_ + ki_ +  kd_);
}

#if USE_UPDATE()
void update()
{
#if DEVICE()
  while (!startbutton())
    {
      if (stopbutton()) state_ += 1;
      if (state_ > 4) state_ = 0;
      int start_val = knob(6);
      delay(100);
      int end_val = knob(6);

      int change = (start_val - end_val)/4 ;
      LCD.clear();  LCD.home() ;
          
      switch (state_)
      {
      case 0:
          gain_t_ += change;
          LCD.setCursor(0,0); LCD.print("gain_tot");
          LCD.setCursor(0,1); LCD.print(gain_t_);
          break;
      case 1:
          gain_p_ += change;
          LCD.setCursor(0,0); LCD.print("gain_p");
          LCD.setCursor(0,1); LCD.print(gain_p_);
          break;
      case 2:
          gain_i_ += change;
          LCD.setCursor(0,0); LCD.print("gain_i");
          LCD.setCursor(0,1); LCD.print(gain_i_);
          break;
      case 3:
          gain_d_ += end_val - start_val;
          LCD.setCursor(0,0); LCD.print("gain_d");
          LCD.setCursor(0,1); LCD.print(gain_d_);
          break;
      case 4:
          velocity_ += change;
          LCD.setCursor(0,0); LCD.print("velocity");
          LCD.setCursor(0,1); LCD.print(velocity_);
      }
    }
  }
#endif  // DEVICE()
}
#endif  // USE_UPDATE()

}  // namespace tape