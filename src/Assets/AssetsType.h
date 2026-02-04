/*****************************************************************************
* @brief : AssetsType
* @author : acedtang
* @date : 2021/4/7
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __ASSETS_TYPE_H
#define __ASSETS_TYPE_H

#include "Util/Define.h"

namespace Sun {

#define ASSET_TEXTURE  0 
#define ASSET_MATERIAL  1
#define ASSET_SKELETON 2
#define ASSET_ANIMATION 3
#define ASSET_SKELETALMESH 4
#define ASSET_STATICMESH 5

    using AssetType = uchar;

}

#endif