#include "UserSetting.h"

namespace Sun {
    UserSetting::UserSetting()
    {
        leftMoveKey_ = 'a';
        rightMoveKey_ = 'd';
        forwardMoveKey_ = 'w';
        backwardMoveKey_ = 's';
        upMoveKey_ = 'q';
        downMoveKey_ = 'e';
    }

    void UserSetting::setLeftMoveKey(int key){
        leftMoveKey_ = key;
    }

    void UserSetting::setRightMoveKey(int key){
        rightMoveKey_ = key;
    }

    void UserSetting::setForwardMoveKey(int key){
        forwardMoveKey_ = key;
    }

    void UserSetting::setBackwardMoveKey(int key){
        backwardMoveKey_ = key;
    }

    void UserSetting::setUpMoveKey(int key)
    {
        upMoveKey_ = key;
    }

    void UserSetting::setDownMoveKey(int key)
    {
        downMoveKey_ = key;
    }

    int UserSetting::getLeftMoveKey() const{
        return leftMoveKey_;
    }

    int UserSetting::getRightMoveKey() const {
        return rightMoveKey_;
    }

    int UserSetting::getForwardMoveKey() const {
        return forwardMoveKey_;
    }

    int UserSetting::getBackwardMoveKey() const {
        return backwardMoveKey_;
    }

    int UserSetting::getUpMoveKey() const
    {
        return upMoveKey_;
    }

    int UserSetting::getDownMoveKey() const
    {
        return downMoveKey_;
    }

    void UserSetting::setMoveSpeed(float speed)
    {
        speed_ = speed;
    }

    float UserSetting::getMoveSpeed() const
    {
        return speed_;
    }

    float UserSetting::getFactor() {
        return factor_;
    }

}
