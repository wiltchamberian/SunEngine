/*****************************************************************************
* @brief :Hash
* 一些简单的hash函数，参考了UBPA::UECS
* @author : acedtang
* @date : 2021/8/20
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __SUN_HASH_H
#define __SUN_HASH_H

#include <cstdio>

namespace Sun {

    inline constexpr std::size_t hash_combine(std::size_t x, std::size_t y) noexcept {
        return x ^ (y + 0x9e3779b9 + (x << 6) + (x >> 2));
    }

    template<class _Iter>
    inline constexpr std::size_t hash_combine_c(_Iter begin, _Iter end) {
        if (begin == end) return 0;
        std::size_t res = *begin;
        for (auto it = begin + 1; it != end; ++it) {
            res = hash_combine(res, (*it));
        }
        return res;
    }

}

#endif