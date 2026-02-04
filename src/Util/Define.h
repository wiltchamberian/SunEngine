#ifndef __UTIL_DEFINE_H
#define __UTIL_DEFINE_H

#include <stdint.h>

namespace Sun {

#define SCLASS()  //元对象处理

    typedef int32_t int32;
    typedef int64_t int64;
    typedef uint32_t uint32;
    typedef uint64_t uint64;
    typedef unsigned char  uchar;
    typedef char char8;

    //this struct should contain all platforms main input prarams
    struct RunStruct
    {
        long long hInstance;
        long long preInstance;
        void* cmdline;
        int cmdshow;
        int window_width;
        int window_height;
    };

//#define OPEN_TEST   //放开表示打开测试代码

}


#endif