#include "PlayerControlSystem.h"
#include "Entity/Character.h"
#include "Common/Common.h"
#include "Common/KeyBoard.h"
#include "Entity/World.h"
#include "Util/exception.h"
#include <iostream>

namespace Sun {

    PlayerControlSystem::PlayerControlSystem(SWorld* world) {
        world_ = world;
    }

    void PlayerControlSystem::prepare() {

    }

    void PlayerControlSystem::tick(float dtime, const SEvent& ev) {
        if (!world_) return;
        Character* pawn = world_->getMainPlayerPtr();
        if (!pawn) {
            ASSERT(false);
            return;
        }

        std::shared_ptr<SCameraComponent> cameraCom = pawn->getCameraCom();
  
        UserSetting& setting = world_->getUserSetting();

        float speed = setting.getMoveSpeed();
        float dis = speed * dtime * setting.getFactor();

        //GameObject* rotor = actor_->rootRotor().data();
        
        //left,right,forward,backward
        int keycode = setting.getLeftMoveKey();
        bool bingo = false;
        if (g_keys[keycode].down == true) {
            pawn->moveRight(-dis,false);
            bingo = true;
        }
        keycode = setting.getRightMoveKey();
        if (g_keys[keycode].down == true) {
            pawn->moveRight(dis,false);
            bingo = true;
        }
        keycode = setting.getForwardMoveKey();
        if (g_keys[keycode].down == true) {
            pawn->moveForward(dis,false);
            bingo = true;
        }
        keycode = setting.getBackwardMoveKey();
        if (g_keys[keycode].down == true) {
            pawn->moveForward(-dis,false);
            bingo = true;
        }
        keycode = setting.getUpMoveKey();
        if (g_keys[keycode].down == true) {
            pawn->moveUp(dis,false);
            bingo = true;
        }
        keycode = setting.getDownMoveKey();
        if (g_keys[keycode].down == true) {
            pawn->moveUp(-dis,false);
            bingo = true;
        }
        if(bingo)
            pawn->updateDescendantsTransform();

        //鼠标移动事件
        if (ev.type == SE_MOUSE) {
            if (ev.width == 0 || ev.height == 0) return;
            float dx = float(ev.value) / ev.width;
            float dy = float(ev.value2) / ev.height;
            float coff = A_HALF_PI;
            dx *= coff;
            dy *= coff;
            cameraCom->addYaw(-dx);
            if (dy > 0.01 || dy < -0.01) {
                int a = 0; int b = a;
            }
            cameraCom->addPitch(-dy);
            //std::cout << "dx:" << dx << std::endl;
            //std::cout << "dy:" << dy << std::endl;
        }
        //鼠标点击事件
        else if (ev.type == SE_MOUSE_CLICK) {
            //left
            if (ev.value == 1) {
                //发射导弹
                actor_->spawn();
            }
        }
    }

}