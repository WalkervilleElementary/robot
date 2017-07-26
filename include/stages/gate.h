#ifndef WALKERVILLE_ROBOT_STAGES_GATE_H
#define WALKERVILLE_ROBOT_STAGES_GATE_H

#include "configs.h"

#include "templates/sequence.h"
#include "hardware/beacon.h"
#include "hardware/encoder.h"
#include "sequences/tape.h"

#include <stdint.h>

namespace stages{

class Gate : public templates::Sequence{

private:
  sequences::Tape& follower_;
  hardware::Beacon& beacon_;
  hardware::Encoder& encoder_;

  static uint32_t  encoder_start_;
  static uint32_t distance_;  // configurable
  static int8_t state_;
  static uint32_t threshold_;

#if CAUTIOUS_GATE_ROUTINE()
  /// gate state: 0 = unknown, 1 = closed
  static bool gate_low_;
#endif  // CAUTIOUS_GATE_ROUTINE()

public:
  inline Gate(sequences::Tape& follower, hardware::Beacon& beacon, hardware::Encoder& encoder):
              follower_(follower), beacon_(beacon), encoder_(encoder) {};
  inline ~Gate(){};
  bool loop();
#if USE_UPDATE()
  bool update();
#endif  // macro USE_UPDATE()
};  // class Gate

}  // namespace stages

#endif  // WALKERVILLE_ROBOT_STAGES_GATE_H
