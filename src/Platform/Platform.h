/*****************************************************************************
* @brief : Platform.h
* 将所有特定平台相关的代码放在一个文件中，宏定义中只需包含平台头文件即可
* @author : acedtang
* @date : 2021/7/25
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __PLATFORM_H
#define __PLATFORM_H

#ifdef _WIN32
#include "WinPlatform.h"
#endif

#ifdef _LINUX
//TODO
#endif

#endif