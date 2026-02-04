/*****************************************************************************
* @brief : PointLight
* 点光源组件
* @author : acedtang
* @date : 2021/3/19
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/
#ifndef __POINTLIGHT_H
#define __POINTLIGHT_H

#include "Math/Vector3D.h"
#include "Entity/Light.h"

namespace Sun {
    class RenderAbleCom;
    class PointLight : public Light
    {
    public:
        void onAddedToLevel() override;
        void tick(float dtime, const SEvent& ev) override;
        void bindUniformData() override;
        vec3 color_;
        int uniformId_ = 0;

    };
}

#endif