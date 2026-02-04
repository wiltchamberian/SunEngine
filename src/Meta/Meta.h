//该文件由元编译器生成
#ifndef __META_H
#define __META_H

#include "Util/QuickMemoryPool.h"
#include "Entity/GameObject.h"
#include "Entity/Pawn.h"

namespace Sun {
    
    //元数据,描述了一个结构体
    struct MetaData {
        //唯一标识id
        int id_;
        //结构体长度
        int size_;
    };

    class Meta
    {
        template<class _T> friend void destroy(_T* data);
        class FactoryManager
        {
        public:
            FactoryManager();

            QuickMemoryPool<Pawn> PawnPool_;
            QuickMemoryPool<GameObject> RotorPool_;
            QuickMemoryPool<GameObject> ActorPool_;

            std::vector<QuickMemoryPool*>  factories_;
        };
    public:
        static void log();
        static void componentSnapshot(GameObject* remote);

        static Pawn* createPawn();
        static GameObject* createRotor();
        static void destroyRotor(GameObject*& ptr);
        static void destroyActor(GameObject*& entity);
    private:
        static Meta::FactoryManager factoryMgr_;
        
    };
    
    template<class _T>
    sp<_T> createRotor() {
        return nullptr;
    }

    template<>
    inline sp<GameObject> createRotor() {
        return Meta::createRotor();
    }

    template<>
    inline sp<Pawn> createRotor() {
        return Meta::createPawn();
    }

    template<class _T>
    void destroy(sp<_T> data) {
        ;
    }

    template<>
    inline void destroy(sp<GameObject> data) {
        Meta::factoryMgr_.RotorPool_.free(data);
    }

    template<>
    inline void destroy(sp<Pawn> data) {
        Meta::factoryMgr_.PawnPool_.free(data);
    }

}



#endif

