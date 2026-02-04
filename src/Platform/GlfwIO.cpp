#include "GlfwIO.h"

//#include "Gui/imgui.h"   
//#include "Gui/imgui_impl_opengl3.h"
//#include "Gui/imgui_impl_glfw.h"//必须放在PlatformIO.h上面！fuck!,why??

#include "ThirdParty/glfw/glad/glad.h"
#include "ThirdParty/glfw/glfw3.h"

#include "Common/MouseState.h"

#include <iostream>
#include "WinIO.h"
#include "Render/GraphicsProxy.h"
#include "Common/KeyBoard.h"

namespace Sun {

    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        //glViewport(0, 0, width, height);

        if (g_graphicsProxy) {
            g_graphicsProxy->recreateGBuffer(width, height);
        }

    }

    void on_window_resize(GLFWwindow* window, int width, int height) {
        if (width == 0 || height == 0) return;

        //VulkanGraphicsLib* vulkan = reinterpret_cast<VulkanGraphicsLib*>(glfwGetWindowUserPointer(window));
        //if (vulkan) {
        //    vulkan->recreateSwapChain();
        //}

        //g_graphicsProxy->setupGBuffer(width, height);
    }

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

        if (action == GLFW_PRESS || action == GLFW_RELEASE) {
            SEvent ev;
            ev.type = SE_KEY;
            //char mapkey = MapKey(scancode);
            char mapkey = key;
            ev.value = key + 'a' - 'A';
            ev.value2 = (action == GLFW_PRESS) ? 1 : 0;

            g_keys[ev.value].down = (action == GLFW_PRESS);
            g_evQue.push(ev);
        }


        switch (key)
        {
        case GLFW_KEY_ESCAPE:
        {
            //glfwSetWindowShouldClose(window, GL_TRUE);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        break;
        default:
            break;
        }
    }

    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            int mode = glfwGetInputMode(window, GLFW_CURSOR);
            if (mode != GLFW_CURSOR_NORMAL) {
                SEvent ev;
                ev.type = SE_MOUSE_CLICK;
                ev.value = 1;
                g_evQue.push(ev);
            }
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            g_ms.firstMouse = true;

        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            int mode = glfwGetInputMode(window, GLFW_CURSOR);
            if (mode == GLFW_CURSOR_DISABLED) {
                SEvent ev;
                ev.type = SE_MOUSE_CLICK;
                ev.value = 0;
                g_evQue.push(ev);
            }
        }
    }

    void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
    {
        int cursor = glfwGetInputMode(window, GLFW_CURSOR);
        if (cursor != GLFW_CURSOR_DISABLED) return;

        int width = 0;
        int height = 0;
        glfwGetWindowSize(window, &width, &height);

        int center_x = width / 2;
        int center_y = height / 2;
        if (g_ms.firstMouse)
        {
            g_ms.lastX = xpos;
            g_ms.lastY = ypos;
            g_ms.firstMouse = false;
            glfwSetCursorPos(window, center_x, center_y);
            return;
        }

        float xoffset = xpos - center_x;
        //for the screen the y axis is downward;
        float yoffset = center_y - ypos;
        g_ms.lastX = xpos;
        g_ms.lastY = ypos;

        SEvent ev;
        ev.type = SE_MOUSE;
        ev.value = xoffset;
        ev.value2 = yoffset;
        ev.width = width;
        ev.height = height;

        g_evQue.push(ev);

        //cursor back to center
        glfwSetCursorPos(window, center_x, center_y);
    }

    GlfwIO::~GlfwIO()
    {

    }

    bool GlfwIO::windowShouldClose() {
        return glfwWindowShouldClose(glfwWindow_);
    }

    void GlfwIO::terminate() {
        glfwDestroyWindow(glfwWindow_);
        glfwTerminate();
    }

    Extend GlfwIO::getFrameBufferSize() {
        Extend extend;
        glfwGetFramebufferSize(glfwWindow_, &extend.width, &extend.height);
        return extend;
    }

    Rect GlfwIO::getViewportGeometry() {
        Extend ext = getFrameBufferSize();
        return { 0,0,ext.width,ext.height };
    }

    void GlfwIO::swapBuffer() {
        glfwSwapBuffers(glfwWindow_);
    }

    void GlfwIO::setFrameBufferSizeCallBack(Func_FrameBufferSizeCallback cb) {
        framebufferSizeCallback_ = cb;
    }

    void GlfwIO::setKeyCallBack(Func_KeyCallback cb) {
        keyCallback_ = cb;
    }

    void GlfwIO::setMouseButtonCallBack(Func_mouse_button_callback cb) {
        mouseButtonCallback_ = cb;
    }

    void GlfwIO::setCursorPosCallBack(Func_cursor_pos_callback cb) {
        cusorPosCallback_ = cb;
    }

    SEvent GlfwIO::getEvent() {
        SEvent ev;

        if (!g_evQue.empty())
        {
            SEvent ev = g_evQue.front();
            g_evQue.pop();
            return ev;
        }
        return ev;
    }

    void GlfwIO::generateEvents() {
        glfwPollEvents();
    }

    Sun::SUserCmd GlfwIO::generateCmd()
    {
        while (!g_evQue.empty()) {
            SEvent ev = g_evQue.front();
            g_evQue.pop();
        }
        return SUserCmd();
    }

    bool GlfwIO::initEnvironment(const RunStruct& stru) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if defined __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        glfwWindow_ = glfwCreateWindow(stru.window_width, stru.window_height, "SunEngine", NULL, NULL);
        if (glfwWindow_ == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(glfwWindow_);
        auto cb = framebufferSizeCallback_ ? framebufferSizeCallback_ : framebuffer_size_callback;
        glfwSetFramebufferSizeCallback(glfwWindow_, cb);
        auto kcb = keyCallback_ ? keyCallback_ : key_callback;
        glfwSetKeyCallback(glfwWindow_, kcb);
        glfwSetInputMode(glfwWindow_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        auto mbcb = mouseButtonCallback_ ? mouseButtonCallback_ : mouse_button_callback;
        glfwSetMouseButtonCallback(glfwWindow_, mbcb);
        auto cpcb = cusorPosCallback_ ? cusorPosCallback_ : cursor_pos_callback;
        glfwSetCursorPosCallback(glfwWindow_, cpcb);

        //glfwSetWindowUserPointer(glfwWindow_, nullptr);
        //glfwSetWindowSizeCallback(glfwWindow_, on_window_resize);

#if 0   //move out
        ASSERT(g_graphicsLib != nullptr);
        ASSERT(g_renderManager != nullptr);
        ASSERT(g_graphicsProxy != nullptr);
        if (g_graphicsLib && g_renderManager && g_graphicsProxy) {
            //g_graphicsLib->bindWindow((void*)(hWnd));
            g_graphicsLib->load();
            g_renderManager->createShaders();
            g_graphicsProxy->recreateGBuffer(SCR_WIDTH, SCR_HEIGHT);
        }
#endif


        return true;
    }

    GLFWwindow* GlfwIO::getGlfwWindow() {
        return glfwWindow_;
    }
}