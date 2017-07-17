#ifndef WALKERVILLE_ROBOT_STAGES_PICKUP_H
#define WALKERVILLE_ROBOT_STAGES_PICKUP_H

#include "configs.h"

#include "templates/sequnce.h"
#include "hardware/ir.h"
#include "sequences/claw.h"
#include "sequences/maneuver"
#include "sequences/tape.h"

namespace stages
{

class Pickup : public templates::Sequence
{
private:
  hardware::Ir ir_;
  sequences::Claw claw_;
  sequnces::Maneuver maneuver_;
  sequnces::Tape follower_;

  char state_;

  int drive_distance_;
  int arm_degree_;
  int claw_degree_;

#if USE_UPDATE()
  char update_state_;
  int start_val;
  int end_val;
  int change;
#endif  // USE_UPDATE()

public:
  Pickup()
  inline ~Pickup(){};
  void setup(hardware::Ir ir, sequences::Claw claw,
              sequences::Maneuver maneuver, sequneces::Tape follower);

  bool loop();

#if USE_UPDATE()
  bool update()
#endif  // USE_UPDATE()
};  // class Pickup

}  // namespace stages

#endif  // WALKERVILLE_ROBOT_STAGES_PICKUP_H
