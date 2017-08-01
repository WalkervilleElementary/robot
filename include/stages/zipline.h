#ifndef WALKERVILLE_ROBOT_STAGES_ZIPLINE_H
#define WALKERVILLE_ROBOT_STAGES_ZIPLINE_H

#include "configs.h"

#include "templates/sequence.h"
#include "hardware/beacon.h"
#include "hardware/encoder.h"
#include "sequences/drivetrain.h"
#include "sequences/platform.h"

#include <stdint.h>

namespace stages{

class Zipline : public templates::Sequence{

private:
  sequences::Drivetrain& driver_;
  sequences::Platform& platform_;
  hardware::Beacon& beacon_;
  hardware::Encoder& encoder_;
  hardware::Qrd& qrd_;

  static int32_t encoder_start_;
  static int32_t ticks_;

  // constants
  static int32_t turn_distance_;
  static int32_t turn_degrees_;
  static int32_t tape_distance_;
  static int32_t backup_distance_;
  // static uint32_t distance_to_zipline_;
  static int forward_speed_;  // for dead reckoning

  static bool left_surface_;

  /**
   * States:
   *  0 = Initialization
   *  1 = Follow tape until first intersection
   *  2 = Follow tape a little more
   *  3 = Follow beacon
   *  4 = Turn toward zipline
   *  5 = Backup and raise platform
   *  6 = Dead reckoning
   *  7 = Backup while lowering platform
   */
  static uint8_t state_;

public:
  inline Zipline(sequences::Drivetrain& driver, sequences::Platform& platform,
                hardware::Beacon& beacon, hardware::Encoder& encoder,
                hardware::Qrd& qrd)
                : driver_(driver), platform_(platform),
                  beacon_(beacon), encoder_(encoder), qrd_(qrd) {};
  inline ~Zipline(){};
  bool loop();
  void stop();
#if DEBUG()
  void set_state(uint8_t state);
#endif  // DEBUG()
#if USE_UPDATE()
  void update();
#endif  // macro USE_UPDATE()
};  // class Zipline

}  // namespace stages

#endif  // WALKERVILLE_ROBOT_STAGES_ZIPLINE_H
