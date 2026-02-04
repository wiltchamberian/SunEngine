/*****************************************************************************
* @brief : 用户输入指令相关
* @author : acedtang
* @date : 2021/3/15
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/

#ifndef __INPUT_H
#define __INPUT_H

namespace Sun
{
#define PITCH
#define YAW
#define ROLL
    //用户指令
    class SUserCmd
    {
        int gameFrame;
        int	gameTime;
        int duplicateCount;
        //以位域标记的按下的按钮
        unsigned char buttons;
        //移动分量
        signed char	forwardmove;
        signed char rightmove;
        signed char upmove;
        //欧拉角
        int angles[3];
        //鼠标移动分量
        short mx;
        short my;
        //冲击
        signed char impulse;         
        //武器类型
        unsigned char weapon;          
    };
}


#endif