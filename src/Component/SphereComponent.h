/*****************************************************************************
* @brief : sphere
* @author : acedtang
* @date : 2021/3/19
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/
#ifndef __SPHERE_COMPONENT_H
#define __SPHERE_COMPONENT_H

#include "Primitive/SpherePrimitive.h"
#include "Component/MeshComponent.h"

namespace Sun {
    class SphereComponent : public MeshComponent
    {
    public:
        void prepareRender() override;
        void render() override;
        SpherePrimitive spherePrimitive;
    };
}

#endif