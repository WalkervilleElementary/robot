#ifndef WALKERVILLE_ROBOT_HARDWARE_ENCODER_H
#define WALKERVILLE_ROBOT_HARDWARE_ENCODER_H

#include "configs.h"

#include <stdint.h>

namespace hardware{

  static const int R_ENCODER_ = R_ENCODER();
  static const int L_ENCODER_ = L_ENCODER();

  extern volatile uint32_t counts[4];
  extern volatile unsigned long prevTime[4]; // defined in arduino library

class Encoder{

private:
  static void enableExternalInterrupt(unsigned int INTX, unsigned int mode);
  static void disableExternalInterrupt(unsigned int INTX);
  static void start(unsigned int INTX);
  static uint32_t stop(unsigned int INTX);

public:
  Encoder();
  inline ~Encoder(){};

  static uint32_t get(unsigned int INTX);

  void loop();
};  // class Encoder

}  // namespace hardware

#endif  // WALKERVILLE_ROBOT_HARDWARE_ENCODER_H
