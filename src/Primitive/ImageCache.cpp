#include "ImageCache.h"
#define STB_IMAGE_IMPLEMENTATION
#include "ThirdParty/stb_image.h"
#include "Primitive/Texture.h"
#include "Common/Define.h"

namespace Sun {

    ImageCache* g_imageCache_ = new ImageCache;

    SharedPtr<Image> ImageCache::loadImage(const char* path) {
        int width, height, nrChannels;
        std::string str(path);
        auto it = imageMap_.find(str);
        if (it == imageMap_.end()) {
            unsigned char* data = stbi_load(path, &width, &height, &nrChannels, STBI_rgb_alpha);
            if (data == nullptr) {
                assert(false);
                return nullptr;
            }
            //assert(nrChannels == 4);
            nrChannels = 4;
            SharedPtr<Image> ptr(new Image());
            ptr->pixelFormat_ = EPixelFormat::RGBA;
            ptr->width_ = width;
            ptr->height_ = height;
            ptr->type_ = ETextureType::TEXTURE_2D;
            ptr->data_.resize(width * height * nrChannels);
            memcpy(ptr->data_.data(), data, ptr->data_.size());
            imageMap_.insert(std::make_pair(str, ptr));
            stbi_image_free(data); //can optimize?TODO
            return ptr;
        }
        else {
            return it->second;
        }
        return nullptr;
    }
}