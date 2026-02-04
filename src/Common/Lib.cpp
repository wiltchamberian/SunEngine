#include "Lib.h"
#include <cstdarg>

namespace Sun {

    void Lib::printf(const char* fmt, ...) {
        va_list		argptr;
        va_start(argptr, fmt);
        
        va_end(argptr);
    }

    void Lib::printfIf(const bool test, const char* fmt, ...) {

    }

    void Lib::error(const char* fmt, ...) {

    }

    void Lib::fatal(const char* fmt, ...) {

    }

    void Lib::warning(const char* fmt, ...) {

    }

    void Lib::warningIf(const bool test, const char* fmt, ...) {

    }

    
}