/*****************************************************************************
* @brief : ParticleComponent
* Á£×Ó×é¼þ
* @author : acedtang
* @date : 2021/5/23
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/
#ifndef __PARTICLE_COMPONENT_H
#define __PARTICLE_COMPONENT_H

#include "MeshComponent.h"
#include "Render/VertexArray.h"

namespace Sun {

    class ParticleComponent : public MeshComponent
    {
    public:
        void prepareRender() override;
        void beginRender() override;
        void render() override;
        void endRender() override;
        void tick(float dtime, const SEvent& ev) override;

        float attractor_masses_[64];
        unsigned int attractor_buffer_;
        unsigned int positionBuf_;
        unsigned int velBuf_;
        unsigned int vao_;
        unsigned tbo_[2];

        float dtime_ = 0;
    };

}

#endif