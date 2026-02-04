#ifndef __STRING_UTIL_H
#define __STRING_UTIL_H

#include <string>

namespace Sun
{
    //从一个路径中提取文件名
    inline std::string getFileName(const char* filePath) {
        std::string tmp(filePath);
        bool find = false;
        int index = 0;
        for (int i = tmp.size() - 1; i >= 0; --i) {
            if (tmp[i] == '\\' || tmp[i] == '/') {
                index = i;
                find = true;
                break;
            }
        }
        if (find == false) {
            return tmp;
        }
        tmp = tmp.substr(index + 1, tmp.size() - index - 1);
        return tmp;
    }

    //从一个路径中提取目录(不含//)
    inline std::string getFileDir(const char* filePath ,bool withSlash) {
        std::string tmp(filePath);
        bool find = false;
        int index = 0;
        for (int i = tmp.size() - 1; i >= 0; --i) {
            if (tmp[i] == '\\' || tmp[i] == '/') {
                index = i;
                find = true;
                break;
            }
        }
        if (find == false) {
            return std::string();
        }
        if (!withSlash)
            tmp = tmp.substr(0, index);
        else
            tmp = tmp.substr(0, index + 1);
        return tmp;
    }

    //去掉一个字符串前面的所有在数组中出现的字符，并返回处理后的字符串
    inline std::string removePrefixIn(const std::string& str, const std::string& removes) {
        int k = 0;
        for (int i = 0; i < str.size(); ++i) {
            bool bingo = false;
            for (int j = 0; j < removes.size(); ++j) {
                if (str[i] == removes[j]) {
                    bingo = true;
                    break;
                }
            }
            if (!bingo) {
                k = i;
                break;
            }
        }
        if (k == 0)return str;
        return str.substr(k, str.size() - k);
    }
    
    //将字符串分割成子串，子串以起始索引和终止索引（左闭右开,相对于原始字符)组成的pair形式返回（避免拷贝)
    //分隔符号为sp，连续的sp等同为1个 ;对str的start和end区间进行分割
    inline std::vector<std::pair<int,int>> splitString(const std::string& str,int start_, int end_, char sp) {
        std::vector<std::pair<int, int>> res;
        int start = start_;
        for (int i = start_; i <=end_; ++i) {
            if (i==end_||str[i] == sp) {
                if (i > start) {
                    res.push_back(std::make_pair(start, i));
                }
                start = i+1;
            }
        }
        return res;
    }
    
}

#endif