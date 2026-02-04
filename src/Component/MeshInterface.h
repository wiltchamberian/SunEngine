#ifndef __MESH_INTERFACE_H
#define __MESH_INTERFACE_H

#include "Common/Macros.h"
#include "Common/Common.h"
#include "Common/BasicDataType.h"
#include "Render/Program.h"
#include "Component/IComponent.h"

namespace Sun {
	class RenderAbleCom;
	class MeshInterface : public IComponent {
	public:
		virtual ~MeshInterface() {}
		virtual void allocGpu(EDrawMode drawTYpe) = 0;
		virtual void releaseGpu() = 0;
		virtual void render();
		virtual void tick(float dtime, const SEvent& ev);
		virtual void onAddedToLevel(RenderAbleCom* com);
		bool isAlloced() {
			return gpuVertexDataHandle_.isValid();
		}
		
		void setVertexDataHandle(const GpuVertexDataHandle& handle);
		GpuVertexDataHandle getVertexDataHandle() const;

		virtual void bindUniformData(const ShaderProgram& pro);
		virtual void unBindUniformData(const ShaderProgram& pro);
		virtual void updateVertexData();
		virtual void bindVertexData();
		virtual void unBindVertexData();
		void drawCall();
		
		inline friend bool operator < (const MeshInterface& m1, const MeshInterface& m2) {
			return m1.gpuVertexDataHandle_ < m2.gpuVertexDataHandle_;
		}
		EPrimitiveType triangleType_ = EPrimitiveType::TRIANGLES;
	protected:
		//gpu¶¥µãÊý¾Ý¾ä±ú
		GpuVertexDataHandle gpuVertexDataHandle_;
		
	};

}


#endif