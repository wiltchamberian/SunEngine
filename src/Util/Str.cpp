#include "Str.h"

namespace Sun {

    std::string Str::toLowerCase(const std::string& str) {
        std::string res = str;

        for (int i = 0; i < res.size(); ++i) {
            if (res[i] >= 'A' && res[i] <= 'Z') {
                res[i] = res[i] + 'a' - 'A';
            }
        }
        return res;
    }

    std::string Str::toUpperCase(const std::string& str) {
        std::string res = str;

        for (int i = 0; i < res.size(); ++i) {
            if (res[i] >= 'a' && res[i] <= 'z') {
                res[i] = res[i] + 'A' - 'a';
            }
        }
        return res;
    }


}