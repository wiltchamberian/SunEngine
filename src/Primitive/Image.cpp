#include "Image.h"
#include "Common/BasicDataType.h"

namespace Sun {

    Image::Image()
        :width_(0)
        ,height_(0)
        ,pixelFormat_(EPixelFormat::RGBA)
        ,type_(ETextureType::TEXTURE_2D)
    {

    }

}