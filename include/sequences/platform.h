#pragma once

#include "configs.h"

#include "hardware/driver.h"
#include "templates/sequence.h"

namespace sequences
{

class Platform : public templates::Sequence
{
private:
/**
 * States:
 *  0 = stopped
 *  1 = raising platform
 *  2 = lower slightly so limit switch is no longer active
 *  3 = lower slowly while backing up
 */
  int state_;
  int raise_speed_;
  int lower_speed_;
  int backup_speed_;
#if USE_UPDATE()
  int update_state_;
#endif  // USE_UPDATE()

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
