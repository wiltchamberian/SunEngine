/*****************************************************************************
* @brief : RenderAPI
* 本身代码除了依赖RHI的部分，本身是平台和设备无关的
* 主要能力为提供一些直接绘制基础图元的能力
* @author : acedtang
* @date : 2021/9/4
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __RENDER_API_H
#define __RENDER_API_H

#include "Common/Common.h"

namespace Sun {

	namespace RenderAPI {
		//按照NDC绘制一个全屏的quad
		void cmdQuad(GpuVertexDataHandle& handle);

		//绘制天空盒立方体
		void cmdSkyCube(GpuVertexDataHandle& handle);
	};

}

#endif