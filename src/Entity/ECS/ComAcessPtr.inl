#include "ComAccessPtr.h"

template<class _ComType>
Sun::ComAccessPtr getEntityAccessPtrComponent(const Sun::EntityAccessPtr& ptr, Sun::ECSMgrBase* mgr) {
	return ptr.getComponentPtr<_ComType>(mgr);
}

template<class _ComType>
_ComType* getEntityAccessPtrComPtr(const Sun::EntityAccessPtr& ptr, Sun::ECSMgr* mgr) {
	Sun::ComAccessPtr comPtr = ptr.getComponentPtr<_ComType>(mgr);
	return comPtr.as<_ComType>();
}

template<class _ComType>
Sun::ComAccessPtr Sun::EntityAccessPtr::getComponentPtr(Sun::ECSMgrBase* mgr) const {
	Sun::ComTypeId id = mgr->getComTypeId<_ComType>();
	auto it = ptrs_.find(id);
	if (it != ptrs_.end())return it->second;
	return Sun::ComAccessPtr();
}

template<size_t I>
Sun::ComAccessPtr getComAccessPtr(const Sun::EntityAccessPtr& ptr, Sun::ECSMgrBase* mgr) {
	return ptr.getComAccessPtr<I>();
}

template<typename _ComType, size_t I>
_ComType* getComPtr(const Sun::EntityAccessPtr& ptr) {
	return ptr.getComAccessPtr<I>().as<_ComType>();
}
