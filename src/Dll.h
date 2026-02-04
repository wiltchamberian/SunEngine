/*****************************************************************************
* @brief : 用于描述加载的game的dll信息
* @author : acedtang
* @date : 2021/2/21
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/
#ifndef __METEOR_DLL_H
#define __METEOR_DLL_H

#define QDECL __cdecl

struct SDll
{
    //dll调exe
    int			(*systemCall)(void* parms , void* parms2);
    
    //exe调dll
    // for dynamic linked modules
    void*     dllHandle;
    int			(QDECL* entryPoint)(void* parms, void* parms2);
};


#endif
