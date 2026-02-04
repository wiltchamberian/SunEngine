/*****************************************************************************
* @brief :List
* 高性能链表实现，相比stl::list优化了内存管理，使用连续内存存储，内置内存池
* TODO,未完成
* @author : acedtang
* @date : 2021/8/7
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __SUN_LIST_H
#define __SUN_LIST_H

#include <list>
#include "QuickMemoryPool.h"

namespace Sun {

#define SUN_CONTAINER inline

	template<class _T>
	class ListNode {
	public:
		template<class ... _Args>
		ListNode(_Args&&... args) :
			data(_STD forward<_Args>(args)...)
		{

		}
		_T data;
		int prev = 0;
		int next = 0;
	};

	template<class _List>
	class List_const_iterator {
	public:
		using value_type = typename _List::value_type;
		using difference_type = typename _List::difference_type;
		using pointer = typename _List::const_pointer;
		using reference = const value_type&;
		using _Tptr = typename _List::pointer;

		SUN_CONTAINER List_const_iterator() noexcept : ptr_() {}
		SUN_CONTAINER List_const_iterator(_Tptr parg) : ptr_(parg) {}

		SUN_CONTAINER List_const_iterator& operator=(const List_const_iterator&) = default;


		SUN_CONTAINER reference operator*() const {
			return *ptr_;
		}

		SUN_CONTAINER pointer operator->() const {
			return ptr_;
		}

		SUN_CONTAINER List_const_iterator& operator++() {
			ptr_ = ptr_->next;
			return *this;
		}

		SUN_CONTAINER List_const_iterator& operator++(int) {
			List_const_iterator tmp = *this;
			ptr_ = ptr_->next;
			return tmp;
		}

		SUN_CONTAINER List_const_iterator& operator--() {
			ptr_ = ptr_->prev;
			return *this;
		}

		SUN_CONTAINER List_const_iterator& operator--(int) {
			List_const_iterator tmp = *this;
			ptr_ = ptr_->prev;
			return tmp;
		}

		SUN_CONTAINER List_const_iterator& operator+=(const difference_type off) {
			for (difference_type i = 0; i < off; ++i) {
				ptr_ = ptr_->next;
			}
			return *this;
		}

		SUN_CONTAINER List_const_iterator operator+(const difference_type off)const {
			List_const_iterator tmp = *this;
			tmp += off;
			return tmp;
		}

		SUN_CONTAINER List_const_iterator& operator-=(const difference_type off) {
			for (difference_type i = 0; i < off; ++i) {
				ptr_ = ptr_->prev;
			}
			return *this;
		}

		SUN_CONTAINER List_const_iterator operator-(const difference_type off)const {
			List_const_iterator tmp = *this;
			tmp -= off;
			return tmp;
		}

		SUN_CONTAINER bool operator==(const List_const_iterator& _Right) const {
			return ptr_ == _Right.ptr_;
		}

		SUN_CONTAINER bool operator!=(const List_const_iterator& _Right) const {
			return !(ptr_ == _Right.ptr_);
		}

		_Tptr ptr_;
		_List* lis_;
	};

	template<class _List>
	class List_iterator : public List_const_iterator<_List> {
	public:
		using reference = typename _List::value_type&;
		using pointer = typename _List::value_type*;
		using _Mybase = List_const_iterator<_List>;
		reference operator*() const {
			return *_Mybase::ptr_;
		}

		pointer operator->() const {
			return _Mybase::ptr_;
		}

		SUN_CONTAINER List_iterator& operator++() {
			_Mybase::ptr_ = _Mybase::ptr_->next;
			return *this;
		}

		SUN_CONTAINER List_iterator operator++(int) {
			List_iterator _Tmp = *this;
			_Mybase::ptr_ = _Mybase::ptr_->next;
			return _Tmp;
		}

		SUN_CONTAINER List_iterator& operator--() {
			_Mybase::ptr_ = _Mybase::ptr_->prev;
			return *this;
		}

		SUN_CONTAINER List_iterator operator--(int) {
			List_iterator _Tmp = *this;
			_Mybase::ptr_ = _Mybase::ptr_->prev;
			return _Tmp;
		}

	};

	template<class _T>
	class List
	{
	public:
		using value_type = _T;
		using iterator = List_iterator<List<_T>>;
		using const_iterator = List_const_iterator<List<_T>>;
		using pointer = int;
		using difference_type = size_t;
		using const_pointer = const ListNode<_T>*;

		List() : first(nullptr){

		}

		template<class ... _Args>
		ListNode<_T>* emplace_back(_Args&&... args) {
			ListNode<_T>* node = pool.malloc(args);
			push_back(node);
			return node;
		}

		ListNode<_T>* push_back(const _T& t) {
			ListNode<_T>* node = pool.malloc(t);
			if (first) {
				if (first->prev) {
					first->prev->next = node;
				}
				node->prev = first->prev;
				first->prev = node;
				node->next = first;
			}
			else {
				first = node;
				first->next = first;
				first->prev = first;
			}
			return node;
		}

		void erase(_T* t) {
			ListNode* li = (ListNode*)t;
			if (li->prev) {
				li->prev->next = li->next;
			}
			else {
				first = li->next;
			}
			if (li->next) {
				li->next->prev = li->prev;
			}
			pool.free(li);
		}

		iterator insert(iterator position, const value_type& val) {

		}

		_T& back() {
			return first->prev;
		}

		/**********************iterators*************************/
		SUN_CONTAINER iterator begin() {
			return iterator(first);
		}

		SUN_CONTAINER const_iterator begin() const {
			return const_iterator(first);
		}

		SUN_CONTAINER iterator end() {
			return iterator(first);
		}

		SUN_CONTAINER const_iterator end() const {
			return const_iterator(first);
		}

	protected:
		//链表首部
		ListNode<_T>* first = nullptr;
		QuickMemoryPool<ListNode<_T>> pool;
		//std::vector<ListNode<_T>> datas_;

		std::vector<int> freeSlots_;
	};
}

#endif