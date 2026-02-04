/*****************************************************************************
* @brief : IMAGE
* @author : acedtang
* @date : 2021/5/23
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __IMAGE_H
#define __IMAGE_H

#include <vector>
#include <string>
#include "Util/Guid.h"
#include "Common/Common.h"
#include "Common/Macros.h"
#include "Common/PixelFormat.h"
#include "Common/BasicDataType.h"

namespace Sun {

    class Image {
    public:
        Image();
        int width_;
        int height_;
        EPixelFormat pixelFormat_;
        ETextureType type_;
        //纹理采样方式
        int magFilter_;
        int minFilter_;
        int wrapS_;
        int wrapT_;
        std::vector<unsigned char> data_;
        std::string source_;
        Guid  guid_;
    };

}

#endif