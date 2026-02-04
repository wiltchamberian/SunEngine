#ifndef __SUN_RENDER_GRAPHICS_PIPELINE_MGR_H
#define __SUN_RENDER_GRAPHICS_PIPELINE_MGR_H

#include "RHI2Define.h"
#include <vector>
#include <map>
#include <string>

namespace Sun {

    using namespace rhi;

    class GraphicsPipelineMgr {
    public:
        void initEnvironment();
        void loadShaders();
        void loadPipeline();
    protected:
        std::vector<VertexInputState> layouts_;
        std::map<std::string, ShaderProgramCreateInfo> shaderMap_;
        std::map<std::string, RHIPtr<IPipeline>> piplines_;
    };

}

#endif 