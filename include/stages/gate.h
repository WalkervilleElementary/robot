#ifndef WALKERVILLE_ROBOT_STAGES_GATE_H
#define WALKERVILLE_ROBOT_STAGES_GATE_H

#include "configs.h"

#include "templates/sequence.h"
#include "hardware/beacon.h"
#include "hardware/encoder.h"
#include "sequences/tape.h"

namespace stages
{

class Gate : public templates::Sequence
{
private:
  sequences::Tape follower_;
  hardware::Beacon beacon_;
  hardware::Encoder encoder_;
  unsigned int encoder_start_;
  int distance_;  // configurable
  char state_;
  int threshold_;

#if USE_UPDATE()
  int start_val;
  int end_val;
  int change;
  char update_state_;
#endif  // USE_UPDATE()

#if CAUTIOUS_GATE_ROUTINE()
  /// gate state: 0 = unknown, 1 = closed
  char gate_state_;
#endif  // CAUTIOUS_GATE_ROUTINE()

public:
  Gate();
  inline ~Gate(){};
  void setup(const sequences::Tape& follower, const hardware::Beacon& beacon, const hardware::Encoder& encoder);
  bool loop();
#if USE_UPDATE()
  bool update();
#endif  // macro USE_UPDATE()
};  // class Gate

}  // namespace stages

#endif  // WALKERVILLE_ROBOT_STAGES_GATE_H
