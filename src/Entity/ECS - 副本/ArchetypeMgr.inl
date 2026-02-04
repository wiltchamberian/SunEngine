#include "ArchetypeMgr.h"

template<class _ComType>
bool Sun::ArchetypeMgr::hasComponent() {
	ComTypeId id = mgr_->getComTypeId<_ComType>();
	if (id_.count(id) > 0) return true;
	return false;
}

template<class _ComType>
_ComType* Sun::ArchetypeMgr::getComponent(const Sun::EntityAddress& addr) {
	ComTypeId id = mgr_->getComTypeId<_ComType>();
	const ComTypeInfo& info =mgr_->getComTypeInfoById(id);
	//assert(addr.chunkIdx >= 0 && addr.chunkIdx < chunks_.size());
	_ComType* res = nullptr;
	Chunk* chunk = chunks_[addr.chunkIdx];
	return (_ComType*)(chunk->data_ + offsets_[id] + addr.idxInChunk * info.align_siz);
}

template<class _ComType>
void Sun::ArchetypeMgr::addComponent(const _ComType& com, const Sun::EntityAddress& addr) {
	if (hasComponent<_ComType>()) return;

}

template<class _T>
void Sun::ArchetypeMgr::setComponent(const Sun::EntityAddress& info) {

}

template<class _ComType>
Sun::ComAccessPtr Sun::ArchetypeMgr::getAccessPtr(const Sun::EntityAddress& addr) {
	Sun::ComTypeId id = mgr_->getComTypeId<_ComType>();
	return getAccessPtr(id, addr);
}

template<class _ComType>
Sun::ComAccessPtr Sun::ArchetypeMgr::getAccessPtr(Chunk* chunk, size_t idxInChunk) {
	Sun::ComTypeId id = mgr_->getComTypeId<_ComType>();
	return getAccessPtr(id, chunk, idxInChunk);
}

template<typename ... _ComType>
Sun::EntityAccessPtr Sun::ArchetypeMgr::getChunkHeadEntityAccessPtr(Chunk* chunk) {
	Sun::EntityAccessPtr res;
	(res.addComponentPtr(getAccessPtr<_ComType>(chunk, 0)), ...);
	return res;
}