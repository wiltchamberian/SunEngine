#include "AssetsMgr.h"
#include "Util/HFile.h"
#include "AssetsPack.h"
#include "AssetsType.h"
#include "Util/exception.h"

namespace Sun {
#if 0
    void AssetsMgr::load(const char* packpath) {
        HFile hfile;
        std::vector<char> vec;

        uint64 len = hfile.openReadAll(packpath, OpenModeBinary & OpenModeRead ,nullptr);
        vec.resize(len);
        hfile.readAll(vec.data());
        hfile.close();

        parsePackData(vec.data(),len);
    }

    bool AssetsMgr::parsePackData(char* data, uint64 len) {
        if (data == 0) return false;
        ASSERT(len >= sizeof(AssetsPackHead));
        if (len < sizeof(AssetsPackHead)) return false;
        
        char* ptr = data;
        AssetsPackHead& head = *(AssetsPackHead*)(ptr);
        bool check = (head.c1 == 'S' && head.c2 == 'U' && head.c3 == 'N');
        ASSERT(check);
        if (!check) return false;

        uint64 red = 0;
        ptr += sizeof(AssetsPackHead);
        red += sizeof(AssetsPackHead);
        while (ptr - data < len) {
            if (len - (ptr-data) < sizeof(AssetHead)) {
                ASSERT(false);
                return false;
            }
            parseAsset(ptr);
        }
        return true;
    }

    bool AssetsMgr::parseAsset(char*& ptr) {
        AssetHead& head = *(AssetHead*)ptr;
        ptr += sizeof(AssetHead);
        switch (head.assetType) {
        case ASSET_TEXTURE:
        {
            THandle<Texture> handle;
            Texture* p = texMgr_.acquire(handle, head.guid);
            p->load();
            ptr += sizeof(Texture);
        }
        break;
        case ASSET_MATERIAL: 
        {
            THandle<Material> handle;
            Material* p = materialMgr_.acquire(handle, head.guid);
            *p = *((Material*)ptr);
            ptr += sizeof(Material);
        }
        break;
        case ASSET_SKELETON: 
        {
            THandle<Skeleton> handle;
            Skeleton* p = skeletonMgr_.acquire(handle, head.guid);
            *p = *((Skeleton*)ptr);
            ptr += sizeof(Skeleton);
        }
        break;
        case ASSET_ANIMATION:
        {
            THandle<Animation> handle;
            Animation* p = animationMgr_.acquire(handle, head.guid);
            *p = *((Animation*)ptr);
            ptr += sizeof(Animation);
        }
        break;
        default:
        {
            ASSERT(false);
            break;
        }
        }
        return true;
    }
#endif

    AssetsMgr* g_assetsMgr = new AssetsMgr();


}