/*****************************************************************************
* @brief : WinPlatform.h
* windows平台定义
* @author : acedtang  
* @date : 2021/7/25
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef WIN_PLATFORM_H
#define WIN_PLATFORM_H

namespace Sun{

// DLL export and import definitions
#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)

	// Function type macros.
#define CDECL	    __cdecl						/* Standard C function */
#define STDCALL		__stdcall					/* Standard calling convention */
#define FORCEINLINE __forceinline				/* Force code to be inline */
#define FORCENOINLINE __declspec(noinline)		/* Force code to NOT be inline */

}



#endif