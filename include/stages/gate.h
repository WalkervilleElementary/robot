#ifndef WALKERVILLE_ROBOT_STAGES_GATE_H
#define WALKERVILLE_ROBOT_STAGES_GATE_H

#include "configs.h"

#include "templates/sequence.h"
#include "hardware/ir.h"
#include "sequences/tape.h"

namespace stages
{

class Gate : public templates::Sequence
{
private:
  sequences::Tape follower_;
  hardware::Ir ir_;
  int threshold_;  // configurable
  char state_;

#if USE_UPDATE()
  int start_val;
  int end_val;
  int change;
  char  update_state_;
#endif  // USE_UPDATE
public:
  Gate();
  inline ~Gate(){};
  void setup(sequences::Tape follower, hardware::Ir ir);
  bool loop();
#if USE_UPDATE()
  bool update();
#endif  // macro USE_UPDATE()
};  // class Gate

}  // namespace stages

#endif  // WALKERVILLE_ROBOT_STAGES_GATE_H
