/*****************************************************************************
* @brief : InstancedMeshCom
* 实例化网格组件，主要用于实例化渲染
* @author : acedtang
* @date : 2021/7/22
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __INSTANCED_MESH_COM_H
#define __INSTANCED_MESH_COM_H

#include "Component/MeshComponent.h"

namespace Sun {

	class InstancedMeshCom : public MeshComponent
	{
	public:
		~InstancedMeshCom();
		void releaseGpu() override;
		void allocGpu(EDrawMode type) override;
		void bindUniformData(const ShaderProgram& pro) override;
		void unBindUniformData(const ShaderProgram& pro) override;

		//只分配实例化数组对应的buffer
		void allocInstanceGpu(EDrawMode type);
	public:
		std::vector<InstancedVertex> instancedVertices_;
		VboEboHandle transformsHandle_;
	};

	//for test
	class SimpleInstancedMeshCom : public MeshComponent {
	public:
		~SimpleInstancedMeshCom();
		void releaseGpu() override;
		void allocGpu(EDrawMode type) override;
	public:
		std::vector<SimpleInstancedVertex> instancedVertices_;
	};
}


#endif