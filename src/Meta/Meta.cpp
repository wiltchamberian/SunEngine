#include "Meta.h"
#include <assert.h>
#include <iostream>
//#include "ArchetypeTest.h"

namespace Sun {

    Meta::FactoryManager Meta::factoryMgr_;

    void Meta::log()
    {
       /* std::cout << "Component1:" << factoryMgr_.PawnPool_.size() << std::endl
            << "Component_Root:" << factoryMgr_.RotorPool_.size() << std::endl;*/
    }

    /* ComponentFactory<Component1> Meta::Component1_factory_;
     ComponentFactory<Component2> Meta::Component2_factory_;
     ComponentFactory<ComponentRoot> Meta::ComponentRoot_factory_;*/

    //void Meta::componentSnapshot(sp<ComponentRoot> remote)
    //{
    //    assert(remote->type_ >= 0 && remote->type_ < factoryMgr_.factories_.size());
    //    factoryMgr_.factories_[remote->type_]->componentSnapshot(remote);
    //}

    Pawn* Meta::createPawn()
    {
        return factoryMgr_.PawnPool_.malloc();
    }

    GameObject* Meta::createRotor()
    {
        return factoryMgr_.RotorPool_.malloc();
    }

    void Meta::destroyRotor(GameObject*& ptr)
    {
 
    }

    void Meta::destroyActor(GameObject*& entity)
    {

    }

    Meta::FactoryManager::FactoryManager()
    {
        ActorPool_.setType(factories_.size());
        factories_.push_back(&ActorPool_);

        PawnPool_.setType(factories_.size());
        factories_.push_back(&PawnPool_);

        RotorPool_.setType(factories_.size());
        factories_.push_back(&RotorPool_);

        
    }


}

