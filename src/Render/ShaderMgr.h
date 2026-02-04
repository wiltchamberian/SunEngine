#ifndef __SUN_RHI_SHADER_BUILDER_H
#define __SUN_RHI_SHADER_BUILDER_H

#include "ShaderModule.h"
#include "Util/ResourceMgr.h"
#include "Util/VectorWrapper.h"
#include "RHIDefine.h"
#include <string>

namespace Sun {
namespace rhi {

    class ShaderMgr 
    {
    public:
        ShaderMgr();
        virtual ~ShaderMgr() {}
        virtual void createShaderProgram() = 0;

        //设置shaders所在目录,计算时会用该目录+ShaderCreateInfo的name作为shader路径
        //example: dir="C:/doc/"   shaderCreateInfo.name = "abc.glsl" ,the path will be"C:/doc/abc.glsl"
        void setShaderProgramCreateInfo(const RHIShaderProgramCreateInfo& info) {
            ci_ = info;
        }

        std::vector<HShaderModule>& getShaderModules() {
            return shaderModules_;
        }
    protected:
        //input
        RHIShaderProgramCreateInfo ci_;
        //output
        std::vector<HShaderModule> shaderModules_;
    };

}

}

#endif