#include "Clock.h"

namespace Sun {

	Clock::Clock() {
		reset();
		start_ = base_;
	}

	void Clock::reset() {
		base_ = internal_clock::now();
	}

	double Clock::getMilliDuration() {
		auto it = internal_clock::now();
		return std::chrono::duration<double, std::milli>(it - base_).count();
	}

	double Clock::getMicroDuration() {
		auto it = internal_clock::now();
		return std::chrono::duration<double, std::micro>(it - base_).count();
	}

	double Clock::getTotalMicroDuration() {
		auto it = internal_clock::now();
		return std::chrono::duration<double, std::micro>(it - start_).count();
	}

	int Clock::getMilliDurationInt() {
		return (int)getMilliDuration();
	}

	double Sys_Milliseconds()
	{
		int			sys_curtime;
		static bool	s_initialized = false;
		static std::chrono::high_resolution_clock::time_point s_timeBase;

		if (!s_initialized) {
			s_timeBase = std::chrono::high_resolution_clock().now();
			s_initialized = true;
		}
		auto curTime = std::chrono::high_resolution_clock().now();
		sys_curtime = std::chrono::duration<double, std::milli>(curTime - s_timeBase).count();

		return sys_curtime;
	}
}