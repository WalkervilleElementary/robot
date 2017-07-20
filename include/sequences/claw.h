#ifndef WALKERVILLE_ROBOT_SEQUENCES_CLAW_H
#define WALKERVILLE_ROBOT_SEQUENCES_CLAW_H

#include "configs.h"

#include "templates/sequence.h"

namespace sequences
{
  const int LEFT_CLAW = 0;
  const int RIGHT_CLAW = 1;

class Claw : public templates::Sequence
{
private:
  unsigned long delay_;
  int state_;
  int degree_;

  int raise_pause;
  int grab_pause;
  int retrieve_pause;

  int left_claw_extended;
  int left_claw_rest;
  int left_claw_vertical;
  int left_open;
  int left_close;
  // Right values have not been tested
  int right_claw_extended;
  int right_claw_rest;
  int right_claw_vertical;
  int right_open;
  int right_close;

#if USE_UPDATE()
  char update_state_;
  int start_val;
  int end_val;
  int change;
#endif  // USE_UPDATE()

public:
  Claw();
  inline ~Claw(){};

  bool raise(int side);
  bool grab(int side);
  bool release(int side);

  bool loop();

#if USE_UPDATE()
  bool update();
#endif  // USE_UPDATE()

};  // class Claw

}  // namespace sequnces

#endif  // WALKERVILLE_ROBOT_SEQUENCES_CLAW_H
