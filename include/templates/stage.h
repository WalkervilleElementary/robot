#ifndef WALKERVILLE_ROBOT_TEMPLATES_STAGE_H
#define WALKERVILLE_ROBOT_TEMPLATES_STAGE_H

#include "configs.h"


namespace templates
{

class Stage
{
public:
  bool loop();

#if USE_UPDATE()
  bool update();
#endif  // USE_UPDATE()

};  // class Stage

}  // namespace templates

#endif  // WALKERVILLE_ROBOT_TEMPLATES_STAGE_H