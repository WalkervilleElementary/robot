#ifndef WALKERVILLE_ROBOT_SEQUENCES_MANEUVER_H
#define WALKERVILLE_ROBOT_SEQUENCES_MANEUVER_H

#include "configs.h"
#include "templates/sequence.h"

#include "hardware/driver.h"
#include "hardware/encoder.h"

#include <stdint.h>

namespace sequences{

class Maneuver : public templates::Sequence{

private:
  static int8_t state_;
  static uint32_t right_limit_;
  static uint32_t left_limit_;
  static int8_t offset_;
  static bool backward_;

  static const float distance_to_encoder_;
  static const float degree_to_distance_;
  static const uint32_t gain_;

  const hardware::Driver& motor_;
  const hardware::Encoder& encoder_;

public:
  inline Maneuver(const hardware::Driver &motor, const hardware::Encoder &encoder):
                motor_(motor), encoder_(encoder){};
  inline ~Maneuver(){};

  bool straight(int distance);
  bool turn(int degrees);

  bool loop();
#if USE_UPDATE()
  void update();
#endif  // USE_UPDATE()
};  // class Maneuver

}  // namespace sequences

#endif  // WALKERVILLE_ROBOT_SEQUENCES_MANEUVER_H

