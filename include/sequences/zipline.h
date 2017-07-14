#pragma once

#include "configs.h"

#include "templates/sequence.h"

namespace sequences
{

class Zipline : public templates::Sequence
{
private:
    enum zipline_state { STOP, RAISE, LOWER };
    zipline_state state;

public:
  Zipline();
  inline ~Zipline(){};
  void setup();
  void stop();
  bool loop();
  bool raise();
  bool lower();
#if USE_UPDATE()
  void update();
#endif  // USE_UPDATE()
};  // class tape

}  // namespace sequences
