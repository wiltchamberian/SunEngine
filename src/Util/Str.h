#ifndef __SUN_UTIL_STR_H
#define __SUN_UTIL_STR_H

#include <string>

namespace Sun {

    class Str {
    public:
        static std::string toLowerCase(const std::string& str);
        static std::string toUpperCase(const std::string& str);

        static inline char toLowerCase(char c) {
            if (c >= 'A' && c <= 'Z') {
                return c + 'a' - 'A';
            }
            return c;
        }

        static char inline toUpperCase(char c) {
            if (c >= 'a' && c <= 'z') {
                return c + 'A' - 'a';
            }
            return c;
        }

        static inline bool isUpperCase(char c) {
            return 'A' <= c && c <= 'Z';
        }

        static inline bool isLowerCase(char c) {
            return 'a' <= c && c <= 'z';
        }

        static inline bool isNumber(char c) {
            return c >= '0' && c <= '9';
        }

        static inline bool isPrefix(const std::string& prefix, const std::string& target) {
            if (prefix.size() > target.size()) return false;
            for (int i = 0; i < prefix.size(); ++i) {
                if (prefix[i] != target[i])return false;
            }
            return true;
        }

        static inline bool isSuffix(const std::string& suffix, const std::string& target) {
            if (suffix.size() > target.size()) return false;
            int index = target.size() - suffix.size();
            for (int i = 0; i < suffix.size(); ++i,++index) {
                if (suffix[i] != target[index]) return false;
            }
            return true;
        }
    };
}

#endif