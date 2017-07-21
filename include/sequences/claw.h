#ifndef WALKERVILLE_ROBOT_SEQUENCES_CLAW_H
#define WALKERVILLE_ROBOT_SEQUENCES_CLAW_H

#include "configs.h"

#include "templates/sequence.h"

#include <stdint.h>

namespace sequences{
  static const int8_t LEFT_CLAW = 0;
  static const int8_t RIGHT_CLAW = 1;

class Claw : public templates::Sequence{

private:
  static unsigned long delay_;
  static int8_t state_;
  static uint16_t degree_;

  static unsigned long raise_pause;
  static unsigned long grab_pause;
  static unsigned long retrieve_pause;

  static uint16_t left_claw_extended;
  static uint16_t left_claw_rest;
  static uint16_t left_claw_vertical;
  static uint16_t left_open;
  static uint16_t left_close;

  static uint16_t right_claw_extended;
  static uint16_t right_claw_rest;
  static uint16_t right_claw_vertical;
  static uint16_t right_open;
  static uint16_t right_close;

public:
  inline Claw(){};
  inline ~Claw(){};

  static bool raise(int8_t side);
  static bool grab(int8_t side);
  static bool release(int8_t side);

  bool loop();

#if USE_UPDATE()
  bool update();
#endif  // USE_UPDATE()

};  // class Claw

}  // namespace sequnces

#endif  // WALKERVILLE_ROBOT_SEQUENCES_CLAW_H
