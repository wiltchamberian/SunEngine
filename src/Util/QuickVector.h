/*****************************************************************************
* @brief : QuickVector
* @author : acedtang
* 由于stl的vector封装的有些重，导致性能不理想，和轻量级容器在100万量级下
* 简单push_back和pop_back操作时间差达到10倍以上，再加上不便于扩展
* 特此实现quickVector，接口与stl vector尽可能保持一致
* 以达到兼容stl目的，方便熟悉stl的调用者使用;
* 百万量级不reserve速度是vector两倍，reserve达到7,8倍
* @date : 2021/7/29
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __QUICK_VECTOR_H
#define __QUICK_VECTOR_H

#include <vector>
#include <utility>
#include <cassert>
#include "Alloc.h"

namespace Sun {

#define SUN_CONTAINER inline

#define DEFAULT_GROWTH_FACTOR 3/2   //QuickVector默认增长系数

	template<class _Myvec>
	class _QuickVector_const_iterator{
	public:
		using value_type = typename _Myvec::value_type;
		using difference_type = typename _Myvec::difference_type;
		using pointer = typename _Myvec::const_pointer;
		using reference = const value_type&;
		using _Tptr = typename _Myvec::pointer;

		SUN_CONTAINER _QuickVector_const_iterator() noexcept : ptr_(){}
		SUN_CONTAINER _QuickVector_const_iterator(_Tptr parg): ptr_(parg){}

		SUN_CONTAINER _QuickVector_const_iterator& operator=(const _QuickVector_const_iterator&) = default;


		SUN_CONTAINER reference operator*() const {
			return *ptr_;
		}

		SUN_CONTAINER pointer operator->() const {
			return ptr_;
		}

		SUN_CONTAINER _QuickVector_const_iterator& operator++() {
			++ptr_;
			return *this;
		}

		SUN_CONTAINER _QuickVector_const_iterator& operator++(int) {
			_QuickVector_const_iterator tmp = *this;
			++ptr_;
			return tmp;
		}

		SUN_CONTAINER _QuickVector_const_iterator& operator--() {
			--ptr_;
			return *this;
		}

		SUN_CONTAINER _QuickVector_const_iterator& operator--(int) {
			_QuickVector_const_iterator tmp = *this;
			--ptr_;
			return tmp;
		}

		SUN_CONTAINER _QuickVector_const_iterator& operator+=(const difference_type off) {
			ptr_ += off;
			return *this;
		}

		SUN_CONTAINER _QuickVector_const_iterator operator+(const difference_type off)const {
			_QuickVector_const_iterator tmp = *this;
			tmp += off;
			return tmp;
		}

		SUN_CONTAINER _QuickVector_const_iterator& operator-=(const difference_type off) {
			ptr_ -= off;
			return *this;
		}

		SUN_CONTAINER _QuickVector_const_iterator operator-(const difference_type off)const {
			_QuickVector_const_iterator tmp = *this;
			tmp -= off;
			return tmp;
		}

		SUN_CONTAINER difference_type operator-(const _QuickVector_const_iterator& right) const {
			return ptr_ - right.ptr_;
		}

		SUN_CONTAINER reference operator[](const difference_type _Off) const  {
			return *(*this + _Off);
		}

		SUN_CONTAINER bool operator==(const _QuickVector_const_iterator& _Right) const  {
			return ptr_ == _Right.ptr_;
		}

		SUN_CONTAINER bool operator!=(const _QuickVector_const_iterator& _Right) const {
			return !(ptr_ == _Right.ptr_);
		}

		SUN_CONTAINER bool operator<(const _QuickVector_const_iterator& _Right) const {
			return ptr_ < _Right.ptr_;
		}

		SUN_CONTAINER bool operator>(const _QuickVector_const_iterator& _Right) const {
			return _Right.ptr_ < ptr_;
		}

		SUN_CONTAINER bool operator<=(const _QuickVector_const_iterator& _Right) const {
			return !(_Right.ptr_ < ptr_);
		}

		SUN_CONTAINER bool operator>=(const _QuickVector_const_iterator& _Right) const {
			return !(ptr_ < _Right.ptr_);
		}

		_Tptr ptr_;
	};

	

	template<class _Myvec>
	class _QuickVector_iterator: public _QuickVector_const_iterator<_Myvec> {
	public:
		using _Mybase = _QuickVector_const_iterator<_Myvec>;
		using iterator_category = std::random_access_iterator_tag;
		using value_type = typename _Myvec::value_type;
		using difference_type = typename _Myvec::difference_type;
		using pointer = typename _Myvec::pointer;
		using reference = value_type&;

		using _Tptr = typename _Myvec::pointer;

		SUN_CONTAINER _QuickVector_iterator() {}

		SUN_CONTAINER _QuickVector_iterator(_Tptr _Parg):_Mybase(_Parg) {}

		SUN_CONTAINER _QuickVector_iterator& operator=(const _QuickVector_iterator&) noexcept = default;

		SUN_CONTAINER reference operator*() const {
			return const_cast<reference>(_Mybase::operator*());
		}

		SUN_CONTAINER pointer operator->() const {
			return this->ptr_;
		}

		SUN_CONTAINER _QuickVector_iterator& operator++() noexcept {
			_Mybase::operator++();
			return *this;
		}

		SUN_CONTAINER _QuickVector_iterator operator++(int) noexcept {
			_QuickVector_iterator _Tmp = *this;
			_Mybase::operator++();
			return _Tmp;
		}

		SUN_CONTAINER _QuickVector_iterator& operator--() noexcept {
			_Mybase::operator--();
			return *this;
		}

		SUN_CONTAINER _QuickVector_iterator operator--(int) noexcept {
			_QuickVector_iterator _Tmp = *this;
			_Mybase::operator--();
			return _Tmp;
		}

		SUN_CONTAINER _QuickVector_iterator& operator+=(const difference_type _Off) noexcept {
			_Mybase::operator+=(_Off);
			return *this;
		}

		SUN_CONTAINER _QuickVector_iterator operator+(const difference_type _Off) const noexcept {
			_QuickVector_iterator _Tmp = *this;
			_Tmp += _Off; 
			return _Tmp;
		}

		SUN_CONTAINER _QuickVector_iterator& operator-=(const difference_type _Off) noexcept {
			_Mybase::operator-=(_Off);
			return *this;
		}

		using _Mybase::operator-;

		SUN_CONTAINER _QuickVector_iterator operator-(const difference_type _Off) const noexcept {
			_QuickVector_iterator _Tmp = *this;
			_Tmp -= _Off; 
			return _Tmp;
		}

		SUN_CONTAINER reference operator[](const difference_type _Off) const noexcept {
			return const_cast<reference>(_Mybase::operator[](_Off));
		}

	};

	template<class _T ,class _Alloc = DefaultAlloc<_T>>
	class QuickVector {
	public:
		using value_type = _T;
		using allocator_type = _Alloc;
		using pointer = _T*;
		using const_pointer = const _T*;
		using reference = _T&;
		using const_reference = const _T&;
		using size_type = size_t;
		using difference_type = ptrdiff_t;

		using iterator = _QuickVector_iterator<QuickVector<_T>>;
		using const_iterator = _QuickVector_const_iterator<QuickVector<_T>>;
		using reverse_iterator = _STD reverse_iterator<iterator>;
		using const_reverse_iterator = _STD reverse_iterator<const_iterator>;

		/****************************member functions**************************/
		explicit QuickVector() {

		}

		explicit QuickVector(size_type n, const value_type& val = value_type()) {
			size_type cap = _calculateGrowth(n);
			_reallocate(n, cap);
			for (auto it = first_; it != last_; ++it)
			{
				alloc_.construct(it, val);
			}
		}

		template<class InputIterator>
		QuickVector(InputIterator first, InputIterator last, const _Alloc& al = _Alloc()) {
			alloc_ = al;
			size_type siz = last - first;
			if (siz == 0) return;

			size_type cap = _calculateGrowth(siz);
			_reallocate(siz, cap);
			auto it = first_;
			for (; first != last; ++first , ++it) {
				alloc_.construct(it, *first);
			}
		}

		QuickVector(std::initializer_list<_T> list, const _Alloc& al = _Alloc()) {
			size_type siz = list.size();
			if (siz == 0) return;

			size_type cap = _calculateGrowth(siz);
			_reallocate(siz, cap);
			auto it = first_;
			for (auto& iter : list) {
				*it = *iter;
				++it;
			}
		}

		QuickVector(const QuickVector& x) {
			alloc_ = x.alloc_;
			size_type siz = x.size();
			if (siz == 0) return;

			size_type cap = _calculateGrowth(siz);
			_reallocate(siz, cap);
			auto it = first_;
			for (size_type i = 0;i<siz;++i){
				*it = x[i];
			}
		}

		QuickVector(QuickVector&& x) {
			size_type siz = x.size();
			if (siz == 0) return;

			size_type cap = _calculateGrowth(siz);
			_reallocate(siz, cap);
			auto it = first_;
			for (size_type i = 0; i < siz; ++i) {
				*it = std::move(x[i]);
			}
		}


		~QuickVector() {
			for (auto it = first_; it != last_; ++it) {
				alloc_.destroy(it);
			}
			alloc_.deallocate(first_, end_ - first_);
		}

		SUN_CONTAINER QuickVector<_T>& operator = (const QuickVector<_T>& vec) {
			for (auto it = first_; it != last_; ++it) {
				alloc_.destroy(it);
			}
			alloc_.deallocate(first_, end_ - first_);
			first_ = nullptr;
			last_ = nullptr;
			end_ = nullptr;

			size_type newSiz = vec.size();
			if (newSiz <= 0) return *this;

			size_type cap = _calculateGrowth(newSiz);
			_reallocate(newSiz, cap);

			auto iter = first_;
			for (auto it = 0; it < newSiz; ++it) {
				*iter = vec[it];
				++iter;
			}

			return *this;
		}

		SUN_CONTAINER QuickVector& operator=(std::initializer_list<_T> list) {
			for (auto it = first_; it != last_; ++it) {
				alloc_.destroy(it);
			}
			alloc_.deallocate(first_, end_ - first_);
			first_ = nullptr;
			last_ = nullptr;
			end_ = nullptr;

			size_type newSiz = list.size();
			if (newSiz <= 0) return *this;

			size_type cap = _calculateGrowth(newSiz);
			_reallocate(newSiz, cap);

			auto iter = first_;
			for (auto it = list.begin(); it != list.end();++it) {
				*iter = *it;
				++iter;
			}
			return *this;
		}

		/**********************iterators*************************/
		SUN_CONTAINER iterator begin() {
			return iterator(first_);
		}

		SUN_CONTAINER const_iterator begin() const{
			return const_iterator(first_);
		}

		SUN_CONTAINER iterator end() {
			return iterator(last_);
		}

		SUN_CONTAINER const_iterator end() const {
			return const_iterator(last_);
		}

		SUN_CONTAINER reverse_iterator  rbegin() {
			return reverse_iterator(end());
		}

		SUN_CONTAINER reverse_iterator  rbegin() const {
			return const_reverse_iterator(end());
		}

		SUN_CONTAINER reverse_iterator rend() {
			return reverse_iterator(begin());
		}

		SUN_CONTAINER reverse_iterator rend() const {
			return const_reverse_iterator(begin());
		}

		SUN_CONTAINER const_iterator cbegin() const noexcept {
			return begin();
		}

		SUN_CONTAINER const_iterator cend() const noexcept {
			return end();
		}

		SUN_CONTAINER const_reverse_iterator crbegin() const noexcept {
			return rbegin();
		}

		SUN_CONTAINER const_reverse_iterator crend() const noexcept {
			return rend();
		}

		/************************capacity*************************/
		SUN_CONTAINER size_type size() const {
			return last_-first_;
		}

		SUN_CONTAINER size_type max_size() const {
			//FIX ME
			return static_cast<size_type>((std::numeric_limits<difference_type>::max)());
		}

		SUN_CONTAINER void resize(size_type n, value_type val = value_type()) {
			size_type siz = last_ - first_;
			if (n < siz) {
				pointer newLast = first_ + n;
				_destroy(newLast, last_);
				last_ = newLast;
				return;
			}
			else if (n > siz) {			
				if (n <= end_ - first_) {
					auto newLast = first_ + n;
					for (auto it = last_; it != newLast; ++it) {
						alloc_.construct(it, val);
					}
					last_ = newLast;
				}
				else {
					size_type newCap = _calculateGrowth(n);
					_reallocate(n, newCap);
					pointer where = first_ + siz;
					for (auto it = where; it != last_; ++it) {
						alloc_.construct(it, val);
					}
				}
				
			}
		}

		SUN_CONTAINER size_type capacity() const {
			return static_cast<size_type>(end_ - first_);
		}

		SUN_CONTAINER bool empty() const {
			return first_ == last_;
		}

		SUN_CONTAINER void reserve(size_type n) {
			if (n > capacity()) {
				_reallocate(last_ - first_, n);
			}
		}

		SUN_CONTAINER void shrink_to_fit() {
			size_type siz = last_ - first_;
			size_type cap = end_ - first_;
			if (cap == siz) return;
			_reallocate(siz, siz);
		}

		/****************************element access**********************/
		SUN_CONTAINER reference operator[] (size_type n) {
			return *(first_ + n);
		}

		SUN_CONTAINER const_reference operator[] (size_type n) const {
			return first_[n];
		}

		SUN_CONTAINER reference at(size_type n) {
			return first_[n];
		}

		SUN_CONTAINER const_reference at(size_type n) const {
			return first_[n];
		}

		SUN_CONTAINER reference front() {
			return *first_;
		}

		SUN_CONTAINER const_reference front() const {
			return *first_;
		}

		SUN_CONTAINER reference back() {
			return last_[-1];
		}

		SUN_CONTAINER const_reference back() const {
			return last_[-1];
		}

		SUN_CONTAINER value_type* data() {
			return first_;
		}

		SUN_CONTAINER const value_type* data() const noexcept {
			return first_;
		}

		/**************************modifiers**********************/
		template<class InputIterator>
		SUN_CONTAINER void assign(InputIterator first, InputIterator last) {
			size_type siz = last - first;
			if (siz > end_ - first_) {

			}
			else {

			}
		}

		SUN_CONTAINER void assign(size_type n, const value_type& val) {

		}

		SUN_CONTAINER void push_back(const value_type& val) {
			if (last_ < end_) {
				alloc_.construct(last_, val);
				last_++;
				return;
			}
			size_type where = last_-first_;
			size_type newCap = _calculateGrowth(where+1);
			_reallocate(where+1, newCap);
			alloc_.construct(first_ + where, val);
			//last += 1;

		}

		SUN_CONTAINER void pop_back() {
			if (last_ > first_) {
				auto ptr = last_ - 1;
				ptr->~value_type();
				last_--;
				return;
			}
			
		}

		SUN_CONTAINER void push(const value_type& val) {
			push_back(val);
		}

		SUN_CONTAINER value_type pop() {
			value_type res;
			if (last_ > first_) {
				auto ptr = last_ - 1;
				res = std::move(*ptr);
				ptr->~value_type();
				--last_;	
			}
			return res;
		}

		SUN_CONTAINER iterator insert(iterator position, const value_type& val) {
			_insert_n(position, 1, val);
		}

		SUN_CONTAINER void insert(iterator position, size_type n, const value_type& val) {
			_insert_n(position, n, val);
		}

		template<class InputIterator>
		SUN_CONTAINER void insert(const_iterator where, InputIterator first, InputIterator last) {
			size_type count = last - first;
			if (count == 0) return;
			size_type start = where.ptr_ - first_;
			size_type oldSiz = last_ - first_;
			if (count <= end_ - last_) {
				last_ += count;
			}
			else {
				size_type newSiz = last_ - first_ + count;
				size_type newCap = _calculateGrowth(newSiz);
				_reallocate(newSiz, newCap);
			}
			size_type diff = oldSiz - start;
			if (diff <= count) {
				auto it2 = first_ + oldSiz + count - 1;
				auto it1 = first_ + oldSiz - 1;
				size_type num = diff;
				for (size_type i = 0; i < num; ++i) {
					alloc_.construct(it2, *it1);
					--it2;
					--it1;
				}
				num = count - num;
				for (size_type i = 0; i < num; ++i) {
					--last;
					alloc_.construct(it2, *last);
					--it2;
				}
				num = oldSiz - start;
				for (size_type i = 0; i < num; ++i) {
					--last;
					*it2 = *last;
					--it2;
				}
			}
			else {
				auto it2 = first_ + oldSiz + count - 1;
				auto it1 = first_ + oldSiz - 1;
				for (size_type i = 0; i < count; ++i) {
					alloc_.construct(it2, *it1);
					--it1;
					--it2;
				}
				size_type num = diff - count;
				for (size_type i = 0; i < num; ++i) {
					*it2 = *it1;
					--it1;
					--it2;
				}
				for (size_type i = 0; i < count; ++i) {
					--last;
					*it2 = *last;
					--it2;
				}
			}

			return;
		}

		SUN_CONTAINER iterator erase(iterator position) {
			if (position == last_) return begin();
			if (position == last_ - 1) {
				pop_back(); return end();
			}
			auto ptr = position.ptr_;
			auto ptr2 = ptr; ++ptr2;
			auto back = last_ - 1;
			while (ptr != back) {
				*ptr = *ptr2;
				++ptr;
				++ptr2;
			}
			ptr2->~value_type();
			--last_;

			return position;
		}

		SUN_CONTAINER iterator erase(iterator first, iterator last) {
			if (first == last) return begin();
			iterator res = first;
			size_type diff = last.ptr_ - first.ptr_;
			size_type moved = last_- last.ptr_;
			for (size_type i = 0; i < moved; ++i) {
				*first = std::move(*last);
				++first;
				++last;
			}
			for (; first != last_; ++first) {
				(*first).~value_type();
			}
			last_ -= diff;
			return res;
		}

		SUN_CONTAINER void swap(QuickVector& x) {
			swap(alloc_, x.alloc_);
			std::swap(first_, x.first_);
			std::swap(last_, x.last_);
			std::swap(end_, x.end_);
		}

		SUN_CONTAINER void clear() {
			for (auto it = first_; it != last_; ++it) {
				it->~value_type();
			}
			last_ = first_;
		}

		template<class ... Args>
		SUN_CONTAINER iterator emplace(const_iterator position, Args&&... args) {

		}

		template<class ... Args>
		SUN_CONTAINER void emplace_back(Args&&... args) {
			if (last_ != end_) {
				alloc_.construct(last_, args);
				++last_;
				return;
			}
			size_type newSiz = last_- first_ + 1;
			size_type newCap = _calculateGrowth(newSiz);

			_reallocate(newSiz, newCap);

			alloc_.construct(last_, args);
			++last_;
			return;
		}

		/***************************allocator*************/
		//TODO

	protected:
		//参考stl实现，计算新的capacity
		size_type _calculateGrowth(const size_type newSiz) const {
			// given _Oldcapacity and _Newsize, calculate geometric growth
			const size_type _Oldcapacity = capacity();
			const auto Max = max_size();

			size_type geometric = _Oldcapacity * DEFAULT_GROWTH_FACTOR;
			if (geometric > Max) {
				return Max; // geometric growth would overflow
			}

			if (geometric < newSiz) {
				geometric = newSiz;// geometric growth would be insufficient
			}

			return geometric; // geometric growth is sufficient
		}

		//重新分配数组，把旧数据拷贝过去
		SUN_CONTAINER void _reallocate(const size_type newSiz, const size_type newCapacity) {
			size_type where = last_ - first_;
			assert(newSiz >= end_-first_);//等于对应reserve情形
			assert(newCapacity >= newSiz);

			pointer newVec = alloc_.allocate(newCapacity);

			//alloc_.construct(newVec + where, newVec + newSiz);

			_move(first_, last_, newVec);

			for (auto it = first_; it != last_; ++it) {
				alloc_.destroy(it);
			}

			alloc_.deallocate(first_, where);

			first_ = newVec;
			last_ = first_ + newSiz;
			end_ = first_ + newCapacity;
		}

		SUN_CONTAINER void _move(pointer first, pointer last, pointer dest) {
			for (auto it = first; it != last; ++it,++dest) {
				//*dest = std::move(*it);
				alloc_.construct(dest, std::move(*it));
			}
		}

		SUN_CONTAINER void _destroy(pointer first, pointer last) {
			for (; first != last; ++first) {
				first->~value_type();
			}
		}

		SUN_CONTAINER void _insert_n(const_iterator where, const size_type count, const value_type& val) {
			if (count == 0) return;
			size_type start = where - first_;
			size_type oldSiz = last_ - first_;
			if (count <= end_ - last_) {	
				last_ += count;
			}
			else {
				size_type newSiz = last_ - first_ + count;
				size_type newCap = _calculateGrowth(newSiz);
				_reallocate(newSiz, newCap);
			}
			size_type diff = oldSiz - start;
			if (diff <= count) {
				auto it2 = first_ + oldSiz + count -1;
				auto it1 = first_ + oldSiz - 1;
				size_type num = diff;
				for (size_type i = 0; i < num; ++i) {
					alloc_.construct(it2, *it1);
					--it2;
					--it1;
				}
				num = count - num;
				for (size_type i = 0; i < num; ++i) {
					alloc_.construct(it2, val);
					--it2;
				}
				num = oldSiz - start;
				for (size_type i = 0; i < num; ++i) {
					*it2 = val;
					it2--;
				}
			}
			else {
				auto it2 = first_ + oldSiz + count - 1;
				auto it1 = first_ + oldSiz - 1;
				for (size_type i = 0; i < count; ++i) {
					alloc_.construct(it2, *it1);
					--it1;
					--it2;
				}
				size_type num = diff - count;
				for (size_type i = 0; i < num; ++i) {
					*it2 = *it1;
					--it1;
					--it2;
				}
				for (size_type i = 0; i < count; ++i) {
					*it2 = val;
					--it2;
				}
			}
			
			return;
		}

		pointer first_ = nullptr;
		pointer last_ = nullptr;
		pointer end_ = nullptr;
		_Alloc alloc_;
	};

	template<class _T>
	bool operator==(const QuickVector<_T>& lhs, const QuickVector<_T>& rhs) {

	}

	template <class T>
	bool operator!= (const QuickVector<T>& lhs, const QuickVector<T>& rhs) {

	}

	template <class T>
	bool operator<  (const QuickVector<T>& lhs, const QuickVector<T>& rhs) {

	}

	template <class T>
	bool operator<= (const QuickVector<T>& lhs, const QuickVector<T>& rhs) {

	}

	template <class T>
	bool operator>  (const QuickVector<T>& lhs, const QuickVector<T>& rhs) {

	}

	template <class T>
	bool operator>= (const QuickVector<T>& lhs, const QuickVector<T>& rhs) {

	}

	template<class _T>
	void swap(QuickVector<_T>& lhs, QuickVector<_T>& rhs) {

	}

	//重命名模板
	template<typename _T>
	using Vector = QuickVector<_T>;


}

#endif