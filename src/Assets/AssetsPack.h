/*****************************************************************************
* @brief : AssetsPack
* 资产打包后的数据结构，2进制
* @author : acedtang
* @date : 2021/4/2
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __ASSETS_PACK_H
#define __ASSETS_PACK_H

#include "Util/Define.h"
#include "Util/Guid.h"

namespace Sun {

    struct AssetsPackHead
    {       
        char c1, c2, c3, c4;
        uint32 numAssets;
        uint32 backup[4];
        AssetsPackHead()
            :c1('S')
            ,c2('U')
            ,c3('N')
            ,c4(0)
        {

        }
    };

    struct AssetHead {
        uint32 assetType;
        uint32 aligent; //for 8byte aligent
        Guid   guid; //4*4 byte
        uint32 backup[2];
        AssetHead():
            assetType(0)
        {

        }
    };
}

#endif