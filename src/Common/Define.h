#ifndef __DEFINE_H
#define __DEFINE_H

#include <cstdint>
#include <memory>

namespace Sun {

#ifdef _WIN32
#define FORCE_INLINE __forceinline
#else
#define FORCE_INLINE inline
#endif

#define PRINT_OPEN 

	using int32 = std::int32_t;
	using uint32 = std::uint32_t;
	using int64 = std::int64_t;
	using uint64 = std::uint64_t;
	using int16 = std::int16_t;
	using uint16 = std::uint16_t;
	using int8 = std::int8_t;
	using uint8 = std::uint8_t;

	using Hash = uint32;

	template<class _T>
	using SharedPtr = std::shared_ptr<_T>;

	template<class _T,class ..._Args>
	inline	SharedPtr<_T> createObject(_Args&&... _args) {
		return SharedPtr<_T>(new _T(_STD forward<_Args>(_args)...));
	}
}

#endif