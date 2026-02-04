/*****************************************************************************
* @brief : exception
* 异常，断言的处理
* @author : acedtang
* @date : 2021/4/6
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __EXCEPTION_H
#define __EXCEPTION_H

#include <cassert>

namespace Sun
{
#ifndef ASSERT
//#define ASSERT(exp) \
//{   static bool ignoreAlways = false; \
//    if(!ignoreAlways) {\
//        ignoreAlways = true;\
//        assert(exp); \
//    }\
//}
#define ASSERT assert
#endif

#define ASSERT_EQUAL(a,b) ASSERT(a==b)
#define ASSERT_EQ(a, b) ASSERT(a == b)
#define ASSERT_GE(a, b) ASSERT(a >= b)
#define ASSERT_GT(a, b) ASSERT(a > b)
#define ASSERT_LE(a, b) ASSERT(a <= b)
#define ASSERT_LT(a, b) ASSERT(a < b)
#define ASSERT_NE(a, b) ASSERT(a != b)

    void reportException(const char* data = nullptr);
};

#endif

