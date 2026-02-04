#include "PlatformMisc.h"
#include "Util/exception.h"
#include <filesystem>
#include <chrono>

#ifdef _WIN32
#include "objbase.h"
#pragma comment(lib,"ole32.lib")
#endif


namespace Sun {

    Guid createGuid() {
        Guid guid;
#ifdef _WIN32
        HRESULT hr = CoCreateGuid((GUID*)&guid);
        if (FAILED(hr))
        {
            ASSERT(false);
        }
#endif
        return guid;
    }

    std::string getGlobalPath() {
        //std::filesystem::path path = std::filesystem::current_path();
        //std::string str = path.u8string();
        //return str  + "/";

        //return "E:/GIT/testproj/SunEngine/myproj/Debug/";

#if defined _WIN32
        TCHAR szPath[MAX_PATH];
        ::GetModuleFileName(NULL, szPath, MAX_PATH);

        //去掉执行的文件名。
        (strrchr(szPath, '\\'))[1] = 0;
        return std::string(szPath);
#endif

    }

    std::string getResourceDir() {
        return "G:/SunEngine__old/myproj/GameResource/";
    }

    double getTime() {
        //return glfwGetTime();
        assert(false); return 0;
    }

#if 0
    int sys_timeBase = 0;
    double Sys_Milliseconds()
    {
  //from quake3 not cross platform
        int			sys_curtime;
        static bool	initialized = false;

        if (!initialized) {
            sys_timeBase = ::timeGetTime();
            initialized = true;
        }
        sys_curtime = ::timeGetTime() - sys_timeBase;

        return sys_curtime;

    }
#endif

        //return glfwGetTime()*0.001;

       
}