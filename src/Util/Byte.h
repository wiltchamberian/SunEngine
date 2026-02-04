/*****************************************************************************
* @brief : Byte
* 字节和位处理相关函数
* @author : acedtang
* @date : 2021/8/13
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __BYTE_H
#define __BYTE_H

#include "Common/Define.h"

namespace Sun {

	template <typename T>
	FORCE_INLINE typename std::enable_if<std::is_same<T, uint32>::value, T>::type reverseBits(T Bits)
	{
		Bits = (Bits << 16) | (Bits >> 16);
		Bits = ((Bits & 0x00ff00ff) << 8) | ((Bits & 0xff00ff00) >> 8);
		Bits = ((Bits & 0x0f0f0f0f) << 4) | ((Bits & 0xf0f0f0f0) >> 4);
		Bits = ((Bits & 0x33333333) << 2) | ((Bits & 0xcccccccc) >> 2);
		Bits = ((Bits & 0x55555555) << 1) | ((Bits & 0xaaaaaaaa) >> 1);
		return Bits;
	}

	/**
 * Aligns a value to the nearest higher multiple of 'Alignment', which must be a power of two.
 *
 * @param  Val        The value to align.
 * @param  Alignment  The alignment value, must be a power of two.
 *
 * @return The value aligned up to the specified alignment.
 */
	template <typename T>
	FORCE_INLINE constexpr T Align(T Val, uint64 Alignment)
	{
		static_assert(std::is_integral<T>::value || std::is_pointer<T>::value, "Align expects an integer or pointer type");

		//找到>=Val的最小的为Alignment整数倍的整数
		return (T)(((uint64)Val + Alignment - 1) & ~(Alignment - 1));
	}

	/**
	 * Aligns a value to the nearest lower multiple of 'Alignment', which must be a power of two.
	 *
	 * @param  Val        The value to align.
	 * @param  Alignment  The alignment value, must be a power of two.
	 *
	 * @return The value aligned down to the specified alignment.
	 */
	template <typename T>
	FORCE_INLINE constexpr T AlignDown(T Val, uint64 Alignment)
	{
		static_assert(std::is_integral<T>::Value || std::is_pointer<T>::value, "AlignDown expects an integer or pointer type");
		//设Alignment =2^x
		//这段代码的意思是将Val的后x位置为0，从而达到向下取整到Alignment整数倍的目的
		return (T)(((uint64)Val) & ~(Alignment - 1));
	}

	/**
	 * Checks if a pointer is aligned to the specified alignment.
	 *
	 * @param  Val        The value to align.
	 * @param  Alignment  The alignment value, must be a power of two.
	 *
	 * @return true if the pointer is aligned to the specified alignment, false otherwise.
	 */
	template <typename T>
	FORCE_INLINE constexpr bool IsAligned(T Val, uint64 Alignment)
	{
		static_assert(std::is_integral<T>::Value || std::is_pointer<T>::value, "IsAligned expects an integer or pointer type");

		//低位置如果为0，则说明是Alignment整数倍
		return !((uint64)Val & (Alignment - 1));
	}

	/**
	 * Aligns a value to the nearest higher multiple of 'Alignment'.
	 *
	 * @param  Val        The value to align.
	 * @param  Alignment  The alignment value, can be any arbitrary value.
	 *
	 * @return The value aligned up to the specified alignment.
	 */
	template <typename T>
	FORCE_INLINE constexpr T AlignArbitrary(T Val, uint64 Alignment)
	{
		static_assert(std::is_integral<T>::Value || std::is_pointer<T>::value, "AlignArbitrary expects an integer or pointer type");

		//对Alignment没有是2的幂要求，
		return (T)((((uint64)Val + Alignment - 1) / Alignment) * Alignment);
	}
}



#endif