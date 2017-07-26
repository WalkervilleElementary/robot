#ifndef WALKERVILLE_ROBOT_STAGES_ZIPLINE_H
#define WALKERVILLE_ROBOT_STAGES_ZIPLINE_H

#include "configs.h"

#include "templates/sequence.h"
#include "hardware/beacon.h"
#include "hardware/encoder.h"
#include "sequences/maneuver.h"
#include "sequences/platform.h"
#include "sequences/tape.h"

#include <stdint.h>

namespace stages{

class Zipline : public templates::Sequence{

private:
  sequences::Tape& follower_;
  sequences::Maneuver& maneuver_;
  sequences::Platform& platform_;
  hardware::Beacon& beacon_;
  hardware::Driver& driver_;
  hardware::Encoder& encoder_;
  hardware::Qrd& qrd_;

  static uint32_t encoder_start_;
  static uint32_t ticks_;

  static uint32_t distance_to_turn_;
  static uint32_t distance_to_zipline_;

  // constants
  static int forward_speed_;  // for dead reckoning

  static bool left_surface_;

  /**
   * States:
   *  0 = Initialization
   *  1 = Follow tape until first intersection
   *  2 = Follow beacon
   *  3 = Turn toward zipline
   *  4 = Backup while raising platform
   *  5 = Dead reckoning
   *  6 = Error state
   */
  static uint8_t state_;

public:
  inline Zipline(sequences::Tape& follower, sequences::Platform& platform,
                sequences::Maneuver& maneuver, hardware::Beacon& beacon,
                hardware::Driver& driver, hardware::Encoder& encoder,
                hardware::Qrd& qrd)
                : follower_(follower), platform_(platform),
                  maneuver_(maneuver), beacon_(beacon), driver_(driver),
                  encoder_(encoder), qrd_(qrd) {};
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
