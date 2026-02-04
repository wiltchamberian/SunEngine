/*****************************************************************************
* @brief : Common
* È«¾Ö
* @author : acedtang
* @date : 2021/4/6
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __UTIL_H
#define __UTIL_H

#include <cstddef>
#include <atomic>
#include <mutex>
#include <vector>

namespace Sun {

	class UniqueTypeCounter {
	public:
		template<class _T>  //deprecated
		static size_t unique_type_id_old() {
			static size_t res = 0;
			static _T b = [this](size_t& siz) {
				siz = counter_++;
				return _T();
			}(res);
			return res;
		}
		template<class _T>
		static size_t unique_type_id() {
			static std::once_flag flag;
			static size_t res = 0;
			std::call_once(flag, []() {
				res = counter_++;
				});
			return res;
		}
	private:
		static std::atomic<size_t> counter_;
	};

}

#endif