#ifndef __STATIC_MESH_RASTER_LIGHTING_SYSTEM_H
#define __STATIC_MESH_RASTER_LIGHTING_SYSTEM_H

#include "Math/Vector3D.h"

namespace Sun {
	class SWorld;
	class StaticMeshRasterDefferedLightingRenderSystem
	{
	public:
		StaticMeshRasterDefferedLightingRenderSystem(SWorld* world);
		//will be called on game start
		virtual void prepareRender();

		//will be called every frame before all render
		virtual void beginRender() ;

		//will be called every frame between beginRender and endRender
		virtual void render();

		//will be called every frame after render
		virtual void endRender();

	protected:
		void openglBeginRender();
		void openglrender();
		void vulkanBeginRender();
		unsigned int quadVAO_ = 0;
		unsigned int quadVBO_ = 0;
		vec3 viewPos_;

		SWorld* world_ = nullptr;
	};

}

#endif