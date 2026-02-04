#include "WinIO.h"


#include "timeapi.h"
//#include "Engine.h"
#include "Common/Common.h"
#include "Common/KeyBoard.h"
#include "Common/Events.h"
#include "Util/exception.h"
#include "Util/Clock.h"
#include "Render/GraphicsLib.h"
//#include "RenderHelper.h"
#include "Render/ShaderManager.h"
#include "Util/Singleton.h"
#include "Platform/PlatformMisc.h"

#include <iostream>
#pragma comment(lib,"winmm.lib")

namespace Sun
{
    //std::queue<SEvent> g_evQue;

#define MAX_LOADSTRING 100

    // 全局变量:
    HINSTANCE hInst;                                // 当前实例
    WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
    WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

    //GLProgrammer g_glprogrammer;
    //GLScene g_glscene;
    HWND g_hwnd;
    SWinIO* g_winIO = nullptr;

    // 此代码模块中包含的函数的前向声明:
    ATOM                MyRegisterClass(HINSTANCE hInstance);
    //BOOL                InitInstance(HINSTANCE, int);
    LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
    INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

    ATOM MyRegisterClass(HINSTANCE hInstance)
    {
        WNDCLASSEXW wcex;

        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OPENGWIN));
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_OPENGWIN);
        wcex.lpszClassName = szWindowClass;
        wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

        return RegisterClassExW(&wcex);
    }

    bool SWinIO::initEnvironment(const RunStruct& stru)
    {
        extend_.width = stru.window_width;
        extend_.height = stru.window_height;
        HINSTANCE hInstance = (HINSTANCE)stru.hInstance;
        HINSTANCE preIntance = (HINSTANCE)stru.preInstance;
        LPWSTR lpstr = (LPWSTR)(stru.cmdline);
        int nCmdShow = stru.cmdshow;

        LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
        LoadStringW(hInstance, IDC_OPENGWIN, szWindowClass, MAX_LOADSTRING);
        ATOM atom = MyRegisterClass(hInstance);
        int error = GetLastError();
        // 执行应用程序初始化:
        if (!InitInstance(hInstance, nCmdShow))
        {
            return false;
        }
        return true;
        //HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OPENGWIN));
    }

    BOOL SWinIO::InitInstance(HINSTANCE hInstance, int nCmdShow)
    {
        hInst = hInstance; // 将实例句柄存储在全局变量中

        HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

        int k = GetLastError();

        if (!hWnd)
        {
            ASSERT(false);
            return FALSE;
        }
        g_hwnd = hWnd;
        g_winIO = this;

        ASSERT(g_graphicsLib != nullptr);
        ASSERT(g_shaderManager != nullptr);
        if (g_graphicsLib && g_shaderManager) {
            g_graphicsLib->bindWindow((void*)(hWnd));
            g_graphicsLib->load();
            g_shaderManager->createShaders();
        }
        
        //init my gl (why here？ move to other)
        //GLEngine::getInstance()->getLoader()->loadOpenGL();
        //GLEngine::getInstance()->getProgrammer()->compile();

        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);

        return TRUE;
    }

    //
    //  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
    //
    //  目标: 处理主窗口的消息。
    //
    //  WM_COMMAND  - 处理应用程序菜单
    //  WM_PAINT    - 绘制主窗口
    //  WM_DESTROY  - 发送退出消息并返回
    //
    //
    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        static int test = 0;
        OutputDebugStringA(("WndProc:" + std::to_string(test) + "\n").c_str());
        test++;
        if (/*SUN == nullptr*/ false) {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        switch (message)
        {
        case WM_COMMAND:
        {
        }
        break;
        case WM_PAINT:
        {
        }
        break;
        case WM_CREATE:
            break;
        case WM_ACTIVATE: {
            int	fActive, fMinimized;
            fActive = LOWORD(wParam);
            fMinimized = (BOOL)HIWORD(wParam);
            assert(g_winIO);
            if (g_winIO) {
                g_winIO->appActivate(fActive != WA_INACTIVE, fMinimized);
            } 
        }
        break;
        case WM_MOUSEWHEEL:
            break;
        case WM_MOVE:
            break;

        case WM_LBUTTONDOWN:
        {

        }
        break;
        case WM_LBUTTONUP:
        {
            SEvent ev;
            ev.type = SE_MOUSE_CLICK;
            ev.value = 1;
            g_evQue.push(ev);
        }
        break;
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MOUSEMOVE:
        {
            //通过其他渠道捕获鼠标事件
            //SEvent ev;
            //ev.type = SEventType::SE_MOUSE;
            //
            //g_evQue.push(ev);
#if 1
            SEvent ev;
            ev.type = SE_MOUSE;
            POINT pt;
            ::GetCursorPos(&pt);
            ::ScreenToClient(g_hwnd, &pt);
            RECT rect;
            ::GetWindowRect(g_hwnd, &rect);
            int center_x = (rect.right - rect.left) / 2;
            int center_y = (rect.bottom - rect.top) / 2;
            ev.value = pt.x - center_x;
            ev.value = pt.y - center_y;
            ev.width = (rect.right - rect.left);
            ev.height = (rect.bottom - rect.top);
            g_evQue.push(ev);

            POINT pt2 = { center_x,center_y };
            ::ClientToScreen(g_hwnd, &pt2);
            ::SetCursorPos(pt2.x,pt2.y);
#endif
            //engine->getGameWorldPtr()->handleMouseEvents(ev);
        }
        break;

        case WM_SYSKEYDOWN:
            break;
        case WM_KEYDOWN:
        {
            SEvent ev;
            ev.type = SE_KEY;
            ev.value = MapKey(lParam);
            ev.value2 = 1; //down
            g_keys[ev.value].down = true;
            g_evQue.push(ev);
        }
        break;
        case WM_SYSKEYUP:
        case WM_KEYUP:
        {
            SEvent ev;
            ev.type = SE_KEY;
            ev.value = MapKey(lParam);
            ev.value2 = 0;
            g_keys[ev.value].down = false;
            g_evQue.push(ev);
        }
        break;
        case WM_CHAR:
        {
            SEvent ev;
            ev.type = SE_CHAR;
            ev.value = wParam;
            ev.value2 = 0;
            g_evQue.push(ev);
        }
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    void SWinIO::generateEvents()
    {
        MSG			msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE)) {
            if (!GetMessage(&msg, nullptr, 0, 0)) {
                //TODO  clear memory and exit
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    SEvent SWinIO::getEventFromSys()
    {
        if (!g_evQue.empty())
        {
            if (g_evQue.size() > 2) {
                //ASSERT(false);
            }
            SEvent ev = g_evQue.front();
            g_evQue.pop();
            return ev;
        }

        MSG			msg;
        //do some delay if faster
        float dtime = 0;
        //0.01s = 10 ms
        int start_time = Sys_Milliseconds();
        while (PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE)) {
            if (!GetMessage(&msg, nullptr, 0, 0)) {
                //TODO  clear memory and exit
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        //do network 

        int end_time = Sys_Milliseconds();
        dtime += (end_time - start_time);

        //for get network infomation
        //TODO

        if (!g_evQue.empty())
        {
            SEvent ev = g_evQue.front();
            g_evQue.pop();
            return ev;
        }

        //没有消息了，则返回一个空消息
        SEvent ev = {};
        ev.type = SE_NONE;
        return ev;
    }

    //void SWinIO::render(const RenderInputInfo& info)
    //{
    //    PAINTSTRUCT ps;
    //    HDC hdc = BeginPaint(g_hwnd, &ps);

    //    PIXELFORMATDESCRIPTOR pfd;
    //    SetPixelFormat(hdc, 1, &pfd);
    //    //hRC = wglCreateContext(hdc); //创建OpenGL绘图环境并创建一个指向OpenGL绘制环境的句柄
    //    //wglMakeCurrent(hdc, hRC); //将传递过来的绘制环境设置为OpenGL将要进行绘制的当前绘制环境
    //    RECT rect;
    //    GetClientRect(g_hwnd, &rect);

    //    SUN->getCurLevel()->render(rect.right - rect.left, rect.bottom - rect.top, info);

    //    EndPaint(g_hwnd, &ps);
    //}

    SEvent SWinIO::getEvent() {
        SEvent ev;

        if (!g_evQue.empty())
        {
            SEvent ev = g_evQue.front();
            g_evQue.pop();
            return ev;
        }
        return ev;
    }

    void SWinIO::appActivate(bool fActive, bool minimize) {
        isMinimized_ = minimize;

        // we don't want to act like we're active if we're minimized
        if (fActive && !isMinimized_)
        {
            activeApp_ = true;
        }
        else
        {
            activeApp_ = false;
        }

        // minimize/restore mouse-capture on demand
        in_appactive_ = activeApp_;
        if (!activeApp_)
        {       
            deactivateMouse();
        }
    }

    void SWinIO::inFrame() {
        if (!activeApp_) {
            deactivateMouse();
            return;
        }

        activateMouse();

        // post events to the system que
        mouseMove();
    }

    Extend SWinIO::getFrameBufferSize() {
        return extend_;
    }

    bool SWinIO::windowShouldClose() {
        return false;
    }

    void SWinIO::swapBuffer() {
        g_graphicsLib->flush();
    }

    void SWinIO::activateMouse() {
        if (mouseActive_)
        {
            return;
        }

        mouseActive_ = true;

        IN_ActivateWin32Mouse();
    }

    void SWinIO::deactivateMouse() {
        if (!mouseActive_) {
            return;
        }
        IN_DeactivateWin32Mouse();
        mouseActive_ = false;
    }

    void SWinIO::mouseMove() {
        int mx = 0;
        int my = 0;

        IN_Win32Mouse(&mx, &my);

        if (!mx && !my) {
            return;
        }
        SEvent ev;
        ev.type = SEventType::SE_MOUSE;
        ev.value = mx;
        ev.value2 = my;
        g_evQue.push(ev);
        //Sys_QueEvent(0, SE_MOUSE, mx, my, 0, NULL);
    }

    void SWinIO::IN_ActivateWin32Mouse(void) {
        int			width, height;
        RECT		window_rect;

        width = GetSystemMetrics(SM_CXSCREEN);
        height = GetSystemMetrics(SM_CYSCREEN);

        GetWindowRect(g_hwnd, &window_rect);
        if (window_rect.left < 0)
            window_rect.left = 0;
        if (window_rect.top < 0)
            window_rect.top = 0;
        if (window_rect.right >= width)
            window_rect.right = width - 1;
        if (window_rect.bottom >= height - 1)
            window_rect.bottom = height - 1;
        window_center_x = (window_rect.right + window_rect.left) / 2;
        window_center_y = (window_rect.top + window_rect.bottom) / 2;

        SetCursorPos(window_center_x, window_center_y);

        SetCapture(g_hwnd);
        ClipCursor(&window_rect);
        while (ShowCursor(FALSE) >= 0)
            ;
    }

    void SWinIO::IN_DeactivateWin32Mouse(void)
    {
        ClipCursor(NULL);
        ReleaseCapture();
        while (ShowCursor(TRUE) < 0)
            ;
    }

    void SWinIO::IN_Win32Mouse(int* mx, int* my) {
        ::POINT		current_pos;

        // find mouse movement
        GetCursorPos(&current_pos);

        // force the mouse to the center, so there's room to move
        SetCursorPos(window_center_x, window_center_y);

        *mx = current_pos.x - window_center_x;
        *my = current_pos.y - window_center_y;
    }
}


