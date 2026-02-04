/*****************************************************************************
* @brief : RenderAble
* 接口类 之 renderAble
* 出于面向接口编程思想，我们从接口维度来定义对象，当一个对象具备某个接口之后
* 它就能被任何可以处理那些接口的函数所处理；z
* 这与常规的组件实体或者ECS模式不同，在另外一个维度上定义了对象能力，因此更为灵活
* renderAble
* @author : acedtang
* @date : 2021/5/14
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#include "Interface.h"

namespace Sun {

    class RenderAble //: virtual public InterfaceBase
    {
    public:
        virtual void prepareRender() = 0;
        virtual void render() = 0;

    };


}