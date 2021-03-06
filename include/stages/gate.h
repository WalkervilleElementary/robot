#ifndef WALKERVILLE_ROBOT_STAGES_GATE_H
#define WALKERVILLE_ROBOT_STAGES_GATE_H

#include "configs.h"

#include "templates/sequence.h"
#include "hardware/beaconwatcher.h"
#include "hardware/encoder.h"
#include "sequences/drivetrain.h"

#include <stdint.h>

namespace stages {

class Gate : public templates::Sequence {

private:
  sequences::Drivetrain& driver_;
  hardware::BeaconWatcher& beaconWatcher_;
  hardware::Encoder& encoder_;

  static int32_t  encoder_start_;
  static int32_t distance_;  // configurable
  static uint32_t threshold_;

  /**
   * State:
   * 0 = initialize
   * 1 = go toward gate
   * 2 = wait for beacon
   */
  static int8_t state_;

  static bool gate_10k_;

public:
  inline Gate(sequences::Drivetrain& driver, hardware::BeaconWatcher& beaconWatcher, hardware::Encoder& encoder):
              driver_(driver), beaconWatcher_(beaconWatcher), encoder_(encoder) {};
  inline ~Gate() {};
  bool loop();
#if USE_UPDATE()
  bool update();
#endif  // macro USE_UPDATE()
};  // class Gate

}  // namespace stages

#endif  // WALKERVILLE_ROBOT_STAGES_GATE_H
