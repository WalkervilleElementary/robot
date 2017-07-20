#ifndef WALKERVILLE_ROBOT_HARDWARE_ENCODER_H
#define WALKERVILLE_ROBOT_HARDWARE_ENCODER_H

#include "configs.h"

namespace hardware
{
  const int R_ENCODER_ = R_ENCODER();
  const int L_ENCODER_ = L_ENCODER();

  extern volatile unsigned int counts[4];
  extern volatile unsigned long prevTime[4];

class Encoder
{
private:
  void enableExternalInterrupt(unsigned int INTX, unsigned int mode);
  void disableExternalInterrupt(unsigned int INTX);
  void start(unsigned int INTX);
  unsigned int stop(unsigned int INTX);

public:
  Encoder();
  inline ~Encoder(){};

  unsigned int get(unsigned int INTX);

  void loop();
};  // class Encoder
}  // namespace hardware

#endif  // WALKERVILLE_ROBOT_HARDWARE_ENCODER_H
