/*****************************************************************************
* @brief : world
* world just represent a game, it is composed of several levels;
* the engine user can create a world ,then the engine will run the game;
* @author : acedtang
* @date : 2021/4/13
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __WORLD_H
#define __WORLD_H

#include "Entity/Level.h"
#include "System/UserSetting.h"
#include "System/PlayerControlSystem.h"

namespace Sun {

    class SWorld {
    public:
        SWorld();
        SWorld(const char* filepath);
        ~SWorld();
        void init();
        //reset to just like a new SWorld;
        void clear();
        //init with a file ,if set clear ==true ,will call clear before loadfile
        void load(const char* filepath , bool clear = true);
        void prepare();
        void save(const char* filepath);
        void addMainPlayer();

        //the world will manager the memory of the level
        SLevel* createLevel();
        //the world will manager the memory of the level
        void addLevel(SLevel* level);
        void addObject(GameObject* obj);

        SLevel* getCurLevel();
        Character* getMainPlayerPtr();
        Matrix4x4 getMainPlayerCameraProjMatrix();
        Matrix4x4 getMainPlayerCameraViewMatrix();
        Matrix4x4 getMainPlayerCameraVPMatrix();
        Transform3D getMainPlayerCameraViewTransform();
        vec3 getMainPlayerViewPos();

        UserSetting& getUserSetting();

        SharedPtr<SCameraComponent> getCameraCom();
        void setCameraComRelativePosition(const vec3& vec);
        void tick(float dtime, const SEvent& ev);
    protected:
        void loadFile(const char* filepath);

        //global config for the game
        UserSetting setting_;
        std::vector<SLevel*> levels_;
        //the cur level(index of levels_)
        int cur_ = 0;
        Character* mainPlayer_;
        PlayerControlSystem* playerControlSystem_;
    };

}

#endif