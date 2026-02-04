/*****************************************************************************
* @brief : RenderCommandBuffer
* 渲染指令buffer
* @author : acedtang
* @date : 2021/5/12
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __RENDER_COMMAND_BUFFER_H
#define __RENDER_COMMAND_BUFFER_H

#include <vector>
#include "Render/RenderCommand.h"

namespace Sun {

    class RenderCommandBuffer {
    public:
        RenderCommandBuffer();
        ~RenderCommandBuffer();
            
        //目前只提供两个简单接口,导入指令,
        //是否要对数据重新排序，以及组织功能后续可考虑加入
        void push_back(const RenderCommandHead* head);
        
        //提交所有指令至gpu
        void submitAll();

    protected:
        void submitStaticMesh(RenderCommandHead* head);
        
        void submitSkeletalMesh(RenderCommandHead* head);

        std::vector<char> vec_;
    };

}

#endif