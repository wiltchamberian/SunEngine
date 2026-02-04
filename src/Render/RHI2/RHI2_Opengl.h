#ifndef __SUN_RHI2_OPENGL_H
#define __SUN_RHI2_OPENGL_H

#include "RHI2.h"

namespace Sun {
    namespace rhi {

        class IOpenglPipeline : public IPipeline {
        
        public:
            uint32_t handle_ = -1;
            //将各种管线配置信息打包到一个uint64
            uint64_t stateBits;
        };

        class OpenglRHI :public RHI2 {
        public:
            RHIPtr<IContext> createContext(const vk::InstanceCreateInfo& ci) override;
        };
    }
}

#endif