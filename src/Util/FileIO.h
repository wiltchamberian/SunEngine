#ifndef __SUN_UTIL_UFILE_H
#define __SUN_UTIL_UFILE_H

#include <fstream>
#include <vector>

namespace Sun {

    class FileIO {
    public:
        //以2进制方式读取整个文件为字节流
        static std::vector<char> readFileBinary(const std::string& filename);

        static std::vector<char> readFile(const std::string& filename);

        static void writeFileBinary(const char* data, int len, const std::string& path);
        static void writeFile(const char* data, int len, const std::string& path);
    };
    

}

#endif