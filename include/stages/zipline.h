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

  // constants
  static float pause_distance_;
  static float backup_distance_;
  static int16_t forward_speed_;
  static int16_t ram_speed_;
  static int16_t backup_speed_;

  static bool left_surface_;
  static int8_t intersections_;
  static int8_t count_;

  /**
   * States:
   *  0 = Initialization
   *  1 = Follow tape until intersection
   *  2 = Drive forward a distance
   *  3 = Go straight (toward zipline)
   *  4 = Check to make sure platform is fully raised
   *  5 = Go straight again (ram into zipline)
   *  6 = Lower platform & back up
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
