#ifndef __RENDERABLE_COM_H
#define __RENDERABLE_COM_H

#include "SceneComponent.h"
#include "Render/Program.h"
#include "Component/MaterialCom.h"
#include "Component/TransformCom.h"
#include "Component/MeshInterface.h"
#include "Component/ComponentType.h"
#include "Component/LightCom.h"

namespace Sun {

    struct RenderAbleCreateInfo {
        ComType meshType;
        ComType materialType;
    };

	class RenderAbleCom : public SActorComponent
	{
	public:
        RenderAbleCom();
        RenderAbleCom(const RenderAbleCreateInfo& createInfo);
        RenderAbleCom(const std::shared_ptr<MeshInterface>& mesh, const std::shared_ptr<MaterialCom>& material);
        
        friend class MeshComponent;
        virtual void onAddedToLevel() override;
        virtual void tick(float dtime, const SEvent& ev)override;
        virtual void prepareRender();
        virtual void beginRender();
        virtual void render();
        virtual void endRender();

        GpuVertexDataHandle getVertexDataHandle();

        virtual uint32_t getInterfaceFlag() override;

        void setMeshInterface(const std::shared_ptr<MeshInterface>& mesh);
        inline std::shared_ptr<MeshInterface> getMeshInterface() const {
            return meshCom_;
        }
        template<class _T>
        std::shared_ptr<_T> getMeshCom() const { 
            return std::dynamic_pointer_cast<_T>(meshCom_); 
        } 

        void allocGpu(S_TYPE drawType);
        void releaseGpu();
        void setShaderProgram(const ShaderProgram& pro);

        virtual void bindUniformData_Transform(const Matrix4x4& view, const Matrix4x4& proj, const Matrix4x4& vp, const Vector3D& cameraPos);
        virtual void unBindUniformData_Transform();

        virtual void bindUniformData_Material();
        virtual void unBindUniformData_Material();

        virtual void bindUniformData();
        virtual void unBindUniformData();

        virtual void updateVertexData();

        //绑定顶点数据阶段 (不同组件的顶点格式不同)
        virtual void bindVertexData();
        virtual void unBindVertexData();
        virtual void drawCall();

        //for Multi shaderPass objPass ,if reimplemnt this function,
        //the reuturn vector should include shaderProgram_ and
        //shaderProgram_ must be the first;
        virtual std::vector<ShaderProgram> getMultShaderPass();
        virtual bool hasMultiShaderPass() const;

        ShaderProgram getShaderProgram() const;
        unsigned int getShaderProgramId() const;

        void setMaterialHandle(const std::shared_ptr<MaterialCom>& ptr);
        std::shared_ptr<MaterialCom> getMaterialHandle() const;

        inline void setVertexDataHandle(const GpuVertexDataHandle& handle) {
            if (meshCom_)
                meshCom_->setVertexDataHandle(handle);
        }

        inline friend bool operator< (const RenderAbleCom& m1, const RenderAbleCom& m2) {
            if (m1.getShaderProgram() < m2.getShaderProgram()) return true;
            else if (m1.getShaderProgram() > m2.getShaderProgram()) return false;
            if (*m1.getMeshInterface() < *m2.getMeshInterface()) return true;
            else if (*m2.getMeshInterface() < *m1.getMeshInterface()) return false;

            return false;
        }
    protected:
        void init(const std::shared_ptr<MeshInterface>& mesh, const std::shared_ptr<MaterialCom>& material);
        std::shared_ptr<MeshInterface> meshCom_;
        //std::shared_ptr<TransformCom> transformCom_;  //moved to GameObject
        std::shared_ptr<MaterialCom> materialCom_;
        std::shared_ptr<LightCom> lightCom_;
        ShaderProgram pro_;
        
	};
}

#endif