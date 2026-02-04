#ifndef __MSG_CENTER_H
#define __MSG_CENTER_H

#include <vector>
#include "Entity/GameObject.h"

namespace Sun {

    class MsgCenter {
    public:
        void onNotifyComponentAddedToLevel();
        void addGameObject(GameObject* com);
        void addGameObjects(const std::vector<GameObject*>& vec);
    protected:
        std::vector<GameObject*> comsAddedToLevel_;
    };

    extern MsgCenter* g_msgCenter;
}

#endif