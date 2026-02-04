#ifndef __METEORWINIO_H
#define __METEORWINIO_H

#include "Platform/PlatformIO.h"
#include <deque>
//#include "../Resources/resource.h"
#include "../resource.h"
//#include"WinUser.h"
#include "windows.h"

namespace Sun
{
    class SWinIO :public PlatformIO
    {
    public:
        void generateEvents() override;
        //SUserCmd generateCmd() override;
        SEvent getEventFromSys() override;
        bool initEnvironment(const RunStruct& stru) override;
        //void render(const RenderInputInfo& info) override;
        SEvent getEvent() override;

        void appActivate(bool active, bool minimize);
        void inFrame();
        Extend getFrameBufferSize() override;
        bool windowShouldClose() override;
        void swapBuffer() override;
    protected:
        BOOL  InitInstance(HINSTANCE, int);

        void activateMouse();
        void deactivateMouse();
        void mouseMove();

        void IN_ActivateWin32Mouse();
        void IN_DeactivateWin32Mouse(void);
        void IN_Win32Mouse(int* mx, int* my);


        bool		activeApp_ = false;
        bool		isMinimized_ = false;
        bool        in_appactive_ = false;
        bool        mouseActive_ = false;

        int	window_center_x = 0;
        int window_center_y = 0;

        Extend extend_;
    };
}


#endif