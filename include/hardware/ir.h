#ifndef WALKERVILLE_ROBOT_HARDWARE_IR_H
#define WALKERVILLE_ROBOT_HARDWARE_IR_H

#include "configs.h"

namespace hardware
{

const int L_QRD_SENSOR_ = L_QRD_SENSOR();
const int R_QRD_SENSOR_ = R_QRD_SENSOR();

class Ir
{
private:
  // getTapeError
  int l_val_;
  int r_val_;
  bool l_on_;
  bool r_on_;
  int error_;
  int tape_threshold_;

public:
  Ir();
  inline ~Ir(){};

  int getTapeError();
  bool tenKHZ();
  int strength();
#if USE_UPDATE()
  void update();
#endif  // USE_UPDATE()
};  // class Ir

}  // namespace hardware

#endif  // WALKERVILLE_ROBOT_HARDWARE_IR_H
