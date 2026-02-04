/**
 * @projectName   Sun
 * @brief         glsl
 * 将glsl的结构类型抽象一层，用于脚本编程或者其他
 * @author        acedtang
 * @date          2021/6/27
 */

#ifndef __GLSL_H
#define __GLSL_H

#include <string>
#include <variant>
#include "Math/Matrix4x4.h"
#include "Math/Vector3D.h"

namespace Sun {

	using UniformVariant = std::variant<int, vec3, Matrix3x3, Matrix4x4>;

	enum class UniformType {
		FLOAT,
		VEC2,
		VEC3,
		VEC4,
		MAT3,
		MAT4,
		SAMPLER2D,
		INT,
	};

	struct Uniform {
		UniformType type;
		std::string name;
		UniformVariant var;
		int num = 1; //若为数组，则可能>1
	};

	//glsl着色器uniform成员名称宏定义
#define U_TVP_TRANSLATE "u_tvp.translate"
#define U_TVP_QUA "u_tvp.qua"
#define U_TVP_SCALE "u_tvp.scale"

#define U_TMODEL_TRANSLATE "u_tmodel.translate"
#define U_TMODEL_QUA "u_tmodel.qua"
#define U_TMODEL_SCALE "u_tmodel.scale"

#define U_INSTANCE "u_instance"
#define U_VIEWPOS "u_viewPos"
#define U_FLUID_HEIGHT "u_fluidHeight"

#define U_VP "u_vp"
#define U_MODEL "u_model"
#define U_PROJ "u_proj"
#define U_VIEW  "u_view"
#define U_LIGHT_SPACE_MAT "u_lightSpaceMat"

#define U_GBONES "u_gBones"

#define U_SCREEN_TEXTURE "u_screenTexture"
#define U_SHADOW_MAP "u_shadowMap"

#define U_TIME "u_time"

/*********************着色器程序名**********************/
#define SP_NORMAL_PASS "NORMAL_PASS"
//#define SP_STATIC_MESH_SHADER "STATIC_MESH_SHADER"
#define SP_SKELETAL_MESH_SHADER "SKELETAL_MESH_SHADER"
#define SP_STATIC_MESH_GEO_DEFFERED_SHADER "STATIC_MESH_GEO_DEFFERED_SHADER"
#define SP_STATIC_MESH_RENDER_DEFFERED_SHADER "STATIC_MESH_RENDER_DEFFERED_SHADER"
#define SP_RAY_GEN_SHADER "RAY_GEN_SHADER"
#define SP_RAY_TRACING_SHADER "RAY_TRACING_SHADER"
#define SP_RAY_TRACING_SHOW_SHADER "RAY_TRACING_SHOW_SHADER"
#define SP_PARTICLE_SHADER "PARTICLE_SHADER"
#define SP_PARTICLE_SHOW_SHADER "PARTICLE_SHOW_SHADER"
#define SP_FRAME_BUFFER "FRAME_BUFFER"
#define SP_SIMPLE_SHADOW "SIMPLE_SHADOW"
//后缀flush表示从framebuffer刷到屏幕上的shader
#define SP_SIMPLE_SHADOW_FLUSH "SIMPLE_SHADOW_FLUSH"

#define SP_NORMAL_MAP "NORMAL_MAP"
#define SP_NORMAL_MAP_FLUSH "NORMAL_MAP_FLUSH"

#define SP_SIMPLE_PASS "SIMPLE_PASS"

#define SP_NORMAL_VISUAL "NORMAL_VISUAL"
}

#endif 