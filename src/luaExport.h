/*****************************************************************************
* @brief : luaExport
* 将所有导出给lua的接口放在这里
* 这里不应该放置业务相关的函数，因为业务逻辑需要频繁修改
* 这里应该放置业务无关的通用函数，设计思想是将业务相关频繁修改的逻辑
* 移动到lua端，这里只提供通用函数接口
* @author : acedtang
* @date : 2021/6/25
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __LUA_EXPORT_H
#define __LUA_EXPORT_H

extern "C" {
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

namespace Sun {

	extern int lua_getGlobalPath(lua_State* L);

	extern int lua_createShader(lua_State* L);

	extern int lua_createProgram(lua_State* L);
}

#endif
