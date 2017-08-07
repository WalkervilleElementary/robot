#ifndef WALKERVILLE_ROBOT_SEQUENCES_CLAW_H
#define WALKERVILLE_ROBOT_SEQUENCES_CLAW_H

#include "configs.h"

#include "templates/sequence.h"

#include <stdint.h>

namespace sequences{
  static const int8_t LEFT_CLAW = 0;
  static const int8_t RIGHT_CLAW = 1;

class Claw : public templates::Sequence {

private:
  enum ClawSequence { NONE = 0, RAISE, GRAB, RELEASE };
  static ClawSequence currentSequence_;
  static unsigned long delayUntil_;
  static uint8_t grabOffset_;
  static int8_t state_;
  static bool left_;

  static unsigned long raise_pause;
  static unsigned long grab_pause;
  static unsigned long retrieve_pause;
  static unsigned long fold_delay;

  static int16_t left_claw_extended;
  static int16_t left_claw_folded;
  static int16_t left_claw_drop;
  static int16_t left_claw_vertical;
  static int16_t left_open;
  static int16_t left_close;
  static int16_t left_offset[];

  static int16_t open_offset;

  static int16_t right_claw_extended;
  static int16_t right_claw_folded;
  static int16_t right_claw_drop;
  static int16_t right_claw_vertical;
  static int16_t right_open;
  static int16_t right_close;
  static int16_t right_offset[];

  static const int8_t EXTENDED = 0;
  static const int8_t FOLDED = 1;
  static const int8_t VERTICAL = 2;
  static const int8_t OPEN = 3;
  static const int8_t CLOSED = 4;

  /**
   * Positions:
   *  0 = extended
   *  1 = folded
   *  2 = vertical
   *  3 = mostly open
   *  4 = close
   */
  static void set_arm_position(int8_t side, int8_t position);

public:
  inline Claw(){};
  inline ~Claw(){};

  static bool raise(int8_t side);
  static bool grab(int8_t side, int8_t offset = 1);
  static bool release(int8_t side);
  static void fold(bool left_surface);

  bool loop();

private:

};  // class Claw

}  // namespace sequnces

#endif  // WALKERVILLE_ROBOT_SEQUENCES_CLAW_H
