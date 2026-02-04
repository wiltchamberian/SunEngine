#include "ECSMgr.h"
#include "ECSGlobal.h"


namespace Sun {

	ECSMgr::ECSMgr() {
		memoryPool_ = new ECSMemoryPool();
	}

	ECSMgr::~ECSMgr() {
		delete memoryPool_;
	}

	//销毁entity
	void ECSMgr::destroyEntity(Entity entity) {
		if (!exist(entity)) {
			throw std::invalid_argument("Entity is invalid!");
			return;
		}
		EntityInfo& info = entityTable_[entity.index];
		ArchetypeMgr* mgr = info.archetypeMgr;

		size_t movedEntityIndex = mgr->erase(info);
		//重定位entiyMap
		if (movedEntityIndex != static_cast<size_t>(-1)) {
			entityTable_[movedEntityIndex].chunkIdx = info.chunkIdx;
			entityTable_[movedEntityIndex].idxInChunk = info.idxInChunk;
		}

		recycleEntityEntry(entity);
	}

	EntityInfo& ECSMgr::getEntityInfo(const Entity& entity) {
		if (!exist(entity)) return EntityInfo();
		return entityTable_[entity.index];
	}

	size_t ECSMgr::getTotalEntityNum() const {
		return entityTable_.size() - freeEntries_.size();
	}

	//通过archetypeId注册原型
	void ECSMgr::registArchetype(const ArchetypeId& id) {
		//注册
		ArchetypeMgr* mgr = new ArchetypeMgr(this, id);
		archeTypeMap_.emplace(id, std::unique_ptr<ArchetypeMgr>(mgr));
	}

	size_t ECSMgr::getArchetypeNum() const {
		return archeTypeMap_.size();
	}

	SVector<ArchetypeMgr*> ECSMgr::filterArchetype(const ArchetypeFilter& filter) const {
		SVector<ArchetypeMgr*> res;
		for (auto& it : archeTypeMap_) {
			bool cond1 = true;
			for (auto& id : filter.all) {
				if (it.first.find(id) == it.first.end()) {
					cond1 = false;
					break;
				}
			}
			bool cond2 = true;
			for (auto& id : filter.any) {
				cond2 = false;
				if (it.first.find(id) != it.first.end()) {
					cond2 = true;
					break;
				}
			}
			bool cond3 = true;
			for (auto& id : filter.none) {
				if (it.first.find(id) != it.first.end()) {
					cond3 = false;
					break;
				}
			}
			if (cond1 && cond2 && cond3) {
				res.emplace_back(it.second.get());
			}
		}
		return res;
	}

	ArchetypeId ECSMgr::getArchetypeId(const Entity& entity) const {
		if (!exist(entity))return ArchetypeId();
		assert(entityTable_[entity.index].archetypeMgr);
		return entityTable_[entity.index].archetypeMgr->id_;
	}

	bool ECSMgr::checkArchetype(const ArchetypeId& id) const {
		if (archeTypeMap_.find(id) != archeTypeMap_.end()) {
			return true;
		}
		return false;
	}

	void ECSMgr::recycleEntityEntry(Entity entity) {
		assert(exist(entity));

		auto& entry = entityTable_[entity.index];
		entry.archetypeMgr = nullptr;
		entry.chunkIdx = static_cast<std::size_t>(-1);
		entry.idxInChunk = static_cast<std::size_t>(-1);
		++entry.version;

		freeEntries_.push_back(entity.index);
	}

	bool ECSMgr::exist(const Entity& e) const {
		return e.index >= 0 && e.index < entityTable_.size() && e.version == entityTable_[e.index].version;
	}

	Entity ECSMgr::requestFreeEntity() {
		Entity entity;
		if (freeEntries_.empty()) {
			std::size_t index = entityTable_.size();
			entityTable_.emplace_back();

			entity.index = index;
			entity.version = entityTable_[index].version;
			return entity;
		}
		std::size_t entry = freeEntries_.back();
		freeEntries_.pop_back();

		entity.index = entry;
		entity.version = entityTable_[entry].version;
		return entity;
	}

	const ComTypeInfo& ECSMgr::getComTypeInfoById(const ComTypeId& id) {
		//size_t index = id.getIndex();
		//assert(index >= 0 && index < comTypeInfos_.size());
		//return comTypeInfos_[index];

		return comTypeInfos_[id];
	}

	void ECSMgr::addPackage(const TaskPackage& package) {
		packages_[package.chunk].push_back(package);
	}

	void ECSMgr::runAllTasks() {
		for (auto& it : packages_) {
			if (it.second.empty()) continue;
			//作用于相同chunk的task,添加依赖关系
			tf::Task pre = taskFlow_.emplace(it.second[0]);
			for (size_t i = 1; i < it.second.size()-1; ++i) {
				tf::Task t = taskFlow_.emplace(it.second[i]);
				pre.precede(t);
				pre = t;
			}
		}
		executor_.run(taskFlow_).wait();
		taskFlow_.clear();
	}
}
