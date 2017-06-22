#include "configs.h"

#if DEVICE()
#include "ir.h"

#include <phys253.h>
#else  // end DEVICE(); start !DEVICE()
#include "hardware/ir.h"
#endif  // end !DEVICE()

namespace hardware
{

Ir::Ir()
{
  tape_threshold_ = TAPE_IR_STRENGTH_THRESHOLD();
}

int Ir::getTapeError()
{
#if DEVICE()
  l_val_ = analogRead(L_SENSOR_);
  r_val_ = analogRead(R_SENSOR_);
#else  // end DEVICE(); start !DEVICE()
  l_val_ = 800;
  r_val_ = 800;
#endif  // end !DEVICE()

#if DEBUG()
#if DEVICE()
  LCD.home();
  LCD.setCursor(0,0); LCD.print(l_val_);
  LCD.setCursor(7,0); LCD.print(r_val_);
#endif  // DEVICE()
#endif  // DEBUG()
  l_on_ = l_val_ > tape_threshold_;
  r_on_ = r_val_ > tape_threshold_;

  if (l_on_ && r_on_) error_ = 0;
  else if(l_on_ && !r_on_) error_ = 1;
  else if(!l_on_ && r_on_) error_ = -1;
  else error_ = (error_ > 0)? 5 : -5;

  return error_;
}

bool Ir::tenKHZ()
{
  // TODO
  return true;
}

int Ir::strength()
{
  // TODO
  return 1000;
}

#if USE_UPDATE()
void Ir::update()
{
#if DEVICE()
  while(!startbutton)
  {
    int start_val = knob(6);
    delay(100);
    int end_val = knob(6);
    tape_threshold_ += (end_val - start_val)/4;
    LCD.clear();  LCD.home() ;
    LCD.setCursor(0,0); LCD.print("threshold");
    LCD.setCursor(0,1); LCD.print(tape_threshold_);
  }
#endif  // DEVICE()
}
#endif  // USE_UPDATE()

}  // namespace hardware
