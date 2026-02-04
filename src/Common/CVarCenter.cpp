#include "CVarCenter.h"
#include "Math/Vector2D.h"
#include "Math/Vector3D.h"
#include "Math/Vector4D.h"

namespace Sun {

	std::unique_ptr<CVarCenter> g_cvarCenter = nullptr;

	CVarCenter::CVarCenter() {
		initEnvironment();
	}

	void CVarCenter::initEnvironment() {
		setCVar<float>(CVAR_VIEWPORT_X_RATIO, 0.0);
		setCVar<float>(CVAR_VIEWPORT_Y_RATIO, 0.0);
		setCVar<float>(CVAR_VIEWPORT_W_RATIO, 1.0);
		setCVar<float>(CVAR_VIEWPORT_H_RATIO, 1.0);

		setCVar(CVAR_CLEAR_COLOR, vec4(0.0, 0.0, 0.0, 1.0));

		setCVar<bool>(CVAR_OPEN_SHADOW_MAP, true);
		setCVar<bool>(CVAR_SHOW_SCREEN_SHADOW_MAP, false);

		setCVar<bool>(CVAR_OPEN_NORMAL_MAP, false);
		setCVar<bool>(CVAR_SHOW_SCREEN_NORMAL_MAP, false);
	}

}