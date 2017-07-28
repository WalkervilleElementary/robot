#pragma once
#include <stdint.h>

namespace hardware {

class EncoderMotor {
public:
  EncoderMotor(DCMotor& motor, Encoder& Encoder);
  
}

}