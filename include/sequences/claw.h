#ifndef WALKERVILLE_ROBOT_SEQUENCES_CLAW_H
#define WALKERVILLE_ROBOT_SEQUENCES_CLAW_H

#include "configs.h"

#include "templates/sequence.h"

namespace sequences
{

class Claw : public templates::Sequence
{
private:
  unsigned long start_time;

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
