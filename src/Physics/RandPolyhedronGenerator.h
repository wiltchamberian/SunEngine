/*****************************************************************************
* @brief : 随机凸多面体生成
* @author : acedtang
* @date : 2021/2/9
* @version : ver 1.0
* @inparam : 
* @outparam :
*****************************************************************************/

#ifndef __RANDPOLYHEDRONGENERATOR_H
#define __RANDPOLYHEDRONGENERATOR_H

namespace Sun
{
    class RandPolyhedronGenerator
    {
    public:
        //在空间[0,x],[0,y],[0,z]随机生成多面体
        void setRange(double x, double y, double z);

    };

}


#endif