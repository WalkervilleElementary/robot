#pragma once

#include "configs.h"

#include "hardware/driver.h"
#include "templates/sequence.h"

namespace sequences
{

class Platform : public templates::Sequence
{
private:
  int state_;  // 0 = stopped, 1 = raising, 2 = lowering
  int raise_speed_;
  int lower_speed_;
  int backup_speed_;

  hardware::Driver driver_;

public:
  Platform();
  inline ~Platform(){};
  void setup(const hardware::Driver &driver);
  bool loop();
  void stop();
  void raise();
  void lower();
#if USE_UPDATE()
  void update();
#endif  // USE_UPDATE()
};  // class tape

}  // namespace sequences
