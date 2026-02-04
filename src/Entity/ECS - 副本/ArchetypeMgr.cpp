#include "ArchetypeMgr.h"
#include <cassert>
#include "Entity.h"
#include "ECSMgr.h"
#include "Util/Byte.h"

namespace Sun {

	//Archetype::Archetype(const SVector<TypeExtraction::type>& vec)
	//	:types_(vec)
	//{


	//}

	ArchetypeMgr::~ArchetypeMgr() {
		for (auto& id : id_) {
			ComTypeInfo info = getComponentInfo(id);
			for (auto& it : chunks_) {
				for (size_t i = 0; i < it->numEntity_; ++i) {
					info.destructor(it->data_ + offsets_[id] + i * info.align_siz);
				}
			}
		}
		for (auto& it : chunks_) {
			delete it;
		}
	}

	ArchetypeMgr::ArchetypeMgr(ECSMgr* mgr, const ArchetypeId& vec)
		:mgr_(mgr)
	{
		id_ = vec;
		size_t obj_siz = 0;
		for (auto& it : vec) {
			ComTypeInfo info = getComponentInfo(it);
			objectSize_ += info.align_siz;
		}
		//将entity的size也考虑进去
		entityAlignSiz_ = Align(sizeof(Entity), SUN_ECS_ALIGNMENT);
		objectSize_ += entityAlignSiz_;
		if(objectSize_>0)
			capacity_ = SUN_CHUNK_SIZE / objectSize_;
		//计算偏移量
		size_t offset = entityAlignSiz_ *capacity_;
		for (ComTypeId& it : id_) {
			offsets_[it] = offset;
			ComTypeInfo info = getComponentInfo(it);
			offset += info.align_siz * capacity_;
		}

	}

	EntityAddress ArchetypeMgr::createObject(Entity entity) {
		EntityAddress res = requestEntry();

		//在chunk的槽位上分配对象
		assignObject(res);

		numEntity_ += 1;
		return res;
	}

	ComTypeInfo ArchetypeMgr::getComponentInfo(const ComTypeId& id) {
		return mgr_->getComTypeInfoById(id);
	}

	SVector<Chunk*>& ArchetypeMgr::getChunks() {
		return chunks_;
	}

	void ArchetypeMgr::assignObject(const EntityAddress& addr) {
		Chunk* chunk = chunks_[addr.chunkIdx];
		for (ComTypeId& it : id_) {
			const ComTypeInfo& info = mgr_->getComTypeInfoById(it);
			size_t comOffset = offsets_[it] + info.align_siz * addr.idxInChunk;
			if (info.default_constructor)
				info.default_constructor(chunk->data_ + comOffset);
		}
	}

	bool ArchetypeMgr::validation(const EntityAddress& info) const {
		return true;
	}

	Chunk* ArchetypeMgr::applyChunk() {
		return mgr_->memoryPool_->mallocChunk();
	}

	void ArchetypeMgr::moveEntity(void* dst, void* src) {
		Entity* ds = (Entity*)(dst);
		Entity* sr = (Entity*)(src);
		*ds = *sr;
		sr->~Entity();
	}

