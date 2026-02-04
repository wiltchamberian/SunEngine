#include "FileIO.h"
#include <cassert>

namespace Sun {

    //以2进制方式读取整个文件为字节流
    std::vector<char> FileIO::readFileBinary(const std::string& filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("failed to open file!");
        }
        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;

    }

    std::vector<char> FileIO::readFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::ate);

        if (!file.is_open()) {
            throw std::runtime_error("failed to open file!");
        }
        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;

    }

    void FileIO::writeFileBinary(const char* data, int len, const std::string& path) {
        if (data == nullptr || len < 0) {
            assert(false); return;
        }
        std::fstream file(path, std::ios::out|std::ios::binary);
        if (!file.is_open()) {
            assert(false);
            return;
        }
        file.write(data, len);
    }

    void FileIO::writeFile(const char* data, int len, const std::string& path) {
        if (data == nullptr || len < 0) {
            assert(false); return;
        }
        std::fstream file(path, std::ios::out);
        if (!file.is_open()) {
            assert(false);
            return;
        }
        file.write(data, len);
    }
}