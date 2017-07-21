#ifndef WALKERVILLE_ROBOT_STAGES_PICKUP_H
#define WALKERVILLE_ROBOT_STAGES_PICKUP_H

#include "configs.h"

#include "templates/sequence.h"
#include "hardware/qrd.h"
#include "sequences/claw.h"
#include "sequences/maneuver.h"
#include "sequences/tape.h"

#include <stdint.h>

namespace stages{

class Pickup : public templates::Sequence{

private:
  const hardware::Qrd& qrd_;
  sequences::Claw& claw_;
  sequences::Maneuver& maneuver_;
  sequences::Tape& follower_;

  static int8_t state_;
  static bool side_;
  static int8_t agents_;

  static uint32_t turn_degree_;
  static uint32_t drive_distance_;

public:
  Pickup(hardware::Qrd qrd, sequences::Claw claw,
              sequences::Maneuver maneuver, sequences::Tape follower):
              qrd_(qrd), claw_(claw), maneuver_(maneuver), follower_(follower) {};
  inline ~Pickup(){};

  bool loop();

#if USE_UPDATE()
  bool update();
#endif  // USE_UPDATE()
};  // class Pickup

}  // namespace stages

#endif  // WALKERVILLE_ROBOT_STAGES_PICKUP_H
