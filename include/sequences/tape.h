#ifndef WALKERVILLE_ROBOT_SEQUENCES_TAPE_H
#define WALKERVILLE_ROBOT_SEQUENCES_TAPE_H

#include "configs.h"

#include "templates/sequence.h"
#include "hardware/driver.h"
#include "hardware/qrd.h"

#include <stdint.h>

namespace sequences{

class Tape : public templates::Sequence{

private:
  static int gain_t_;
  static int gain_p_;
  static int gain_i_;
  static int gain_d_;

  static int8_t i_error_;
  static int8_t prev_error_;

  static int kp_;
  static int ki_;
  static int kd_;

  // general
  static int error_;
  static int command_;
  static int velocity_;

  const hardware::Qrd& qrd_;
  const hardware::Driver& motor_;

  static int computeCommand(int8_t error, unsigned long dt);

public:
  inline Tape(const hardware::Qrd& qrd, const hardware::Driver& motor): qrd_(qrd), motor_(motor){};
  inline ~Tape(){};
  void stop();
  bool loop();
#if USE_UPDATE()
  void update();
#endif  // USE_UPDATE()
};  // class tape

}  // namespace tape

#endif  // WALKERVILLE_ROBOT_SEQUENCES_TAPE_H
