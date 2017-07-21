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

public:
  inline Beacon(){};
  inline ~Beacon(){};

  static int8_t getTapeError();
  uint32_t leftIntensity() const;
  uint32_t rightIntensity() const;
};  // class Beacon

}  // namespace hardware

#endif  // WALKERVILLE_ROBOT_HARDWARE_BEACON_H