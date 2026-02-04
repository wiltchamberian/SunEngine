#include "PrimitiveMemoryManager.h"

namespace Sun {

    PrimitiveMemoryManager g_primitiveMgr;

    //返回primitiveEntity索引 ,-1为无效值 ,申请内存
    int PrimitiveMemoryManager::acquire(int primitiveType) {
        int res = -1;
        PrimitiveEntity ent;
        if (!freedEntities_.empty()) {
            res = freedEntities_.back();
            freedEntities_.pop_back();
        }
        else {
            primitiveEntities_.emplace_back();
            res = primitiveEntities_.size() - 1;
        }
        switch (primitiveType) {
        case ShapeType::BOX:
        {
            boxPrimitives_.emplace_back();
            primitiveEntities_[res].offset = boxPrimitives_.size() - 1;
            primitiveEntities_[res].len = sizeof(BoxPrimitive);
            primitiveEntities_[res].type = primitiveType;
            boxReverseMap_.emplace_back();
            boxReverseMap_.back() = res;
        }
        break;
        case ShapeType::SKELETAL_MESH:
        {
            skeletalMeshPrimitives_.emplace_back();
            primitiveEntities_[res].offset = skeletalMeshPrimitives_.size() - 1;
            primitiveEntities_[res].len = sizeof(SkeletalMeshPrimitive);
            primitiveEntities_[res].type = primitiveType;
            skeletalMeshReverseMap_.emplace_back();
            skeletalMeshReverseMap_.back() = res;
        }
        break;
        case ShapeType::STATIC_MESH:
        {
            staticMeshPrimitives_.emplace_back();
            primitiveEntities_[res].offset = staticMeshPrimitives_.size() - 1;
            primitiveEntities_[res].len = sizeof(StaticMeshPrimitive);
            primitiveEntities_[res].type = primitiveType;
            staticMeshReverseMap_.emplace_back();
            staticMeshReverseMap_.back() = res;
        }
        break;
        case ShapeType::SPHERE:
        {
            spherePrimitives_.emplace_back();
            primitiveEntities_[res].offset = spherePrimitives_.size() - 1;
            primitiveEntities_[res].len = sizeof(SpherePrimitive);
            primitiveEntities_[res].type = primitiveType;
            sphereReverseMap_.emplace_back();
            sphereReverseMap_.back() = res;
        }
        break;
        default:
        {
            ASSERT(false);
        }
        break;
        }
        return res;
    }

    //索引数据
    bool PrimitiveMemoryManager::deReference(int h, BoxPrimitive*& data) {
        ASSERT(h >= 0 && h < primitiveEntities_.size());
        PrimitiveEntity& ent = primitiveEntities_[h];
        if (ent.offset < 0) {
            data = nullptr;
            return false;
        }
        ASSERT(ent.type == BoxPrimitive::type_);
        if (ent.type != BoxPrimitive::type_) return false;
        ASSERT(ent.offset >= 0 && ent.offset < boxPrimitives_.size());
        data = &boxPrimitives_[ent.offset];
        return true;
    }

    //索引数据
    bool PrimitiveMemoryManager::deReference(int h, SpherePrimitive*& data) {
        ASSERT(h >= 0 && h < primitiveEntities_.size());
        PrimitiveEntity& ent = primitiveEntities_[h];
        if (ent.offset < 0) {
            data = nullptr;
            return false;
        }
        ASSERT(ent.type == SpherePrimitive::type_);
        if (ent.type != SpherePrimitive::type_) return false;
        ASSERT(ent.offset >= 0 && ent.offset < spherePrimitives_.size());
        data = &spherePrimitives_[ent.offset];
        return true;
    }

    //索引数据
    bool PrimitiveMemoryManager::deReference(int h, StaticMeshPrimitive*& data) {
        ASSERT(h >= 0 && h < primitiveEntities_.size());
        PrimitiveEntity& ent = primitiveEntities_[h];
        if (ent.offset < 0) {
            data = nullptr;
            return false;
        }
        ASSERT(ent.type == StaticMeshPrimitive::type_);
        if (ent.type != StaticMeshPrimitive::type_) return false;
        ASSERT(ent.offset >= 0 && ent.offset < staticMeshPrimitives_.size());
        data = &staticMeshPrimitives_[ent.offset];
        return true;
    }