	size_t ArchetypeMgr::erase(const EntityAddress& addr) {
		//表示被移动的entity索引
		size_t res = static_cast<size_t>(-1);
		//check
		if (!validation(addr)) return res;

		Chunk* chunk = chunks_[addr.chunkIdx];
		//在chunk上调用析构函数
		ComTypeInfo info;
		for (auto& it:id_) {
			info = getComponentInfo(it);
			void* com = chunk->data_ + offsets_[it] + addr.idxInChunk * info.align_siz;
			info.destructor(com);
		}
		//被移动的chunk
		Chunk* srcChunk = chunks_.back();
		//说明待删除的entity不是 最后一个chunk的最后一个entity ,则需要进行移动操作
		if (addr.chunkIdx != chunks_.size()-1 || addr.idxInChunk < chunk->numEntity_ - 1) {
			//被移动的chunk
			for (auto& it : id_) {
				info = getComponentInfo(it);
				void* dst = chunk->data_ + offsets_[it] + addr.idxInChunk * info.align_siz;
				void* src = srcChunk->data_ + offsets_[it] + (srcChunk->numEntity_ - 1) * info.align_siz;
				if (info.destructor) {
					info.destructor(dst);
				}
				if (info.move_constructor) {
					info.move_constructor(dst, src);
				}
				else if (info.copy_constructor) {
					info.copy_constructor(dst, src);
				}
				if (info.destructor) {
					info.destructor(src);
				}
			}
			//entity移动
			void* dst = chunk->data_ + entityOffset_ + addr.idxInChunk * entityAlignSiz_;
			void* src = srcChunk->data_ + entityOffset_ + (srcChunk->numEntity_ - 1) * entityAlignSiz_;
			moveEntity(dst, src);
			//更新索引
			Entity* entity = (Entity*)(dst);
			EntityInfo& info = mgr_->getEntityInfo(*entity);
			info.chunkIdx = addr.chunkIdx;
			info.idxInChunk = addr.idxInChunk;

			//赋值被移动的entity的索引
			res = entity->index;
		}
		else {
			//析构所有组件
			for (auto& it : id_) {
				info = getComponentInfo(it);
				void* dst = chunk->data_ + offsets_[it] + addr.idxInChunk * info.align_siz;
				if (info.destructor) {
					info.destructor(dst);
				}
			}
		}
		--srcChunk->numEntity_;
		if (srcChunk->numEntity_ == 0) {
			mgr_->memoryPool_->freeChunk(chunk);
			chunks_.pop_back();
		}

		//原型的entity数减去1
		--numEntity_;

		//返回被移动的entity索引，如果没有被移动则返回-1
		return res;
	}

	EntityAddress ArchetypeMgr::requestEntry() {
		EntityAddress res;
		Chunk* chunk = nullptr;
		//只有可能最后一个chunk不满，如果其不满则从它分配
		if (chunks_.size()>0 && chunks_.back()->numEntity_<capacity_) {
			chunk = chunks_.back();
			res.chunkIdx = chunks_.size() - 1;
#if 0  //deprecated
			if (chunk->numEntity_ +1 == capacity_) {
				//查找新的索引最小的未满chunks
				indexToChunk_ = -1;
				for (int i = 0; i < chunks_.size(); ++i) {
					if (chunks_[i] != nullptr && chunks_[i]->numEntity_ < capacity_) {
						indexToChunk_ = i;
						break;
					}
				}
			}
#endif
		}
		//所有chunk都已经满了
		else {
			chunk = applyChunk();
			chunks_.emplace_back(chunk);
			res.chunkIdx = chunks_.size() - 1;
#if 0 //deprecated
			indexToChunk_ = chunks_.size() - 1;
			if (capacity_ == 1) {
				indexToChunk_ = static_cast<size_t>(-1);
			}
#endif
		}
		res.idxInChunk = chunk->numEntity_;
		chunk->numEntity_ += 1;

		return res;
	}

	ArchetypeId ArchetypeMgr::getArchetypeId() const {
		return id_;
	}

	ComAccessPtr ArchetypeMgr::getAccessPtr(const ComTypeId& id, const EntityAddress& addr) {
		size_t offset = offsets_[id];
		Chunk* chunk = chunks_[addr.chunkIdx];
		size_t align_siz = getComponentInfo(id).align_siz;
		std::uint8_t* ptr = chunk->data_ + offset + align_siz * addr.idxInChunk;
		return ComAccessPtr(id, align_siz, ptr);
	}

	ComAccessPtr ArchetypeMgr::getAccessPtr(const ComTypeId& id, Chunk* chunk, size_t idxInChunk) {
		size_t offset = offsets_[id];
		size_t align_siz = getComponentInfo(id).align_siz;
		std::uint8_t* ptr = chunk->data_ + offset + align_siz * idxInChunk;
		return ComAccessPtr(id, align_siz, ptr);
	}
}