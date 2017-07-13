#ifndef WALKERVILLE_ROBOT_TEMPLATES_SQUENCE_H
#define WALKERVILLE_ROBOT_TEMPLATES_SQUENCE_H

#include "configs.h"


namespace templates
{

class Sequence
{
public:
  bool loop();
#if USE_UPDATE()
  void update();
#endif  // USE_UPDATE();
};  // class Sequence

}  // namespace templates

#endif  // WALKERVILLE_ROBOT_TEMPLATES_SQUENCE_H
