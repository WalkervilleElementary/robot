#ifndef WALKERVILLE_ROBOT_HARDWARE_BEACON_H
#define WALKERVILLE_ROBOT_HARDWARE_BEACON_H

#include "configs.h"

namespace hardware{

static const int L_BEACON_SENSOR_ = L_BEACON_SENSOR();
static const int R_BEACON_SENSOR_ = R_BEACON_SENSOR();

class Beacon{

private:
  static const uint8_t num_samples_ = 67;
  static const uint32_t sample_rate_ = 61000;

  /// don't update more frequently than this
  static const unsigned long update_interval_ = 20;

  static unsigned long left_update_;
  static unsigned long right_update_;
  static uint32_t left_intensity_;
  static uint32_t right_intensity_;
  static uint8_t uncertainty_;

public:
  inline Beacon(){};
  inline ~Beacon(){};

  static int8_t getTapeError();
  static uint32_t leftIntensity();
  static uint32_t rightIntensity();
  void update();
};  // class Beacon

}  // namespace hardware

#endif  // WALKERVILLE_ROBOT_HARDWARE_BEACON_H
