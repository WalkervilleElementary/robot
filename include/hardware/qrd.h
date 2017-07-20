#ifndef WALKERVILLE_ROBOT_HARDWARE_QRD_H
#define WALKERVILLE_ROBOT_HARDWARE_QRD_H

#include "configs.h"

namespace hardware
{

const int LL_QRD_SENSOR_ = LL_QRD_SENSOR();
const int LC_QRD_SENSOR_ = LC_QRD_SENSOR();
const int RC_QRD_SENSOR_ = RC_QRD_SENSOR();
const int RR_QRD_SENSOR_ = RR_QRD_SENSOR();


class Qrd
{
private:
  // getTapeError
  int error_;
  int tape_threshold_;

public:
  Qrd();
  inline ~Qrd(){};

  int getTapeError();
  bool isIntersection();
#if USE_UPDATE()
  void update();
#endif  // USE_UPDATE()
};  // class Qrd

}  // namespace hardware

#endif  // WALKERVILLE_ROBOT_HARDWARE_QRD_H
