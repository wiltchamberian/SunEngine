/*****************************************************************************
* @brief : RenderPassBase
* 抽象了一次完整的渲染管线调用接口，
* 一次完整的渲染流程分为多个renderPass,每个renderPass分为多个shader,每个shader
* 下会对多个材质进行分类，最后每个材质下多个gameobject渲染
* @author : acedtang
* @date : 2021/4/14
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __RENDER_PASS_H
#define __RENDER_PASS_H

#include <vector>
#include <set>
#include <map>
#include "Util/Factory.h"
#include "Assets/AssetsMgr.h"
#include "Render/ShaderManager.h"

namespace Sun {

    class ShaderComponentBinding {
    public:
        ShaderComponentBinding(const EShaderType& type) :type_(type)
        {}
        EShaderType type_;
        std::set<ComponentType> coms_;
    };

    class RenderPass {
    public:
        //virtual void drawCall() = 0;
        //virtual void setPassCall() = 0;

    public:
        //THandle<Shader> shaderId_;
        //one system respond one shader 
        std::map<EShaderType,ShaderComponentBinding> bindings_;
    };


}


#endif