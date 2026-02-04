/*****************************************************************************
* @brief : Texture
* 纹理，包含纹理参数，路径 以及gpu句柄，但不包含内存数据块
* @author : acedtang
* @date : 2021/3/19
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __TEXTURE_H
#define __TEXTURE_H

#include <string>
#include <vector>
#include "Common/Macros.h"
#include "Util/Guid.h"
#include "Common/Common.h"
#include "Common/PixelFormat.h"
#include "Common/BasicDataType.h"

namespace Sun
{
    struct Texture {
        int handle_ = INVALID_HANDLE;
        std::string source_;
        ETextureType   textureType_;
        unsigned int width_;
        unsigned int height_;
        EPixelFormat pixelFormat_;

        //纹理采样方式
        int magFilter_;
        int minFilter_;
        int wrapS_;
        int wrapT_;
        
        //Guid guid;
        Texture()
            :textureType_(ETextureType::TEXTURE_2D)
        {
        }
        inline bool valid() {
            return (handle_ != -1) && (textureType_ != ETextureType::INVALID);
        }
    };



    //struct Texture2D: public Texture
    //{
    //    std::string _path;
    //    Texture2D()
    //    {
    //        //_glType = 0x0DE1;
    //    }
    //    Texture2D(const std::string path)
    //    {
    //        _path = path;
    //        //_glType = 0x0DE1;
    //    }
    //};

    struct TextureCube : public Texture
    {
        std::vector<std::string> _paths;
        TextureCube() {

        }
        TextureCube(const std::vector<std::string>& paths) {
            _paths = paths;
        }
    };

    
}


#endif