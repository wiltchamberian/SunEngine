/*****************************************************************************
* @brief : ProgramFactory
* 着色器工厂，跨平台，实例化为opengl,vulkan等
* @author : acedtang
* @date : 2021/5/6
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __PROGRAM_FACTORY_H
#define __PROGRAM_FACTORY_H

#include <vector>

namespace Sun {

    class ProgramFactory {
    public:
        virtual int createProgram(int shader) = 0;
        virtual int createProgram(const std::vector<int>& shaders) = 0;
    };
}

#endif