    //索引数据
    bool PrimitiveMemoryManager::deReference(int h, SkeletalMeshPrimitive*& data) {
        ASSERT(h >= 0 && h < primitiveEntities_.size());
        PrimitiveEntity& ent = primitiveEntities_[h];
        if (ent.offset < 0) {
            data = nullptr;
            return false;
        }
        ASSERT(ent.type == SkeletalMeshPrimitive::type_);
        if (ent.type != SkeletalMeshPrimitive::type_) return false;
        ASSERT(ent.offset >= 0 && ent.offset < skeletalMeshPrimitives_.size());
        data = &skeletalMeshPrimitives_[ent.offset];
        return true;
    }

    //删除
    bool PrimitiveMemoryManager::destroy(int h) {
        ASSERT(h >= 0 && h < primitiveEntities_.size());
        if (h < 0 || h >= primitiveEntities_.size()) return false;

        PrimitiveEntity& ent = primitiveEntities_[h];
        if (ent.offset < 0) return false;
        switch (ent.type) {
        case ShapeType::BOX:
        {
            ASSERT(ent.offset < boxPrimitives_.size());
            if (ent.offset >= boxPrimitives_.size()) return false;
            boxPrimitives_[ent.offset] = boxPrimitives_.back();
            int entIndex = boxReverseMap_[boxPrimitives_.size() - 1];
            boxPrimitives_.pop_back();
            boxReverseMap_.pop_back();
            ASSERT(entIndex >= 0 && entIndex < primitiveEntities_.size());
            primitiveEntities_[entIndex].offset = ent.offset;
            ent.offset = -1;
            freedEntities_.push_back(h);
        }
        break;
        case ShapeType::SKELETAL_MESH:
        {
            ASSERT(ent.offset < skeletalMeshPrimitives_.size());
            if (ent.offset >= skeletalMeshPrimitives_.size()) return false;
            skeletalMeshPrimitives_[ent.offset] = skeletalMeshPrimitives_.back();
            int entIndex = skeletalMeshReverseMap_[skeletalMeshPrimitives_.size() - 1];
            skeletalMeshPrimitives_.pop_back();
            skeletalMeshReverseMap_.pop_back();
            ASSERT(entIndex >= 0 && entIndex < primitiveEntities_.size());
            primitiveEntities_[entIndex].offset = ent.offset;
            ent.offset = -1;
            freedEntities_.push_back(h);
        }
        break;
        case ShapeType::STATIC_MESH:
        {
            ASSERT(ent.offset < staticMeshPrimitives_.size());
            if (ent.offset >= staticMeshPrimitives_.size()) return false;
            staticMeshPrimitives_[ent.offset] = staticMeshPrimitives_.back();
            int entIndex = staticMeshReverseMap_[staticMeshPrimitives_.size() - 1];
            staticMeshPrimitives_.pop_back();
            staticMeshReverseMap_.pop_back();
            ASSERT(entIndex >= 0 && entIndex < primitiveEntities_.size());
            primitiveEntities_[entIndex].offset = ent.offset;
            ent.offset = -1;
            freedEntities_.push_back(h);
        }
        break;
        case ShapeType::SPHERE:
        {
            ASSERT(ent.offset < spherePrimitives_.size());
            if (ent.offset >= spherePrimitives_.size()) return false;
            spherePrimitives_[ent.offset] = spherePrimitives_.back();
            int entIndex = sphereReverseMap_[spherePrimitives_.size() - 1];
            spherePrimitives_.pop_back();
            sphereReverseMap_.pop_back();
            ASSERT(entIndex >= 0 && entIndex < primitiveEntities_.size());
            primitiveEntities_[entIndex].offset = ent.offset;
            ent.offset = -1;
            freedEntities_.push_back(h);
        }
        break;
        default:
        {
            ASSERT(false);
            return false;
        }
        break;
        }
        return true;
    }
}