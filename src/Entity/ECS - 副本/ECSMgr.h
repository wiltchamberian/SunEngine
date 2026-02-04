/*****************************************************************************
* @brief : ECSMgr
* ECS架构的中心类，管理所有信息实体，组件，原型，系统等
* ecs架构说明：
* 主要组成部分:
* 1,ECSMgr 2,Archetype 3,Chunk, 4,Entity 
* ECSMgr管理所有一切
* 其中包含多个ArchetypeMgr,一个ArchetypeMgr管理相同原型的一组Chunk
* 每个ArchetypeMgr对应多个Chunk,Chunk用来存储组件数据
* ECSMgr中存有一个EntityTable,这里记录着每个Entity对应的Archtype*和Chunk索引；
* 通过这个索引可以找到关联的组件信息；
* 而Entity本身可以看成一个句柄，其存储着EntityTable的索引。
* Entity先索引到EntityTable的节点，再通过该节点索引到组件信息 （2级索引)
* (采用二级索引的原因是ECS架构下组件可能发生移动，从而导致1级索引失效)
* 
* 示意图:
* EntityTable:[entityInfo0 ,entityInfo1,..........]
* ArchetypeMgr0 ,   ArchetypeMgr1,...
* ArchetypeMgr0 : Chunk0  Chunk1, ... ,Chunk_n...
* Chunk:[                         ] a fix length buffer   component are in chunk
* with a SOA format
* entityInfo save a pointer to  ArchetypeMgr
* @author : acedtang
* @date : 2021/8/19
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __SUN_ECS_MGR_H
#define __SUN_ECS_MGR_H

#include <functional>
#include <type_traits>
#include <cstdint>
#include <algorithm>
#include <typeinfo>
#include <vector>
#include <memory>
#include <unordered_map>
#include "Archetype.h"
#include "ArchetypeMgr.h"
#include "Entity.h"
#include "ComAccessPtr.h"
#include "../ECS2/ArchetypeIter.h"
#include "TaskPackage.h"
#include "ECSGlobal.h"
#include "ComTypeId.h"
#include "Util/Hash.h"
#include "Util/Util.h"
#include "ECSMemoryPool.h"
#include "ThirdParty/taskflow/taskflow.hpp"

namespace Sun {

	class Entity;

	struct ArchetypeIdHash {
		std::size_t operator()(const ArchetypeId& id) const noexcept{
			return hash_combine_c(id.begin(), id.end());
		}
	};

	class ECSMgr
	{
	public:
		friend class ArchetypeMgr;

		ECSMgr();
		~ECSMgr();

		//通过原型创建Entity
		template<typename _Archetype>
		Entity createEntity();

		//销毁entity
		void destroyEntity(Entity entity);

		EntityInfo& getEntityInfo(const Entity& entity);

		template<typename _ComType>
		_ComType* getComponent(const Entity& entity);

		template<typename _ComType>
		void setComponent(const Entity& entity, const _ComType& cmpt);

		template<typename _ComType>
		bool hasComponent(const Entity& entity, const _ComType& cmpt) const;

		template<typename ... _ComType>
		void addComponent(const Entity& entity);

		template<typename ... _ComType>
		void removeComponent(const Entity& entity);

		template<typename ... _ComType>
		size_t _getArchetypeEntityNum() const;

		template<typename _Archetype>
		size_t getArchetypeEntityNum() const;

		template<typename _Archetype>
		ArchetypeIter<_Archetype> begin();

		template<typename ... _ComType>
		SVector<Entity> getArchetypeEntities() const;

		size_t getTotalEntityNum() const;

		template<typename ... _ComType>
		ArchetypeId registArchetype();

		//通过原型模板注册原型
		template<typename _Archetype>
		ArchetypeId registArchetypeT();

		//通过archetypeId注册原型
		void registArchetype(const ArchetypeId& id);

		template<typename ... ComType>
		bool unRegistArchetype();

		template<typename ... ComType>
		bool _hasArchetype() const;

		template<typename _Archetype>
		bool hasArchetype() const;

		size_t getArchetypeNum() const;

		SVector<ArchetypeMgr*> filterArchetype(const ArchetypeFilter& filter) const;

		template<typename _ComType>
		ComTypeId getComTypeId();

		template<typename _ComType>
		const ComTypeInfo& getComTypeInfo() const;

		//计算原型id
		template<typename _ComType>
		void extractComTypeId(ArchetypeId& vec) const;

		//计算原型id
		template<typename ... _ComType>
		ArchetypeId getArchetypeId();

		ArchetypeId getArchetypeId(const Entity& entity) const;

		template<typename _Archetype, typename _T, size_t... N>
		void extractArchetypeId(ArchetypeId& id, std::integer_sequence<_T, N...> seq) const;

		template<typename _Archetype>
		ArchetypeId getArchetypeIdByArchetype() const;

	protected:
		//检查原型是否存在
		bool checkArchetype(const ArchetypeId& id) const;

		void recycleEntityEntry(Entity entity);

		//一次性注册多个组件
		template<typename ... _ComType>
		void registComs();

		//注册单个组件
		template<typename _ComType>
		bool registCom();

		//取消注册多个组件
		template<class ... _ComType>
		void unRegistComs();

		//取消注册单个组件
		template<class _ComType>
		void unRegistCom();

		//创建Entity
		template<typename ... _ComType>
		Entity _createEntity();

		template<class _Archetype>
		Entity _createEntityByArchetype(const ArchetypeId& id);

		template<typename _Archetype, typename _T, size_t... N>
		void seq_registComs(std::integer_sequence<_T, N...> is);

		bool exist(const Entity& e) const;

		template<class _ComType>
		void regsitMemberFunc(MemberFunctions& func);

		Entity requestFreeEntity();

		const ComTypeInfo& getComTypeInfoById(const ComTypeId& id);

		void addPackage(const TaskPackage& package);

		template <typename C, std::enable_if_t<tf::is_condition_task_v<C>, void>*>
		tf::Task emplaceTask(C&& task);

		void runAllTasks();
	protected:
		//SVector<Archetype_t*>  objTypeIndexes_;
		//SVector<ArchetypeMgr*> archetypeMgrs_;

		std::unordered_map<ArchetypeId, std::unique_ptr<ArchetypeMgr>, ArchetypeIdHash> archeTypeMap_;
		
		std::vector<EntityInfo> entityTable_;
		std::vector<std::size_t> freeEntries_;

		//ComTypeIndex_t*指向某个全局的ComTypeIndex_t对象，该对象存储着
		//注册的某个组件类型的唯一id，通过ComTypeIndex<_ComType>::index可以访问到
		//组件_ComType对应的id;  
		//同时这个id可直接作为functions_数组的索引，索引该组件关联的函数
		//SVector<ComTypeId> comTypeIds_;
		SVector<bool> comIdValids_;
		SVector<ComTypeInfo> comTypeInfos_;

		ECSMemoryPool* memoryPool_ = nullptr;

		//UniqueTypeCounter comTypeCounter_;

		tf::Taskflow taskFlow_;
		tf::Executor executor_;

		std::map<const Chunk*, SVector<TaskPackage>> packages_;
	};

}

#include "ECSMgr.inl"
#include "Entity.inl"
#include "ArchetypeMgr.inl"
#include "ComAcessPtr.inl"

#endif