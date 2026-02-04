/*****************************************************************************
* @brief : RenderInputStruct
* 渲染输入结构体
* 调用RHI时，输入参数通常较多，将这些参数封装为结构体，则便于修改，不用每次修改接口
* 命名规则默认为对应api的名字首字母大写
* @author : acedtang
* @date : 2021/8/9
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#include "Common/BasicDataType.h"
#include "Common/PixelFormat.h"

namespace Sun {

	struct TexImage2D {
		//句柄
		unsigned int handle;
		//图像数据
		unsigned char* data;
		//图像宽度
		int width;
		//图像高度
		int height;
		//像素数据数据类型
		EBasicDataType pixelDataType;
		//输入像素格式
		EPixelFormat inputPixelFormat;
		//内部像素格式
		EPixelFormat internalPixelFormat;
		//纹理环绕方式
		ETextureWrapper wrapperMode;
		//纹理过滤方式
		ETextureMinMagFilter filter;
	};

	struct FrameBufferTexture2D {
		EFrameBufferType target;
		EAttachmentType attachment;
		ETextureType textarget;
		uint hTex;
		uint level;
	};

}