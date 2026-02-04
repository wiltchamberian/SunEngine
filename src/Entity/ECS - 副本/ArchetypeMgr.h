/*****************************************************************************
* @brief : ArchetypeMgr
* 对单个原型以及相关数据进行管理的类
* @author : acedtang
* @date : 2021/8/19
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __SUN_ECS_ARCHETYPE_MGR_H
#define __SUN_ECS_ARCHETYPE_MGR_H

#include <vector>
#include "Chunk.h"
//#include "Entity.h"
#include "ComTypeId.h"
#include "ComAccessPtr.h"
#include "Util/base/containers/flat_map.h"

namespace Sun {

	//template<class _T>
	//using SVector = std::vector<_T>;

	class ObjectInfo {
		int chunkId_;
		int offset_;
	};

	class Entity;
	class ECSMgr;

	class ArchetypeMgr {
	public:
		~ArchetypeMgr();

		friend class ECSMgr;
		//ArchetypeMgr(const SVector<TypeExtraction::type>& vec);
		ArchetypeMgr(ECSMgr* mgr, const ArchetypeId& id);

		//接收entity标签，创建object
		EntityAddress createObject(Entity entity);

		//删除object
		size_t erase(const EntityAddress& addr);

		EntityAddress requestEntry();

		ArchetypeId getArchetypeId() const;

		ComAccessPtr getAccessPtr(const ComTypeId& id, const EntityAddress& addr);
		template<class _ComType>
		ComAccessPtr getAccessPtr(const EntityAddress& addr);

		ComAccessPtr getAccessPtr(const ComTypeId& id, Chunk* chunk, size_t idxInChunk);
		template<class _ComType>
		ComAccessPtr getAccessPtr(Chunk* chunk, size_t idxInChunk);

		template<typename ... _ComType>
		EntityAccessPtr getChunkHeadEntityAccessPtr(Chunk* chunk);

		template<class _T>
		bool hasComponent();

		template<class _T>
		_T* getComponent(const EntityAddress& addr);

		template<class _ComType>
		void addComponent(const _ComType& com, const EntityAddress& addr);

		template<class _T>
		void setComponent(const EntityAddress& info);

		ComTypeInfo getComponentInfo(const ComTypeId& id);

		SVector<Chunk*>& getChunks();
	protected:
		void assignObject(const EntityAddress& addr);
		inline bool validation(const EntityAddress& addr) const;
		inline Chunk* applyChunk();
		inline void moveEntity(void* dst, void* src);

		ECSMgr* mgr_;

		//base::flat_map<size_t, size_t> index_to_offset_;
		

		////该原型对应的组件在mgr_中的索引集合
		//SVector<size_t> comTypeIndexes_;
		// 
		////组件在object内部的偏移量，长度与types_相同
		//SVector<size_t> offsets_;

		//当前原型中所有entity的个数
		int numEntity_ = 0;

		SVector<Chunk*> chunks_;
		//始终指向最小索引的未满chunk，如果不存在，则为-1
		int indexToChunk_ = -1;
		
		
		//SVector<size_t> freeChunks_;

		/*********这里记录索引组件需要的信息***********/
		//该原型中任意chunk所能容纳的最大object个数
		size_t capacity_ =0;
		//该原型中单个object占用的字节数(含align)
		size_t objectSize_ =0;
		//entity 偏移量始终为0
		const size_t entityOffset_ = 0;
		//eneity align size
		size_t entityAlignSiz_ = 0;

		ArchetypeId id_;
		//和ArchetypeId一一对应，每个组件起始位置在chunk中的偏移量
		base::flat_map<ComTypeId, size_t> offsets_;
	};

}

#endif