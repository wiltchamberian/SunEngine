/*****************************************************************************
* @brief : FactoryProducer
* 工厂构造者，根据输入字符串不同，或者宏定义，创建不同工厂
* @author : acedtang
* @date : 2021/5/6
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __FACTORY_PRODUCER_H
#define __FACTORY_PRODUCER_H

namespace Sun {
    class ProgramFactory;
    class ShaderFactory;

    class FactoryProducer {
    public:
        static ProgramFactory* createProgramFactory();
        static ShaderFactory* createShaderFactory();
    };


}

#endif