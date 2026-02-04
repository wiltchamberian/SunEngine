/*****************************************************************************
* @brief : glgraphicsProxy
*  µœ÷graphicsProxy
* @author : acedtang
* @date : 2021/6/5
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __GL_GRAPHICS_PROXY_H
#define __GL_GRAPHICS_PROXY_H

#include "GraphicsProxy.h"

namespace Sun {

	class GLGraphicsProxy : public GraphicsProxy
	{
	public:
		GBuffer recreateGBuffer(int width,int height) override;
		
		VertexBufferLayout buildStaticMeshVertexBufferLayout();
	};

}

#endif