#include "ECSMgr.h"
#include <stdexcept>
#include "Util/base/containers/flat_map.h"

//通过原型创建Entity
template<typename _Archetype>
Sun::Entity Sun::ECSMgr::createEntity() {
	ArchetypeId id = registArchetypeT<_Archetype>();
	Entity entity = _createEntityByArchetype<_Archetype>(id);
	return entity;

}

template<typename _ComType>
_ComType* Sun::ECSMgr::getComponent(const Entity& entity) {
	const EntityInfo& info = entityTable_[entity.index];
	return info.archetypeMgr->getComponent<_ComType>(info);
}

template<typename _ComType>
void Sun::ECSMgr::setComponent(const  Sun::Entity& entity, const _ComType& com) {
	if (hasComponent(entity,com)) {
		EntityInfo& info = entityTable_[entity.index];
		if (info.archetypeMgr) {
			info.archetypeMgr->setComponent(info);
		}
	}
}

template<typename _ComType>
bool Sun::ECSMgr::hasComponent(const Sun::Entity& entity, const _ComType& com) const {
	if (!exist(entity)) {
		return false;
	}
	auto& info = entityTable_[entity.index];
	if (info.archetypeMgr == nullptr) return false;
	return info.archetypeMgr->hasComponent<_ComType>();
}

template<typename ... _ComType>
void Sun::ECSMgr::addComponent(const Sun::Entity& entity) {
	if (!exist(entity)) {
		return;
	}
	EntityInfo& info = entityTable_[entity.index];
	
	ArchetypeMgr* srcMgr = info.archetypeMgr;
	ArchetypeId srcId = info.archetypeMgr->getArchetypeId();
	size_t srcSiz = srcId.size();

	ArchetypeId dstId = srcId;
	ArchetypeId id = getArchetypeId<_ComType...>();
	dstId.insert(id.begin(), id.end());
	//说明新增加的组件都在原来entity都有，直接返回
	if (dstId.size() == srcSiz) {
		return;
	}

	//先尝试注册新组件
	registComs<_ComType...>();

	//注册新原型
	if (!checkArchetype(dstId)) {
		registArchetype(dstId);
	}

	//获取新原型mgr
	ArchetypeMgr* dstMgr = archeTypeMap_[dstId];
	EntityAddress dstAddr = dstMgr->requestEntry();
	EntityAddress srcAddr = info;
	for (ComTypeId& comTypeId : dstId) {
		const ComTypeInfo& info = getComTypeInfoById(comTypeId);
		//旧组件移动拷贝构造过来
		if (srcId.find(comTypeId) != srcId.end()) {
			ComAccessPtr ptr = dstMgr->getAccessPtr(comTypeId, dstAddr);
			ComAccessPtr optr = srcMgr->getAccessPtr(comTypeId, srcAddr);
			info.copy_constructor(ptr.get(), optr.get());
		}
		//新组件直接构造
		else {
			ComAccessPtr ptr = dstMgr->getAccessPtr(comTypeId, dstAddr);
			info.default_constructor(ptr.get());
		}
	}

	//释放原来的entity
	std::size_t movedEntityIndex = srcMgr->erase(srcAddr);
	if (movedEntityIndex != static_cast<std::size_t>(-1)) {
		entityTable_[movedEntityIndex].chunkIdx = srcAddr.chunkIdx;
		entityTable_[movedEntityIndex].idxInChunk = srcAddr.idxInChunk;
	}

	//修改引用
	info.chunkIdx = dstAddr.chunkIdx;
	info.idxInChunk = dstAddr.idxInChunk;
	info.archetypeMgr = dstMgr;

	return;
}

template<typename ... _ComType>
void Sun::ECSMgr::removeComponent(const Sun::Entity& entity) {
	if (!exist(entity)) {
		return;
	}
	EntityInfo& info = entityTable_[entity.index];

	ArchetypeMgr* srcMgr = info.archetypeMgr;
	ArchetypeId srcId = info.archetypeMgr->getArchetypeId();
	size_t srcSiz = srcId.size();

	ArchetypeId dstId = srcId;
	ArchetypeId id = getArchetypeId<_ComType...>();
	dstId.erase(id.begin(), id.end());
	//说明移除的组件在原组件中不存在
	if (dstId.size() == srcSiz) {
		return;
	}

	//注册新原型
	if (!checkArchetype(dstId)) {
		registArchetype(dstId);
	}

	//获取新原型mgr
	ArchetypeMgr* dstMgr = archeTypeMap_[dstId];
	EntityAddress dstAddr = dstMgr->requestEntry();
	EntityAddress srcAddr = info;
	for (ComTypeId& comTypeId : dstId) {
		const ComTypeInfo& info = getComTypeInfoById(comTypeId);

		ComAccessPtr ptr = dstMgr->getAccessPtr(comTypeId, dstAddr);
		ComAccessPtr optr = srcMgr->getAccessPtr(comTypeId, srcAddr);
		info.copy_constructor(ptr.get(), optr.get());

	}

	//释放原来的entity
	std::size_t movedEntityIndex = srcMgr->erase(srcAddr);
	if (movedEntityIndex != static_cast<std::size_t>(-1)) {
		entityTable_[movedEntityIndex].chunkIdx = srcAddr.chunkIdx;
		entityTable_[movedEntityIndex].idxInChunk = srcAddr.idxInChunk;
	}

	//修改引用
	info.chunkIdx = dstAddr.chunkIdx;
	info.idxInChunk = dstAddr.idxInChunk;
	info.archetypeMgr = dstMgr;

	return;
}

