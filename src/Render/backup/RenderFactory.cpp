#include "RenderFactory.h"
#include "Render/Render.h"
#include "Render/GLRasterRender.h"
#include "PCH.h"

namespace Sun {

    void RenderFactory::createRender() {
        if (g_render != nullptr) return;
#ifdef OPENGL
        g_render = new SGLRasterRender();
#endif
    }


}