/*****************************************************************************
* @brief : EquationComponent
* 方程组件，该组件不用顶点，用方程的形式描述图形曲面
* 3元n次方程的形式
* @author : acedtang
* @date : 2021/5/29
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/
#ifndef __EQUATION_COMPONENT_H
#define __EQUATION_COMPONENT_H

#include "PrimitiveComponent.h"

namespace Sun {

    class EquationComponent : public SPrimitiveComponent
    {
    public:
        EquationComponent();
        ~EquationComponent();


    };

}

#endif