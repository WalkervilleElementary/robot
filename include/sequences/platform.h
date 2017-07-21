#pragma once

#include "configs.h"

#include "hardware/driver.h"
#include "templates/sequence.h"

#include <stdint.h>

namespace sequences{

class Platform : public templates::Sequence{

private:
/**
 * States:
 *  0 = stopped
 *  1 = raising platform
 *  2 = lower slightly so limit switch is no longer active
 *  3 = lower slowly while backing up
 */
  static int8_t state_;
  static int raise_speed_;
  static int lower_speed_;
  static int backup_speed_;

  static const uint8_t upper_switch_;
  static const uint8_t lower_switch_;
  static const uint8_t motor_number_;

  const hardware::Driver& driver_;

public:
  inline Platform(const hardware::Driver &driver): driver_(driver) {};
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
