#ifndef __SUN_ENGINE_H
#define __SUN_ENGINE_H

#include <string>
#include <vector>
#include "Common/Common.h"
#include "Util/Define.h"
#include "Entity/World.h"
#include "ObjectFactory.h"

namespace Sun
{

    // settings
    extern const unsigned int SCR_WIDTH;
    extern const unsigned int SCR_HEIGHT;

    class SLevel;
    class SPlatformIO;

    class GameBase {
    public:
        virtual SWorld* createWorld() = 0;
    };

    //represet the engine
#ifndef SUN
#define SUN SEngine::get()
#endif

    class SEngine
    {
    protected:
        SEngine();
        ~SEngine();

    public:
        void initEnvironment(const RunStruct& stru);
        void initGlobal();
        void run();

        //const MeteorMainPlayer& getMainPlayer();
        
        //void useCamera(std::shared_ptr<MeteorCamera>);

        //in use
        static SEngine* get();
        void loadGame(GameBase* game);
        SWorld* getWorld();
        SLevel* getCurLevel();
    protected:
        void tick(float dtime, const SEvent& ev);
        //服务端处理
        void svFrame();
        //客户端处理
        void clFrame();
        //发消息给服务端
        void sendMsgToServer();

        static SEngine* _instance;
        
        
        //std::weak_ptr<MeteorCamera> _usingCamera;

        //in use
        SWorld* world_;
    };

}

#endif

