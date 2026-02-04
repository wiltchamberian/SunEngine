#include "PlatformIO.h"
#include "GlfwIO.h"
#include "WinIO.h"

namespace Sun
{
    PlatformIO* g_platformIO = nullptr;

    // settings
    const unsigned int SCR_WIDTH = 1600;
    const unsigned int SCR_HEIGHT = 900;

    void PlatformIO::registViewPortGeometryGetter(std::function<Rect()>&& func) {
        getViewportGeometry_ = func;
    }

    void PlatformIO::registFrameBufferSizeGetter(std::function<Extend()>&& func) {
        frameBufferSizeGetter_ = func;
    }

    PlatformIO::~PlatformIO()
    {

    }

    PlatformIO* PlatformIO::createPlatformIO()
    {
        //for use glfw , no need to 
        return new GlfwIO();

#ifdef _WIN32
        PlatformIO* io = new SWinIO();
        return io;

#endif
        return nullptr;
    }

    bool PlatformIO::windowShouldClose() {
        return false;
    }

    void PlatformIO::terminate() {
    }

    Rect PlatformIO::getViewportGeometry() {
        return getViewportGeometry_();
    }

    Extend PlatformIO::getFrameBufferSize() {
        return frameBufferSizeGetter_();
    }

    void PlatformIO::swapBuffer() {
    }

    SEvent PlatformIO::getEvent() {
        SEvent ev;

        if (!g_evQue.empty())
        {
            SEvent ev = g_evQue.front();
            g_evQue.pop();
            return ev;
        }
        return ev;
    }

    void PlatformIO::generateEvents() {
    }

    Sun::SUserCmd PlatformIO::generateCmd()
    {
        while (!g_evQue.empty()) {
            SEvent ev = g_evQue.front();
            g_evQue.pop();
        }
        return SUserCmd();
    }

    bool PlatformIO::initEnvironment(const RunStruct& stru) {
        return true;
    }

}


