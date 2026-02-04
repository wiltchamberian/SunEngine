#include "RenderCommandBuffer.h"

namespace Sun {

    //目前只提供两个简单接口,导入指令
    void RenderCommandBuffer::push_back(const RenderCommandHead* head) {
        char* d = (char*)head;
        for (int i = 0; i < head->len; ++i) {
            vec_.push_back(*(d + i));
        }
    }

    //提交所有指令至gpu
    void RenderCommandBuffer::submitAll() {
        if (vec_.empty()) return;

        char* d = &vec_[0];
        int position = 0;
        RenderCommandHead* head = (RenderCommandHead*)(d);
        int num = 0;
        while (position < vec_.size()) {
            switch (head->type) {
            case RenderCommandType::COMMAND_STATIC_MESH: 
            {
                submitStaticMesh(head);
            }
            break;
            case RenderCommandType::COMMAND_SKELETAL_MESH:
            {
                submitSkeletalMesh(head);
            }
            break;
            default:
                break;
            }

            d += head->len;
            position += head->len;
            head = (RenderCommandHead*)(d);
            num += 1;
        }
    }

    void RenderCommandBuffer::submitStaticMesh(RenderCommandHead* head) {
        StaticMeshRenderData* d = (StaticMeshRenderData*)head;

        
    }

    void RenderCommandBuffer::submitSkeletalMesh(RenderCommandHead* head) {
        SkeletalMeshRenderData* d = (SkeletalMeshRenderData*)head;


    }


}