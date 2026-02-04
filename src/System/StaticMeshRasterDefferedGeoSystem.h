/*****************************************************************************
* @brief : 静态网格光栅化延时着色渲染系统
* @author : acedtang
* @date : 2021/3/20
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __STATIC_MESH_RASTER_DEFFERED_GEO_SYSTEM_H
#define __STATIC_MESH_RASTER_DEFFERED_GEO_SYSTEM_H

#include "Render/RenderHandle.h"
#include "Render/GraphicsProxy.h"
namespace Sun {

	class StaticMeshRasterDefferedGeoRenderSystem
	{
	public:
		//will be called on game start
		virtual void prepareRender();

		//will be called every frame before all render
		virtual void beginRender();

		//will be called every frame between beginRender and endRender
		virtual void render();

		//will be called every frame after render
		virtual void endRender();

	protected:
		GBuffer gbuf_;
	};

}


#endif