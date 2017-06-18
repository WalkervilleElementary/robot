#ifndef WALKERVILLE_ROBOT_TEMPLATES_MODULE_H
#define WALKERVILLE_ROBOT_TEMPLATES_MODULE_H

#include "configs/configs.h"


namespace module 
{

class Module
{
public:
  bool loop();

#if USE_UPDATE()
  bool update();
#endif  // USE_UPDATE

};  // class module

}  // namespace module

#endif  // WALKERVILLE_ROBOT_TEMPLATES_MODULE_H