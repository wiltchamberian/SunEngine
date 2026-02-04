//导出给lua的函数

#include "luaExport.h"
#include "Platform/PlatformMisc.h"
#include "Render/GraphicsLib.h"

namespace Sun {

	int lua_getGlobalPath(lua_State* L){
	
		std::string path = getGlobalPath();

		lua_pushfstring(L, path.c_str());

		return 1;
	}

	int lua_createShader(lua_State* L) {

		std::string path = lua_tostring(L, -2);
		EShaderType type = static_cast<EShaderType>(lua_tointeger(L, -1));
		Shader shader = g_graphicsLib->createShader(path.c_str(), type, true);

		lua_pushinteger(L, shader.id_);
		lua_pushinteger(L, lua_Integer(shader.type_));

		return 2;
	}

	int lua_createProgram(lua_State* L) {

		std::vector<Shader> input;
		//从table获取参数,
		int l = lua_tointeger(L, -1);
		//lua_gettable(L, -1);

		for (int i = 0; i < l; ++i) {
			Shader shader;
			//获取Shader第一个参数
			lua_gettable(L, -1);
			shader.id_ = lua_tointeger(L, -1);
			//获取shader第二个参数
			lua_gettable(L, -1);
			shader.type_ = (EShaderType)lua_tointeger(L, -1);
			input.push_back(shader);
		}

		g_graphicsLib->createProgram(input);

		return 0;
	}
}

