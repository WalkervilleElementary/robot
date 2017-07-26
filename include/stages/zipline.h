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

  static uint32_t encoder_start_;
  static uint32_t ticks_;

  static uint32_t distance_to_turn_;
  static uint32_t distance_to_zipline_;
  static uint32_t ir_start_;

  // constants
  static int forward_speed_;  // for dead reckoning
  static int reverse_speed_;  // for backing up from zipline
  static uint8_t beacon_error_;

  static bool left_surface_;

  /**
   * Margin of error for beacon readings. When left & right encoder readings
   * get below this margin, robot will leave the tape.
   */
  static uint8_t error_margin_;

  /**
   * States:
   *  0 = Initialization
   *  1 = Follow tape
   *  2 = Follow beacon
   *  3 = Dead reckoning
   *  4 = Error state
   */
  static uint8_t state_;

public:
  inline Zipline(sequences::Tape& follower, sequences::Platform& platform,
                sequences::Maneuver& maneuver, hardware::Beacon& beacon,
                hardware::Driver& driver, hardware::Encoder& encoder)
                : follower_(follower), platform_(platform),
                  maneuver_(maneuver), beacon_(beacon),
                  driver_(driver), encoder_(encoder) {};
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
