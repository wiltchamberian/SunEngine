#include "ArchetypeMgr2.h"


template<class _ComType>
void Sun::ArchetypeMgr2::addComponent(const _ComType& com, const Sun::EntityAddress& addr) {
	if (hasComponent<_ComType>()) return;

}

template<class _T>
void Sun::ArchetypeMgr2::setComponent(const Sun::EntityAddress& info) {

}

template<class _ComType>
Sun::ComAccessPtr Sun::ArchetypeMgr2::getAccessPtr(const Sun::EntityAddress& addr) {
	Sun::ComTypeId id = mgr_->getComTypeId<_ComType>();
	return getAccessPtr(id, addr);
}

template<class _ComType>
Sun::ComAccessPtr Sun::ArchetypeMgr2::getAccessPtr(Chunk* chunk, size_t idxInChunk) {
	Sun::ComTypeId id = mgr_->getComTypeId<_ComType>();
	return getAccessPtr(id, chunk, idxInChunk);
}

template<typename ... _ComType>
Sun::EntityAccessPtr Sun::ArchetypeMgr2::getChunkHeadEntityAccessPtr(Chunk* chunk) {
	Sun::EntityAccessPtr res;
	(res.addComponentPtr(getAccessPtr<_ComType>(chunk, 0)), ...);
	return res;
}