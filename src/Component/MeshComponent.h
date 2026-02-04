/*****************************************************************************
* @brief : MeshCom
* 网格组件
* @author : acedtang
* @date : 2021/3/19
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/
#ifndef __MESH_COMPONENT_H
#define __MESH_COMPONENT_H

#include "Math/Matrix4x4.h"
#include "Primitive/StaticMeshPrimitive.h"
#include "Common/Common.h"
#include "Common/BasicDataType.h"
#include "MeshInterface.h"
#include "Util/QuickVector.h"

namespace Sun {

    //class SubMesh {
    //public:
    //    //子网格索引可以共享一组顶点数据
    //    std::shared_ptr<std::vector<Vertex>> vertices_;
    //    std::vector<int> indices_;
    //};

    //class Mesh {
    //public:
    //    friend class MeshComponent;
    //    Mesh() {
    //        submeshes_.resize(1);
    //    }
    //    
    //    //std::vector<int> indices_;

    //    FORCE_INLINE uint32 getSubMeshCount() const {
    //        return submeshes_.size();
    //    }
    //protected:
    //    //MUST保证Vector的实现在重分配时内部进行移动拷贝以保证效率
    //    Vector<SubMesh> submeshes_;
    //};

    class MeshComponent : public MeshInterface
    {
    public:
        virtual ~MeshComponent() override;
        friend class StaticMeshRasterRenderPass;
        virtual void prepareRender();
        virtual void beginRender();
        virtual void endRender();
        void tick(float dtime, const SEvent& ev) override;

        void allocGpu(EDrawMode drawType) override;
        void releaseGpu() override;

        //光栅化渲染管线--end

        //for Multi shaderPass objPass ,if reimplemnt this function,
        //the reuturn vector should include shaderProgram_ and
        //shaderProgram_ must be the first;
        virtual std::vector<ShaderProgram> getMultShaderPass();
        virtual bool hasMultiShaderPass() const;

        void setVertices(const std::vector<Vertex>& vertices);
        void setVertices(std::vector<Vertex>&& vertices);
        void setIndices(const std::vector<int>& indices);
        void setIndices(std::vector<int>&& indices);
        std::vector<Vertex>& getVertices();
        std::vector<int>& getIndices();
        const Vertex* getVerticesData() const;
        size_t getVerticesSize() const;
        const int* getIndicesData() const;
        size_t getIndicesSize() const;
        inline EPrimitiveType getPrimitiveType() {
            return triangleType_;
        }

    protected:
        std::vector<Vertex> vertices_;
        std::vector<int> indices_;
    };

}

#endif