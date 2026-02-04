/*****************************************************************************
* @brief : Clock
* @author : acedtang
* @date : 2021/7/23
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __CLOCK_H
#define __CLOCK_H

#include <chrono>

namespace Sun {

	using internal_clock = std::chrono::system_clock;
	//using internal_clock = std::chrono::high_resolution_clock;
	class Clock {
	public:
		Clock();
		void reset();
		double getMilliDuration();
		double getMicroDuration();
		double getTotalMicroDuration();
		int getMilliDurationInt();
		template<class _T, class _Resolution>
		_T getDuration() {
			auto it = internal_clock::now();
			return (_T)std::chrono::duration<double, _Resolution>(it - base_).count();
		}
	protected:
		internal_clock::time_point base_;
		internal_clock::time_point start_;
	};

	extern double Sys_Milliseconds(); 
}


#endif