#ifndef WALKERVILLE_ROBOT_HARDWARE_IR_H
#define WALKERVILLE_ROBOT_HARDWARE_IR_H

#include "configs/configs.h"

namespace hardware
{

const int L_SENSOR_ = L_SENSOR();
const int R_SENSOR_ = R_SENSOR();

class Ir
{
private:
  // getTapeError
  int l_val_;
  int r_val_;
  int l_on_;
  int r_on_;
  int error_;
  int tape_threshold_; // configurable

public:
  Ir();
  inline ~Ir(){};
  
  int getTapeError();
  bool tenKHZ();
  int strength();

};  // class Ir

}  // namespace hardware

#endif  // WALKERVILLE_ROBOT_HARDWARE_IR_H