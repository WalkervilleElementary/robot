#ifndef WALKERVILLE_ROBOT_TEMPLATES_STAGES_H
#define WALKERVILLE_ROBOT_TEMPLATES_STAGES_H

#include "configs/configs.h"


namespace templates 
{

class Stages
{
public:
  bool loop();
#if USE_UPDATE()
  bool update();
#endif  // USE_UPDATE

};  // class Stages

}  // namespace templates

#endif  // WALKERVILLE_ROBOT_TEMPLATES_STAGES_H