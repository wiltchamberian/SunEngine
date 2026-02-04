/*****************************************************************************
* @brief : platformMisc
* 平台杂类相关
* @author : acedtang
* @date : 2021/4/6
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#include "Util/Guid.h"

namespace Sun {

    Guid createGuid();

    std::string getGlobalPath();

    std::string getResourceDir();

    double getTime();

}