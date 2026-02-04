#ifndef __SUN_MOUSE_STATE_H
#define __SUN_MOUSE_STATE_H

namespace Sun {

	struct MouseState {
		bool firstMouse = true;
		float lastX = 0.0;
		float lastY = 0.0;
	};
	
	extern MouseState g_ms;
}

#endif