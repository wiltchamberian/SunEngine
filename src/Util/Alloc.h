/*****************************************************************************
* @brief : Alloc
* @author : acedtang
* 考虑到QuickVector需要搭配一个内存分配器为佳，为了保持设计一致性，实现
* 默认分配器，同时，考虑到内存池的使用，实现AllocProxy可以绑定到内存池指针
* @date : 2021/7/29
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __SUN_ALLOC_H
#define __SUN_ALLOC_H

#include <memory>

namespace Sun {

#define SUN_DYNALLOC inline

	// VARIABLE TEMPLATE _New_alignof
	template <class _Ty>
	_INLINE_VAR constexpr size_t _New_alignof = (_STD max)(alignof(_Ty),
		static_cast<size_t>(__STDCPP_DEFAULT_NEW_ALIGNMENT__) // TRANSITION, VSO-522105
		);

	template<class _T>
	class DefaultAlloc {
	public:
		using _From_primary = DefaultAlloc;
		using value_type = _T;
		using pointer = _T*;
		using const_pointer = const _T*;
		using reference = _T&;
		using const_reference = _T&;
		using size_type = size_t;
		using difference_type = ptrdiff_t;

		template<class _Other>
		struct rebind {
			using other = DefaultAlloc<_Other>;
		};

		_T* address(_T& _Val) const noexcept {
			return _STD addressof(_Val);
		}

		const _T* address(const _T& _Val) const noexcept {
			return _STD addressof(_Val);
		}

		DefaultAlloc() {
		}

		DefaultAlloc(const DefaultAlloc& other) noexcept {
		}

		template <class _Other>
		DefaultAlloc(const DefaultAlloc<_Other>& other) noexcept {
		}

		SUN_DYNALLOC ~DefaultAlloc() {

		}

		SUN_DYNALLOC DefaultAlloc& operator=(const DefaultAlloc& other) {
			return *this;
		}

		SUN_DYNALLOC void deallocate(_T* const _Ptr, const size_t _Count) {
			// no overflow check on the following multiply; we assume _Allocate did that check
			//_Deallocate<_New_alignof<_T>>(_Ptr, sizeof(_T) * _Count);
			//pool_->_free(_Ptr);
			::free(_Ptr);
		}

		SUN_DYNALLOC __declspec(allocator) _T* allocate(_CRT_GUARDOVERFLOW const size_t _Count) {
			return static_cast<_T*>(::malloc(sizeof(_T)*_Count));
		}


		//__declspec(allocator) _T* allocate(_CRT_GUARDOVERFLOW const size_t _Count, const void*) {
		//	return allocate(_Count);
		//}

		template <class _Objty, class... _Types>
		void construct(_Objty* const _Ptr, _Types&&... _Args) {
			::new ((_Ptr)) _Objty(_STD forward<_Types>(_Args)...);
		}


		template <class _Uty>
		void destroy(_Uty* const _Ptr) {
			_Ptr->~_Uty();
		}

		size_t max_size() const noexcept {
			return static_cast<size_t>(-1) / sizeof(_T);
		}

	};

	template<class _Alloc>
	class AllocProxy {
	public:
		AllocProxy(_Alloc* alloc) :alloc_(alloc) {}

	protected:
		_Alloc* alloc_;
	};
}


#endif