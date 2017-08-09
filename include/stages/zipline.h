#ifndef WALKERVILLE_ROBOT_STAGES_ZIPLINE_H
#define WALKERVILLE_ROBOT_STAGES_ZIPLINE_H

#include "configs.h"

#include "templates/sequence.h"
#include "hardware/beacon.h"
#include "hardware/encoder.h"
#include "sequences/drivetrain.h"
#include "sequences/platform.h"
#include "sequences/claw.h"

#include <stdint.h>

namespace stages {

class Zipline : public templates::Sequence {

private:
  sequences::Drivetrain& driver_;
  sequences::Platform& platform_;
  sequences::Claw& claw_;
  hardware::Encoder& encoder_;
  hardware::Qrd& qrd_;

  // constants
  static int32_t checkpoint_;
  static int32_t backup_distance_;
  static int16_t forward_power_;
  static int16_t backup_power_;

  static bool left_surface_;
  static int8_t intersections_;
  static int8_t count_;
  static int32_t encoder_start_;

  /**
   * States:
   *  0 = Initialization
   *  1 = Follow tape until intersection
   *  2 = Drive forward a distance
   *  3 = Go straight until checkpoint
   *  4 = Ram into zipline
   *  5 = Lower platform
   */
  static uint8_t state_;

public:
  inline Zipline(sequences::Drivetrain& driver, sequences::Platform& platform, sequences::Claw& claw,
                hardware::Encoder& encoder, hardware::Qrd& qrd)
                : driver_(driver), platform_(platform), claw_(claw),
                  encoder_(encoder), qrd_(qrd) {};
  inline ~Zipline() {};
  bool loop();
  void stop();
  void side(bool left_surface);
#if DEBUG()
  void set_state(uint8_t state);
#endif  // DEBUG()
#if USE_UPDATE()
  void update();
#endif  // macro USE_UPDATE()
};  // class Zipline

}  // namespace stages

#endif  // WALKERVILLE_ROBOT_STAGES_ZIPLINE_H
