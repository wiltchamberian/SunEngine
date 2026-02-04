/*****************************************************************************
* @brief : VertexData
* @author : acedtang
* @date : 2021/7/22
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __VERTEX_DATA_H
#define __VERTEX_DATA_H

#include "VertexBufferLayout.h"
#include <vector>

namespace Sun {
	//每个VertexData结构体对应一个VertexBufferObject,描述其数据和布局信息
	struct VertexData {
		void* vertice = nullptr;	
		int vertexCount = 0;
		int vertexSiz = 0;
		void* indice = nullptr;
		int indiceCount = 0;
		int indiceSiz = 0;
		EDrawMode usage;
		VertexBufferLayout layout;
	};

	//typedef std::vector<VertexData> VertexDataArray;
	struct VertexDataArray : public std::vector<VertexData>
	{
		//this is used by drawCall to derteminate the final draw vertexCount or indiceCount
		int vertexCount = 0;
		int indiceCount = 0;
	};

	struct VboEboCreateInfo {
		void* vertices = nullptr;
		int verticeCount = 0;
		int verticeSiz = 0;
		void* indices = nullptr;
		int indiceCount = 0;
		int indiceSiz = 0;
		EDrawMode drawMode;
	};

}




#endif