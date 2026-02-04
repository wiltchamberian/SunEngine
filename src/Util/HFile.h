/*****************************************************************************
* @brief : hfile
* 封装了文件处理相关，跨平台，
* 目前简单实现，支持扩展
* @author : acedtang
* @date : 2021/4/7
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __HFILE_H
#define __HFILE_H

#define _CRT_SECURE_NO_WARNINGS 1
#include <cstdio>
#include <string>
#include "Define.h"

namespace Sun {

#define OpenFail -1
#define OpenModeBinary  0x01
#define OpenModeText  0x02
#define OpenModeRead  0x04
#define OpenModeWrite  0x08

    class HFile {
        FILE* file_;
    public:
        HFile();
        ~HFile();
        void clear();
        bool open(const char* path , int mode);
        void close();
        uint32 read(char* outData, uint32 len);
        uint64 readAll(char* outData);
        uint64 readAll(std::string& str);
        int64 openReadAll(const char* path, int mode, char* outData);
    };


}


#endif