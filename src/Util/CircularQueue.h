/*****************************************************************************
* @brief : CircularQueue
* 循环队列，一种高效的循环队列实现，
* 相比于stl::deque的性能要高不少,
* 当越界时，会保留最近push的元素.
* @author : acedtang
* @date : 2021/8/11
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __CIRCULAR_QUEUE_H
#define __CIRCULAR_QUEUE_H

#include <cassert>

namespace Sun {

	template<class _T ,int _Power = 8>
	class CircularQueue {
	public:
		CircularQueue()
		{
			memory_ = ::malloc(sizeof(_T) * (1 << _Power));
			datas_ = (_T*)memory_;
		}

		~CircularQueue() {
			::free(memory_);
		}

		//向队列尾部追加元素
		void push(const _T& data) {
			if (count == capacity_) {
				assert(false, "CircularQueue will overflow, the old elements will be discard!");
				datas_[first].~_T();
				++first;
				first = first & mask_;
				--count;
			}
			::new (datas_ + end) _T(data);

			++end;
			end = end & mask_;
			++count;
		}
		//取出队列首部元素的引用
		_T& front() {
			assert(count>0);
			return datas_[first];
		}
		//弹出队列首部元素
		void pop() {
			if (count > 0) {
				datas_[first].~_T();
				++first;
				first = first & mask_;
				--count;
			}
		}
		bool empty() const {
			return count == 0;
		}
		size_t size() const {
			return count;
		}
		void clear() {
			int k = 0;
			int cur = first;
			for (k = 0; k < count; ++k) {
				datas_[cur].~_T();
				cur = (cur + 1) & mask_;
			}
			first = 0;
			count = 0;
			end = 0;
		}
		//从队列首部开始第i个元素(i 从0 开始)
		_T& front(int i) {
			return datas_[(first + i)&mask_];
		}
	protected:	
		int first = 0;
		//队列尾部元素的索引值+1
		int end = 0;
		//队列中真实元素个数
		int count = 0;
		const int capacity_ = 1 << _Power;
		const int mask_ = (1 << _Power) - 1;
		_T* datas_ = nullptr;
		void* memory_ = nullptr;
	};



}

#endif