template<typename _Archetype>
size_t Sun::ECSMgr::getArchetypeEntityNum() const {
	bool has = hasArchetype<_Archetype>();
	if (!has) return 0;
	auto id = getArchetypeIdByArchetype<_Archetype>();
	auto it = archeTypeMap_.find(id);
	if (it == archeTypeMap_.end()) return 0;
	return it->second->numEntity_;
}

template<typename _Archetype>
Sun::ArchetypeIter<_Archetype> Sun::ECSMgr::begin() {
	if (!hasArchetype<_Archetype>()) {
		return Sun::ArchetypeIter<_Archetype>;
	}

	Sun::ArchetypeId id = getArchetypeIdByArchetype<_Archetype>();
	ArchetypeMgr* amgr = archeTypeMap_[id];
	ArchetypeIter<_Archetype> iter;

	iter.chunks = amgr->chunks_.data();
	iter.capacity = amgr->capacity_;
	//iter.offsets.

	return iter;
}

template<typename ... _ComType>
Sun::ArchetypeId Sun::ECSMgr::registArchetype() {
	//计算原型tag
	ArchetypeId tag = getArchetypeId<_ComType...>();

	//如果已经注册直接返回
	if (checkArchetype(tag)) {
		return tag;
	}

	//尝试注册每个组件
	registComs<_ComType...>();

	//注册
	registArchetype(tag);

	return tag;
}

//通过原型模板注册原型
template<typename _Archetype>
Sun::ArchetypeId Sun::ECSMgr::registArchetypeT() {
	//获取原型id
	ArchetypeId tag= getArchetypeIdByArchetype<_Archetype>();
	//如果该原型已经注册过，则直接返回
	if (archeTypeMap_.find(tag) != archeTypeMap_.end()) {
		return tag;
	}
	//注册该原型的所有组件
	using tuple_type = typename _Archetype::tuple_type;
	constexpr size_t siz = std::tuple_size_v<tuple_type>;
	auto seq = std::make_integer_sequence<size_t, siz>();
	seq_registComs<_Archetype>(seq);
	//真实注册原型
	ArchetypeMgr* mgr = new ArchetypeMgr(this, tag);
	archeTypeMap_.emplace(tag, std::unique_ptr<ArchetypeMgr>(mgr));
	return tag;
}

template<typename ... ComType>
bool Sun::ECSMgr::unRegistArchetype() {
	ArchetypeId id = getArchetypeId<ComType...>();
	auto it = archeTypeMap_.find(id);
	if (it == archeTypeMap_.end()) {
		return false;
	}
	archeTypeMap_.erase(it);

	return true;
}

template<typename _Archetype>
bool Sun::ECSMgr::hasArchetype() const {
	ArchetypeId id = getArchetypeIdByArchetype<_Archetype>();
	auto it = archeTypeMap_.find(id);
	if (it == archeTypeMap_.end()) {
		return false;
	}
	return true;
}

//一次性注册多个组件
template<typename ... _ComType>
void Sun::ECSMgr::registComs() {
	(registCom<_ComType>(), ...);
}

//注册单个组件
template<typename _ComType>
bool Sun::ECSMgr::registCom() {
#if 0
	if (ComTypeIndex<_ComType>::isRegist()) {
		return false;
	}

	ComTypeInfo func;
	regsitMemberFunc<_ComType>(func);
	comTypeInfos_.emplace_back(std::move(func));

	ComTypeIndex<_ComType>::setIndex(comTypeInfos_.size() - 1);
	comTypeIds_.push_back(&(ComTypeIndex<_ComType>::index_));
#endif

	auto id = getComTypeId<_ComType>();
	if (id < comIdValids_.size()) {
		if (comIdValids_[id] == true) {
			return false;
		}
	}

	if (comIdValids_.size() <= id) {
		comIdValids_.resize(id + 1);
	}
	comIdValids_[id] = true;

	ComTypeInfo func;
	regsitMemberFunc<_ComType>(func);
	func.siz = sizeof(_ComType);
	func.align_siz = sizeof(_ComType); //how to set this align? TODO

	if (comTypeInfos_.size() <= id) {
		comTypeInfos_.resize(id + 1);
	}
	comTypeInfos_[id] = func;
}

//取消注册多个组件
template<class ... _ComType>
void Sun::ECSMgr::unRegistComs() {
	(unRegistCom<_ComType>(), ...);
}

//取消注册单个组件
template<class _ComType>
void Sun::ECSMgr::unRegistCom() {
	//判断该组件是否已经注册，如果没有，直接返回
	if (ComTypeIndex<_ComType>::getIndex() < 0) {
		return;
	}
	//取消注册组件之前，取消注册所有包含该原型的
}

