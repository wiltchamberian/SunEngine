#include "SunQuickView.h"
#include "Platform/PlatformIO.h"
#include "Common/KeyBoard.h"

namespace Sun {

	void SunQuickView::keyPressEvent(QKeyEvent* ev) {

		keyCallBack(ev);

		QQuickView::keyPressEvent(ev);
	}

	void SunQuickView::keyReleaseEvent(QKeyEvent* ev) {

        keyCallBack(ev);
		QQuickView::keyReleaseEvent(ev);
	}

    char mapKey(int key) {
        char mapKey = key;
        //当前只处理了26个字母
        if (key >= Qt::Key_A && key <= Qt::Key_Z) {
            mapKey = key - Qt::Key_A + 'a';
        }

        return mapKey;
    }

	void SunQuickView::keyCallBack(QKeyEvent* ev) {
        auto type = ev->type();
        int key = ev->key();
        char mapkey = mapKey(key);

        if (type == QEvent::KeyPress || type == QEvent::KeyRelease) {
            SEvent e;
            e.type = SE_KEY;
            
            e.value = mapkey;
            e.value2 = (type == QEvent::KeyPress) ? 1 : 0;

            g_keys[e.value].down = (type == QEvent::KeyPress);
            g_evQue.push(e);
        }


        switch (key)
        {
        case Qt::Key_Escape:
        {
            //glfwSetWindowShouldClose(window, GL_TRUE);
            //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
           
        }
        break;
        default:
            break;
        }
	}
}