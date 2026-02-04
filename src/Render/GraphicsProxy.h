/*****************************************************************************
* @brief : graphicsProxy
* 对于将图形库单独封装为API的方式，考虑到不同的图形库接口差异过大，
* 实际上很难封装为统一形式，这里采用一种折中方法，对于任何类需要调用图形库接口
* 将这些和图形库独立的调用逻辑封装到代理类中，代理类不支有状态，只是函数接口
* 这样当需要切换图形库时，只需切换代理就好了，代理函数可以符合一段图形调用逻辑
* 而不用与图形库API保持一一映射关系，降低了独立开发压力
* @author : acedtang
* @date : 2021/6/5
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/


#ifndef __GRAPHICS_PROXY_H
#define __GRAPHICS_PROXY_H

#include <memory>
#include "Program.h"
#include "Render/RenderHandle.h"
#include "Component/MaterialCom.h"
#include "Common/Macros.h"
#include "Common/BasicDataType.h"

namespace Sun {

	struct GBuffer {
		unsigned int gBuffer = -1;
		unsigned int gPosition = -1;
		unsigned int gNormal = -1;
		unsigned int gAlbedo = -1;
		unsigned int gMRA = -1;
		unsigned int rboDepth = -1;//renderbuffer
	};

	class GraphicsProxy {
	public:
		virtual RenderHandle commitStaticMeshDataToGPU(void* vertice, long long verticByteLen, void* indice, long long  indiceByteSiz, const ShaderProgram& pro , EDrawMode usage = EDrawMode::STATIC_DRAW) = 0;
		virtual RenderHandle commitStaticMeshInstanceDataToGPU(void* vertice, long long verticByteLen, void* indice, long long  indiceByteSiz, const ShaderProgram& pro
			, void* instanceData, long long instanceByteLen) = 0;

		virtual GBuffer recreateGBuffer(int width, int height) = 0;
		
	public:
		GBuffer gbuf_;
	};

	extern GraphicsProxy* g_graphicsProxy;
}

#endif 