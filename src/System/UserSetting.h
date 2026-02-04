#ifndef __USER_SETTING_H
#define __USER_SETTING_H

/**
 * @projectName   Sun
 * @brief         user setting
 * @author        acedtang
 * @date          2021/01/21
 * 处理游戏用户的配置参数
 */

namespace Sun
{
    class UserSetting
    {
    public:
        UserSetting();
        void setLeftMoveKey(int key);
        void setRightMoveKey(int key);
        void setForwardMoveKey(int key);
        void setBackwardMoveKey(int key);
        void setUpMoveKey(int key);
        void setDownMoveKey(int key);

        int getLeftMoveKey() const;
        int getRightMoveKey() const;
        int getForwardMoveKey() const;
        int getBackwardMoveKey() const;
        int getUpMoveKey() const;
        int getDownMoveKey() const;

        //设置移动速率
        void setMoveSpeed(float speed);
        float getMoveSpeed() const;
        float getFactor();
    protected:
        int leftMoveKey_;
        int rightMoveKey_;
        int forwardMoveKey_;
        int backwardMoveKey_;
        int upMoveKey_;
        int downMoveKey_;
        // m/s
        float speed_ = 2.0;

        //缩放因子(真实世界的长度/m映射到游戏世界的数值)
        //也就是游戏世界中该因子的值对应的真实世界的1m
        float factor_ = 1.0;//0.02;
    };
}



#endif

