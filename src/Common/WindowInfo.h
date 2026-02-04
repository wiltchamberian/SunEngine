#ifndef __SUN_COMMON_WINDOW_INFO_H
#define __SUN_COMMON_WINDOW_INFO_H

namespace Sun {

#ifdef _WIN32
    struct WindowInfo {

        int				nativeScreenWidth;	// this is the native screen width resolution of the renderer
        int				nativeScreenHeight; //
    };

#endif


    extern WindowInfo g_winInfo;
}

#endif