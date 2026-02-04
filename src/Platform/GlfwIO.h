#ifndef __SUN_GLFW_IO_H
#define __SUN_GLFW_IO_H

#include "PlatformIO.h"

class GLFWwindow;
namespace Sun {

    typedef void (*Func_FrameBufferSizeCallback)(GLFWwindow* window, int width, int height);
    typedef void (*Func_KeyCallback)(GLFWwindow* window, int key, int scancode, int action, int mods);
    typedef void (*Func_mouse_button_callback)(GLFWwindow* window, int button, int action, int mods);
    typedef void (*Func_cursor_pos_callback)(GLFWwindow* window, double xpos, double ypos);


    class GlfwIO : public PlatformIO
    {
    public:
        ~GlfwIO();
        virtual bool windowShouldClose();
        void terminate();
        virtual SEvent getEventFromSys() { return SEvent(); }

        virtual void inFrame() {};
        virtual bool initEnvironment(const RunStruct& stru);
        virtual SEvent getEvent();
        virtual void generateEvents();
        virtual SUserCmd generateCmd();
        GLFWwindow* getGlfwWindow();
        virtual Extend getFrameBufferSize();
        Rect getViewportGeometry() override;
        virtual void swapBuffer();

        void vmCall(void* info, void* extraInfo) {}

        //如果设置为空指针，将使用系统默认回调；如果想执行空回调，请设置为自定义的空函数实现
        void setFrameBufferSizeCallBack(Func_FrameBufferSizeCallback cb);
        void setKeyCallBack(Func_KeyCallback cb);
        void setMouseButtonCallBack(Func_mouse_button_callback cb);
        void setCursorPosCallBack(Func_cursor_pos_callback cb);

    protected:
        Func_FrameBufferSizeCallback framebufferSizeCallback_ = nullptr;
        Func_KeyCallback keyCallback_ = nullptr;
        Func_mouse_button_callback mouseButtonCallback_ = nullptr;
        Func_cursor_pos_callback cusorPosCallback_ = nullptr;
        //std::queue<SEvent> _evQue;
        GLFWwindow* glfwWindow_ = nullptr;
    };
}



#endif