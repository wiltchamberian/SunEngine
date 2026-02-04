#ifndef __SUN_COMMON_LIB_H
#define __SUN_COMMON_LIB_H

namespace Sun {

    class Lib {
    public:
        static void printf(const char* fmt, ...);
        static void printfIf(const bool test, const char* fmt, ...);
        static void error(const char* fmt, ...);
        static void fatal(const char* fmt, ...);
        static void warning(const char* fmt, ...);
        static void warningIf(const bool test, const char* fmt, ...);

    };

#define LIB_PRINT(abc) Lib::printf(abc);
#define LIB_ERROR(abc) Lib::error(abc);
}

#endif