#include "HFile.h"
#include "Util/exception.h"

namespace Sun {

    HFile::HFile()
        :file_(0)
    {
    }

    HFile::~HFile() {
        clear();
    }

    void HFile::clear() {
        if (file_)
            fclose(file_);
        file_ = nullptr;
    }

    bool HFile::open(const char* path, int mode) {
        if ((mode & OpenModeBinary) && (mode & OpenModeRead)) {
            file_ = fopen(path, "rb");
        }
        else if ((mode & OpenModeBinary) && (mode & OpenModeWrite)) {
            file_ = fopen(path, "wb");
        }
        else if ((mode & OpenModeText) && (mode & OpenModeRead)) {
            file_ = fopen(path, "r");
        }
        if (file_ != 0) return true;
        return false;
    }

    void HFile::close() {
        if(!file_)
            fclose(file_);
    }

    uint32 HFile::read(char* outData, uint32 len) {
        if (!file_) return 0;
        fread(outData, 1, len, file_);
        return len;
    }

    uint64 HFile::readAll(char* outData) {
        if (file_ != 0) {
            size_t pos = ftell(file_);
            fseek(file_, 0, SEEK_END);
            long size = ftell(file_);
            fseek(file_, 0, SEEK_SET);
            fread(outData, 1, size, file_);
            fseek(file_, pos, SEEK_SET);
            return size;
        }
        return 0;
    }

    uint64 HFile::readAll(std::string& str) {
        if (file_ != 0) {
            size_t pos = ftell(file_);
            fseek(file_, 0, SEEK_END);
            long size = ftell(file_);
            fseek(file_, 0, SEEK_SET);
            str.resize(size);
            fread((void*)str.data(), 1, size, file_);
            fseek(file_, pos, SEEK_SET);
            return size;
        }
        return 0;
    }

    int64 HFile::openReadAll(const char* path, int mode, char* outData) {
        ASSERT(mode & OpenModeRead);
        bool ok = false;
        if(file_==0)
            ok = open(path, mode);
        if (!ok) return -1;
        if (outData == nullptr) {
            fseek(file_, 0, SEEK_END);
            uint64 l = ftell(file_);
            fseek(file_, 0, SEEK_SET);
            return l;
        }
        else {
            return readAll(outData);
        }
    }

}