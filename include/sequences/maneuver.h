#ifndef WALKERVILLE_ROBOT_SEQUENCES_MANEUVER_H
#define WALKERVILLE_ROBOT_SEQUENCES_MANEUVER_H

#include "configs.h"
#include "templates/sequence.h"

#include "hardware/driver.h"
#include "hardware/encoder.h"


namespace sequences
{

class Maneuver : public templates::Sequence
{
private:
  int state_;
  unsigned int right_limit_;
  unsigned int left_limit_;

  float distance_to_encoder_;
  float degree_to_distance_;
  int gain_;

  hardware::Driver motor_;
  hardware::Encoder encoder_;

public:
  Maneuver();
  inline ~Maneuver(){};
  void setup(const hardware::Driver &motor, const hardware::Encoder &encoder);

  bool straight(int distance);
  bool turn(int degrees);

  bool loop();
#if USE_UPDATE()
  void update();
#endif  // USE_UPDATE()
};  // class Maneuver
}  // namespace sequences

#endif  // WALKERVILLE_ROBOT_SEQUENCES_MANEUVER_H

