#ifndef WALKERVILLE_ROBOT_SEQUENCES_TAPE_H
#define WALKERVILLE_ROBOT_SEQUENCES_TAPE_H

#include "configs/configs.h"
#include "templates/sequence.h"
#include "hardware/driver.h"
#include "hardware/ir.h"

namespace sequences
{

class Tape : public templates::Sequence
{
private:
#pragma region  // variables
  // computeCommand
  int gain_t_;
  int gain_p_;
  int gain_i_;
  int gain_d_;

  int i_error_;
  int prev_error_;

  int kp_;
  int ki_;
  int kd_;

  // general
  int error_;
  int command_;
  int velocity_; 

#if USE_UPDATE()
  char state_;
#endif  // USE_UPDATE()

  hardware::Ir ir_;
  hardware::Driver motor_;
#pragma endregion  // pragma region variables

  int computeCommand(int error, int dt);

public:
  Tape();
  inline ~Tape(){};
  void setup(hardware::Ir ir, hardware::Driver motor);
  bool loop();
#if USE_UPDATE()
  bool update();
#endif  // macro USE_UPDATE()
};  // class tape

}  // namespace tape

#endif  // WALKERVILLE_ROBOT_SEQUENCES_TAPE_H