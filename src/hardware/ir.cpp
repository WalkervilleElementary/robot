#include "hardware/ir.h"

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
#else 
  l_val_ = 800;
  r_val_ = 800;
#endif  // DEVICE

  l_on_ = l_val_ > tape_threshold_;
  r_on_ = r_val_ > tape_threshold_;

  if (l_on_ && r_on_) return 0;
  else if(l_on_ && !r_on_) return 1;
  else if(!l_on_ && r_on_) return -1;
  else return (error_ > 0)? 5 : -5;
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

}  // namespace hardware