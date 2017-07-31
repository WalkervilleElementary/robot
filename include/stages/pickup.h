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
  const hardware::Encoder& encoder_;
  const hardware::Driver& motor_;
  sequences::Claw& claw_;
  sequences::Maneuver& maneuver_;
  sequences::Tape& follower_;


  static int8_t state_;
  static bool side_;
  static int8_t agents_;

  static uint32_t start_encoder_;
  static uint32_t current_encoder_;

  static uint32_t to_ramp_;
  static uint32_t to_intersection_;

  static uint32_t turn_degree_;
  static uint32_t drive_distance_;

  static const int8_t height[];

public:
  Pickup(const hardware::Qrd& qrd, const hardware::Encoder& encoder,
          const hardware::Driver& driver, sequences::Claw& claw,
          sequences::Maneuver& maneuver, sequences::Tape& follower):
              qrd_(qrd), encoder_(encoder), motor_(driver),
              claw_(claw), maneuver_(maneuver), follower_(follower) {};
  inline ~Pickup(){};

  bool loop();

#if USE_UPDATE()
  bool update();
#endif  // USE_UPDATE()
};  // class Pickup

}  // namespace stages

#endif  // WALKERVILLE_ROBOT_STAGES_PICKUP_H
