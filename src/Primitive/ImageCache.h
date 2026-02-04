/*****************************************************************************
* @brief : TextureCache.h
* 纹理缓存 ，在内存中
* @author : acedtang
* @date : 2021/5/23
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/
#ifndef __IMAGE_CACHE_H
#define __IMAGE_CACHE_H

#include <unordered_map>
#include <memory>
#include "Primitive/Image.h"

namespace Sun {

    class ImageCache {
    public:
        std::shared_ptr<Image> loadImage(const char* path);

    public:
        std::unordered_map<std::string, std::shared_ptr<Image>> imageMap_;
    };

    extern ImageCache* g_imageCache_;
}

#endif