#ifndef WALKERVILLE_ROBOT_STAGES_PICKUP_H
#define WALKERVILLE_ROBOT_STAGES_PICKUP_H

#include "configs.h"

#include "templates/sequence.h"
#include "hardware/qrd.h"
#include "sequences/claw.h"
#include "sequences/drivetrain.h"

#include <stdint.h>

namespace stages{

class Pickup : public templates::Sequence{

private:
  const hardware::Qrd& qrd_;
  const hardware::Encoder& encoder_;
  sequences::Claw& claw_;
  sequences::Drivetrain& driver_;

  static int8_t state_;
  static bool side_;
  static int8_t agents_;

  static int32_t to_ramp_;
  static int32_t to_intersection_;
  static int32_t to_tick_;
  static int32_t turn_degree_;

  static int32_t start_encoder_;
  static int32_t current_encoder_;
  static int32_t alignment_distance_;

  static const int8_t height[];

public:
  inline Pickup(const hardware::Qrd& qrd, const hardware::Encoder& encoder,
    sequences::Claw& claw, sequences::Drivetrain& driver):
    qrd_(qrd), encoder_(encoder), claw_(claw), driver_(driver) {};
  inline ~Pickup(){};

  bool loop();
  void side(bool left_surface);
#if USE_UPDATE()
  bool update();
#endif  // USE_UPDATE()
};  // class Pickup

}  // namespace stages

#endif  // WALKERVILLE_ROBOT_STAGES_PICKUP_H
