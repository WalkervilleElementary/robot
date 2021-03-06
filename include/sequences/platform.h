#pragma once
#include "configs.h"

#include "sequences/drivetrain.h"
#include "templates/sequence.h"

#include <stdint.h>

namespace sequences {

class Platform : public templates::Sequence {

private:
/**
 * States:
 *  0 = do nothing
 *  1 = raising platform
 *  2 = lower slightly so limit switch is no longer active
 *  3 = lower fully
 */
  static int8_t state_;
  static bool raised_;
  static int raise_speed_;
  static int lower_speed_top_;
  static int lower_speed_bottom_;
  static int maintain_power_;

  static unsigned long lower_time_;
  static int lower_speed_modifier_;

  static const uint8_t upper_switch_;
  static const uint8_t lower_switch_;
  static const uint8_t motor_number_;

public:
  inline Platform(){};
  inline ~Platform(){};
  bool loop();
  void stop();
  static void raise();
  static void lower();
#if USE_UPDATE()
  void update();
#endif  // USE_UPDATE()
};  // class tape

}  // namespace sequences
