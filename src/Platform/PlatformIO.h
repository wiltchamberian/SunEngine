/*****************************************************************************
* @brief : PlatformIO
* 对平台窗口系统的抽象，实例化为不同的窗口体系，如Qt,glfw,Win32等
* @author : acedtang
* @date : 2021/7/25
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __PLATFORMIO_H
#define __PLATFORMIO_H

#include "Util/Define.h"
#include "Input.h"
#include "Common/Common.h"
#include <functional>

#include <queue>

class GLFWwindow;
namespace Sun
{

    class PlatformIO
    {
    public:
        virtual ~PlatformIO();
        static PlatformIO* createPlatformIO();

        virtual bool windowShouldClose();
        void terminate();
        virtual SEvent getEventFromSys() { return SEvent(); }

        virtual void inFrame() {};
        virtual bool initEnvironment(const RunStruct& stru);
        virtual SEvent getEvent();
        virtual void generateEvents();
        virtual SUserCmd generateCmd();
        virtual Rect getViewportGeometry();
        virtual void swapBuffer();
        virtual Extend getFrameBufferSize();

        void vmCall(void* info, void* extraInfo) {}

        void registViewPortGeometryGetter(std::function<Rect()>&& func);
        void registFrameBufferSizeGetter(std::function<Extend()>&& func);
    protected:
        std::function<Rect()> getViewportGeometry_;
        std::function<Extend()> frameBufferSizeGetter_;
    };

    extern PlatformIO* g_platformIO;
}


#endif