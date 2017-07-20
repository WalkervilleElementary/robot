#ifndef WALKERVILLE_ROBOT_STAGES_PICKUP_H
#define WALKERVILLE_ROBOT_STAGES_PICKUP_H

#include "configs.h"

#include "templates/sequence.h"
#include "hardware/qrd.h"
#include "sequences/claw.h"
#include "sequences/maneuver.h"
#include "sequences/tape.h"

namespace stages
{

class Pickup : public templates::Sequence
{
private:
  hardware::Qrd qrd_;
  sequences::Claw claw_;
  sequences::Maneuver maneuver_;
  sequences::Tape follower_;

  char state_;
  bool side_;
  int agents_;

  int turn_distance_;
  int drive_distance_;

#if USE_UPDATE()
  char update_state_;
  int start_val;
  int end_val;
  int change;
#endif  // USE_UPDATE()

public:
  Pickup();
  inline ~Pickup(){};
  void setup(hardware::Qrd qrd, sequences::Claw claw,
              sequences::Maneuver maneuver, sequences::Tape follower);

  bool loop();

#if USE_UPDATE()
  bool update();
#endif  // USE_UPDATE()
};  // class Pickup

}  // namespace stages

#endif  // WALKERVILLE_ROBOT_STAGES_PICKUP_H