//创建Entity
template<typename ... _ComType>
Sun::Entity Sun::ECSMgr::_createEntity() {
	registArchetype<_ComType...>();
	Entity entity = createEntity<Archetype<_ComType...>>();
	return entity;
}

template<class _Archetype>
Sun::Entity Sun::ECSMgr::_createEntityByArchetype(const ArchetypeId& id) {

	std::unique_ptr<ArchetypeMgr>& archetypeMgr = archeTypeMap_[id];

	Entity entity = requestFreeEntity();
	EntityAddress address = archetypeMgr->createObject(entity);
	EntityInfo& info = entityTable_[entity.index];
	info.chunkIdx = address.chunkIdx;
	info.idxInChunk = address.idxInChunk;
	info.archetypeMgr = archetypeMgr.get();
	return entity;
}

template<typename ... _ComType>
size_t Sun::ECSMgr::_getArchetypeEntityNum() const {
	ArchetypeId tag = getArchetypeId<_ComType...>();
	auto it = archeTypeMap_.find(tag);
	if (it == archeTypeMap_.end()) return 0;
	return it->second->numEntity_;
}

template<typename ... ComType>
bool Sun::ECSMgr::_hasArchetype() const {
	ArchetypeId id = getArchetypeId<ComType...>();
	auto it = archeTypeMap_.find(id);
	if (it == archeTypeMap_.end()) {
		return false;
	}
	return true;
}


template<typename _Archetype, typename _T, size_t... N>
void Sun::ECSMgr::seq_registComs(std::integer_sequence<_T, N...> is) {
	(registCom<typename _Archetype::template type<N>>(), ...);
}

template<class _ComType>
void Sun::ECSMgr::regsitMemberFunc(Sun::MemberFunctions& func) {
	//注册构造函数
	if constexpr (std::is_default_constructible<_ComType>::value
		/*&& !std::is_trivially_constructible<_ComType>::value*/) {
		func.default_constructor = [](void* addr) {
			new(addr)_ComType();
		};
	}

	//注册拷贝构造函数
	if constexpr (std::is_copy_constructible<_ComType>::value
		/*&& !std::is_trivially_copy_constructible<_ComType>::value*/) {
		func.copy_constructor = [](void* dst, const void* src) {
			new(dst)_ComType(*static_cast<const _ComType*>(src));
		};
	}

	//注册移动拷贝构造函数
	if constexpr (std::is_move_constructible<_ComType>::value
		/*&& !std::is_trivially_move_constructible<_ComType>::value*/) {
		func.move_constructor = [](void* dst, void* src) {
			new(dst)_ComType(std::move(*static_cast<const _ComType*>(src)));
		};
	}

	//注册移动赋值
	if constexpr (std::is_move_assignable<_ComType>::value
		/*&& !std::is_trivially_move_assignable<_ComType>::value*/) {
		func.move_assignment = [](void* dst, void* src) {
			*static_cast<_ComType*>(dst) = std::move(*static_cast<_ComType*>(src));
		};
	}

	//注册析构函数
	if constexpr (std::is_destructible<_ComType>::value
		/*&& !std::is_trivially_destructible<_ComType>::value*/) {
		func.destructor = [](void* addr) {
			static_cast<_ComType*>(addr)->~_ComType();
		};
	}

}

template<typename _ComType>
Sun::ComTypeId Sun::ECSMgr::getComTypeId() {
	return UniqueTypeCounter::unique_type_id<_ComType>();
}

template<typename _ComType>
const Sun::ComTypeInfo& Sun::ECSMgr::getComTypeInfo() const {
	return getComTypeInfoById(getComTypeId<_ComType>());
}

//计算原型id
template<typename _ComType>
void Sun::ECSMgr::extractComTypeId(Sun::ArchetypeId& vec) const{
	//vec.emplace(ComTypeId::buildComTypeId<_ComType>());
	vec.emplace(UniqueTypeCounter::unique_type_id<_ComType>());
}

//计算原型id
template<typename ... _ComType>
Sun::ArchetypeId Sun::ECSMgr::getArchetypeId() {
	Sun::ArchetypeId output;
	(extractComTypeId<_ComType>(output), ...);
	//std::sort(output.begin(), output.end());
	return output;
}

template<typename _Archetype, typename _T, size_t... N>
void Sun::ECSMgr::extractArchetypeId(Sun::ArchetypeId& id, std::integer_sequence<_T, N...> seq) const{
	(extractComTypeId<typename _Archetype::template type<N>>(id), ...);
}

template<typename _Archetype>
Sun::ArchetypeId Sun::ECSMgr::getArchetypeIdByArchetype() const{
	using tuple_type = typename _Archetype::tuple_type;
	constexpr size_t siz = std::tuple_size_v<tuple_type>;
	auto seq = std::make_integer_sequence<size_t, siz>();
	ArchetypeId id;
	extractArchetypeId<_Archetype>(id, seq);
	return id;
}

template <typename C, std::enable_if_t<tf::is_condition_task_v<C>, void>*>
tf::Task Sun::ECSMgr::emplaceTask(C&& task) {
	return taskFlow_.emplace(task);
}
