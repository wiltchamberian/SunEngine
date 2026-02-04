#include "MsgCenter.h"

namespace Sun {

    MsgCenter* g_msgCenter = new MsgCenter();

    void MsgCenter::onNotifyComponentAddedToLevel() {
        for (int i = 0; i < comsAddedToLevel_.size(); ++i) {
            comsAddedToLevel_[i]->onAddedToLevel();
        }
        comsAddedToLevel_.clear();
    }

    void MsgCenter::addGameObject(GameObject* com) {
        comsAddedToLevel_.push_back(com);
    }

    void MsgCenter::addGameObjects(const std::vector<GameObject*>& vec) {
        for (auto& it : vec) {
            comsAddedToLevel_.push_back(it);
        }
    }
}