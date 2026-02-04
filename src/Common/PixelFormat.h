/*****************************************************************************
* @brief : PixelFormat
* ÏñËØ¸ñÊ½
* @author : acedtang
* @date : 2021/8/9
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __PIXEL_FORMAT_H
#define __PIXEL_FORMAT_H

#include <cassert>

namespace Sun {

	enum class EPixelFormat {
		RGB,                           
	    RGBA,
        RGBA32F,
        RGB32F,
        RGBA16F,
        RGB16F,

		DEPTH24_STENCIL8,

		STENCIL_INDEX,
		DEPTH_COMPONENT,
		DEPTH_STENCIL
	};

	extern int getChannelNumFromPixelFormat(EPixelFormat pixelFormat);
}


#endif