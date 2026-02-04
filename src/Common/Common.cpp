#include "Common.h"

namespace Sun {

    bool g_is_server = false;

    bool g_is_journal = false;

    GRAPHICS_API_TYPE globalGetGraphics_API_Type() {
        return GRAPHICS_API_TYPE::G_OPENGL;
    }
}


