#ifndef WALKERVILLE_ROBOT_HARDWARE_QRD_H
#define WALKERVILLE_ROBOT_HARDWARE_QRD_H

#include "configs.h"

#include <stdint.h>

namespace hardware{

static const uint8_t LL_QRD_SENSOR_ = LL_QRD_SENSOR();
static const uint8_t LC_QRD_SENSOR_ = LC_QRD_SENSOR();
static const uint8_t RC_QRD_SENSOR_ = RC_QRD_SENSOR();
static const uint8_t RR_QRD_SENSOR_ = RR_QRD_SENSOR();


class Qrd{

private:
  static const int error_array_[];
  static bool invalid_;
  // getTapeError
  static int8_t error_;
  static int tape_threshold_; // this is not sized because this is how analogRead works

public:
  inline Qrd(){};
  inline ~Qrd(){};

  static int8_t getTapeError();
  static bool isIntersection();
  static bool isIntersection2(bool isLeft);
#if USE_UPDATE()
  static void update();
#endif  // USE_UPDATE()
};  // class Qrd

}  // namespace hardware

#endif  // WALKERVILLE_ROBOT_HARDWARE_QRD_H
