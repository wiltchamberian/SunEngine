/*****************************************************************************
* @brief : AssetMgr
* @author : acedtang
* @date : 2021/4/7
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __ASSET_MGR_H
#define __ASSET_MGR_H

#include <type_traits>
#include <unordered_map>
#include <utility>
#include "HandleMgr.h"
#include "Util/Singleton.h"
#include "Component/MaterialCom.h"
#include "Assets/Skeleton.h"
#include "Assets/Animation.h"
#include "Assets/SkeletalMesh.h"
#include "Assets/StaticMesh.h"
#include "Primitive/Image.h"
#include "Assets/AssetsType.h"
#include "Platform/PlatformMisc.h"

namespace Sun {
#if 0
    class AssetsMgr {
    public:
        typedef THandleMgr<Texture, THandle<Texture>> TextureMgr;
        typedef THandleMgr<Material, THandle<Material>> MaterialMgr;
        typedef THandleMgr<Skeleton, THandle<Skeleton>> SkeletonMgr;
        typedef THandleMgr<Animation, THandle<Animation>> AnimationMgr;
        typedef THandleMgr<SkeletalMesh, THandle<SkeletalMesh>> SkeletalMeshMgr;
        typedef THandleMgr<StaticMesh, THandle<StaticMesh>> StaticMeshMgr;
        //typedef THandleMgr<Shader, THandle<Shader>> ShaderMgr;
        
        typedef THandleMgr<Skeleton2, THandle<Skeleton2>> Skeleton2Mgr;
        typedef THandleMgr<Matrix4x4, THandle<Matrix4x4>> Matrix4x4Mgr;
    private:        
        TextureMgr texMgr_;
        MaterialMgr materialMgr_;
        SkeletonMgr skeletonMgr_;
        AnimationMgr animationMgr_;
        //ShaderMgr shaderMgr_;
        SkeletalMeshMgr skeletalMeshMgr_;
        StaticMeshMgr staticMeshMgr_;
        Matrix4x4Mgr matrix4x4Mgr_;
        Skeleton2Mgr skeleton2Mgr_;
    public:
        template<class _T>
        inline THandleMgr<_T, THandle<_T>>& getMgr() {
        }
        template<>
        inline THandleMgr< Texture, THandle< Texture>>& getMgr<Texture>() {
            return texMgr_;
        }
        template<>
        inline THandleMgr< Material, THandle< Material>>& getMgr<Material>() {
            return materialMgr_;
        }
        template<>
        inline THandleMgr< Skeleton, THandle< Skeleton>>& getMgr<Skeleton>() {
            return skeletonMgr_;
        }
        template<>
        inline THandleMgr< Animation, THandle< Animation>>& getMgr<Animation>() {
            return animationMgr_;
        }
        template<>
        inline THandleMgr< SkeletalMesh, THandle< SkeletalMesh>>& getMgr<SkeletalMesh>() {
            return skeletalMeshMgr_;
        }
        template<>
        inline THandleMgr< StaticMesh, THandle< StaticMesh>>& getMgr<StaticMesh>() {
            return staticMeshMgr_;
        }
        template<>
        inline THandleMgr< Matrix4x4, THandle< Matrix4x4>>& getMgr<Matrix4x4>() {
            return matrix4x4Mgr_;
        }
        template<>
        inline THandleMgr< Skeleton2, THandle< Skeleton2>>& getMgr<Skeleton2>() {
            return skeleton2Mgr_;
        }

        void load(const char* packpath);
    protected:
        bool parsePackData(char* data, uint64 len);
        bool parseAsset(char*& ptr);
    };
#endif

    template<typename _Key, typename _T ,typename _Hash = std::hash<_Key>, typename _KeyEqual = std::equal_to<_Key>>
    class AssetsContainer {
        friend class AssetsMgr;
    public:
        static std::unordered_map<_Key, SharedPtr<_T>, _Hash, _KeyEqual> map_;
    };
    template<typename _Key, typename _T, typename _Hash , typename _KeyEqual >
    std::unordered_map<_Key, SharedPtr<_T>, _Hash, _KeyEqual> AssetsContainer<_Key, _T, _Hash, _KeyEqual>::map_;
    
    class AssetsMgr {
    public:
        std::unordered_map<Guid, std::shared_ptr<MaterialCom>, GuidHashFunc, GuidEqual> materials_;
        std::unordered_map<Guid, std::shared_ptr<Skeleton>, GuidHashFunc, GuidEqual> skeletons_;
        std::unordered_map<Guid, std::shared_ptr<Animation>, GuidHashFunc, GuidEqual> animations_;
        std::unordered_map<Guid, std::shared_ptr<Skeleton2>, GuidHashFunc, GuidEqual> skeletons2_;
        std::unordered_map<Guid, std::shared_ptr<StaticMesh>, GuidHashFunc, GuidEqual> staticMeshes_;
        std::unordered_map<Guid, std::shared_ptr<Image>, GuidHashFunc, GuidEqual> textures_;
        std::unordered_map<Guid, std::shared_ptr<SkeletalMesh>, GuidHashFunc, GuidEqual> skeletalMeshes_;
    };

    extern AssetsMgr* g_assetsMgr;

    template<typename _Key,typename _Type, typename ... _Inputs>
    SharedPtr<_Type> acquire(const _Key& key, _Inputs && ... _Args) {
        auto& it = AssetsContainer<_Key, _Type>::map_.find(key);
        if (it != AssetsContainer<_Key, _Type>::map_.end()) {
            return AssetsContainer<_Key, _Type>::map_[key];
        }
        else {
            SharedPtr<_Type> ptr(new _Type(::std::forward<_Inputs>(_Args)...));
            AssetsContainer<_Key, _Type>::map_.insert(std::make_pair(key, ptr));
            return ptr;
        }
        return nullptr;
    }

    

    template<class _MaterialType, class... _Types>
    std::shared_ptr<_MaterialType> acquireMaterial(_Types&&... _Args)
    {
        std::shared_ptr<_MaterialType> ptr(new _MaterialType(_STD forward<_Types>(_Args)...));
        ptr->guid_ = createGuid();
        g_assetsMgr->materials_.insert(std::make_pair(ptr->guid_, ptr));
        return ptr;
    }

    template<class... _Types>
    std::shared_ptr<Skeleton> acquireSkeleton(_Types&&... _Args)
    {
        std::shared_ptr<Skeleton> ptr(new Skeleton(_STD forward<_Types>(_Args)...));
        ptr->guid_ = createGuid();
        g_assetsMgr->skeletons_.insert(std::make_pair(ptr->guid_, ptr));
        return ptr;
    }

    template<class... _Types>
    std::shared_ptr<Skeleton2> acquireSkeleton2(_Types&&... _Args)
    {
        std::shared_ptr<Skeleton2> ptr(new Skeleton2(_STD forward<_Types>(_Args)...));
        ptr->guid_ = createGuid();
        g_assetsMgr->skeletons2_.insert(std::make_pair(ptr->guid_, ptr));
        return ptr;
    }

    template<class... _Types>
    std::shared_ptr<Animation> acquireAnimation(_Types&&... _Args)
    {
        std::shared_ptr<Animation> ptr(new Animation(_STD forward<_Types>(_Args)...));
        ptr->guid_ = createGuid();
        g_assetsMgr->animations_.insert(std::make_pair(ptr->guid_, ptr));
        return ptr;
    }

    template<class... _Types>
    std::shared_ptr<StaticMesh> acquireStaticMesh(_Types&&... _Args)
    {
        std::shared_ptr<StaticMesh> ptr(new StaticMesh(_STD forward<_Types>(_Args)...));
        ptr->guid_ = createGuid();
        g_assetsMgr->staticMeshes_.insert(std::make_pair(ptr->guid_, ptr));
        return ptr;
    }

    template<class... _Types>
    std::shared_ptr<Image> acquireImage(_Types&&... _Args)
    {
        std::shared_ptr<Image> ptr(new Image(_STD forward<_Types>(_Args)...));
        ptr->guid_ = createGuid();
        g_assetsMgr->textures_.insert(std::make_pair(ptr->guid_, ptr));
        return ptr;
    }

    template<class... _Types>
    std::vector <std::shared_ptr<Image>> acquireTextures(_Types&&... _Args, int n) {
        if (n > 0) return {};
        std::vector <std::shared_ptr<Image>> vec;
        for (int i = 0; i < n; ++i) {
            vec.push_back(acquireTexture(_Args));
        }
        return vec;
    }

    template<class... _Types>
    std::shared_ptr<SkeletalMesh> acquireSkeletalMesh(_Types&&... _Args)
    {
        std::shared_ptr<SkeletalMesh> ptr(new SkeletalMesh(_STD forward<_Types>(_Args)...));
        ptr->guid_ = createGuid();
        g_assetsMgr->skeletalMeshes_.insert(std::make_pair(ptr->guid_, ptr));
        return ptr;
    }
    
}


#endif