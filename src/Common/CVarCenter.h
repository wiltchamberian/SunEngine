/**
 * @projectName   CVarCetner
 * @brief        
 * 控制台变量中心，主要存放一些全局的控制台变量，如果将这些变量分散到各个使用类中，
 * 则过于分散，需要许多不必要的wrapper代码，难以管理，因此，用一个类统一管理这些变量
 * @author        acedtang
 * @date          2021/9/10
 */

#ifndef __SUN_CVAR_CENTER_H
#define __SUN_CVAR_CENTER_H

#include "Common/Common.h"
#include <variant>
#include <unordered_map>
#include <memory>
#include <string>

namespace Sun {
	//using CVar = std::variant<int, float, vec2,vec3,vec4, std::string>;

#define CVAR_VIEWPORT_X_RATIO "viewport_x_ratio"
#define CVAR_VIEWPORT_Y_RATIO "viewport_y_ratio"
#define CVAR_VIEWPORT_W_RATIO "viewport_w_ratio"
#define CVAR_VIEWPORT_H_RATIO "viewport_h_ratio"

#define CVAR_CLEAR_COLOR "clear_color"

#define CVAR_OPEN_SHADOW_MAP "open_shadow_map"

#define CVAR_SHOW_SCREEN_SHADOW_MAP "show_screen_shadow_map"

#define CVAR_OPEN_NORMAL_MAP "open_normal_map"

#define CVAR_SHOW_SCREEN_NORMAL_MAP "show_screen_normal_map"

	//TODO: may add a require for _T type
	template <typename _T>
	class TCVarTypeCenter {
	public:
		template<typename _T>
		static _T& getCVar(const std::string& name, const _T& defaultValue) {
			auto iter = cvars_.find(name);
			if (iter != cvars_.end()) {
				return iter->second;
			}
			else {
				cvars_[name] = defaultValue;
				return cvars_[name];
			}
		}
		template<typename _T>
		static void setCVar(const std::string& name, const _T& value) {
			cvars_[name] = value;
		}
		static std::unordered_map<std::string, _T> cvars_;
	};
	template<typename _T>
	std::unordered_map<std::string, _T> TCVarTypeCenter<_T>::cvars_;


	class CVarCenter {
	public:
		CVarCenter();
		
		template<typename _T>
		void setCVar(const std::string& name ,const _T& value) {
			TCVarTypeCenter<_T>::setCVar(name,value);
		}
		template<typename _T>
		_T&  getCVar(const std::string& name, const _T& defaultValue = _T()) {
			return TCVarTypeCenter<_T>::getCVar(name, defaultValue);
		}

		inline ViewPortRatio getViewPortRatio() {
			ViewPortRatio ratio;
			ratio.x = getCVar<float>(CVAR_VIEWPORT_X_RATIO);
			ratio.y = getCVar<float>(CVAR_VIEWPORT_Y_RATIO);
			ratio.w = getCVar<float>(CVAR_VIEWPORT_W_RATIO);
			ratio.h = getCVar<float>(CVAR_VIEWPORT_H_RATIO);
			return ratio;
		}
		inline void setViewPortRatio(const ViewPortRatio& ratio) {
			setCVar(CVAR_VIEWPORT_X_RATIO, ratio.x);
			setCVar(CVAR_VIEWPORT_Y_RATIO, ratio.y);
			setCVar(CVAR_VIEWPORT_W_RATIO, ratio.w);
			setCVar(CVAR_VIEWPORT_H_RATIO, ratio.h);
		}
	protected:
		void initEnvironment();
	};

	extern std::unique_ptr<CVarCenter> g_cvarCenter;

}

#endif