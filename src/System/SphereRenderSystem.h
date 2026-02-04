#ifndef __SPHERE_RENDER_PASS_COMPONENT_H
#define __SPHERE_RENDER_PASS_COMPONENT_H

#include "Component/MeshComponent.h"
#include "Component/SphereComponent.h"

namespace Sun {
    class SphereRenderSystem:public MeshComponent
    {
    public:
        void prepareRender();
        void render() override;

        std::vector<SphereComponent*> spheres_;
        RenderPassType passType_;
    };
}

#endif