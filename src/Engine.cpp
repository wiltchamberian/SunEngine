// Sun.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//必须放在最上面，否则找不到符号!fuck ,被其他的宏影响了吗？
//#include "Gui/imgui.h"
//#include "Gui/imgui_impl_opengl3.h"
//#include "Gui/imgui_impl_glfw.h"

#include <thread>

#include "Engine.h"
#include "Audio/Audio.h"
#include "PCH.h"
#include "Common/Common.h"
#include "Common/CVarCenter.h"
#include "Platform/PlatformIO.h"
#include "Entity/Level.h"
#include "Util/exception.h"
#include "Util/Singleton.h"
#include "Util/Util.h"
#include "Util/Clock.h"
#include "Render/GraphicsLib.h"
#include "Render/GraphicsProxy.h"
//#include "RenderHelper.h"
//#include "Render/RenderFactory.h"
#include "Render/GraphicsLibFactory.h"
#include "Render/ShaderManager.h"


namespace Sun
{
    // settings
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;

    SEngine* SEngine::_instance = new SEngine;

    SEngine::SEngine()
    {
    }

    SEngine::~SEngine()
    {
    }

    void SEngine::initEnvironment(const RunStruct& stru)
    {
        GRAPHICS_API_TYPE apiType = globalGetGraphics_API_Type();
        g_graphicsLib = GraphicsLibFactory::createGraphicsLib(apiType);
        g_graphicsProxy = GraphicsLibFactory::createGraphicsProxy(apiType);
        g_shaderManager = new ShaderManager();
        g_cvarCenter.reset(new CVarCenter());

        g_audioEngine = Audio::createAudioEngine();
        if (g_audioEngine) {
            bool ok = g_audioEngine->initEnvironment();
            if (!ok) {
                assert(false);
            }
        }

        //初始化glfw ，窗口 以及opengl库
        if (g_platformIO)
            g_platformIO->initEnvironment(stru);
        else {
            g_platformIO = PlatformIO::createPlatformIO();
            if (g_platformIO == nullptr) {
                assert(false);
            }
            g_platformIO->initEnvironment(stru);
        }
           
       
        ASSERT(g_graphicsLib != nullptr);
        ASSERT(g_shaderManager != nullptr);
        //ASSERT(g_graphicsProxy != nullptr);
        if (g_graphicsLib && g_shaderManager/* && g_graphicsProxy*/) {
            //g_graphicsLib->bindWindow((void*)(hWnd));
            g_graphicsLib->load();
            g_shaderManager->createShaders();
            //g_graphicsProxy->recreateGBuffer(SCR_WIDTH, SCR_HEIGHT);
        }

        //初始化gui
        //ImGui::CreateContext();
        //ImGui_ImplGlfw_InitForOpenGL(g_platformIO->getGlfwWindow(), true);
        //const char* glsl_version = "#version 430";
        //ImGui_ImplOpenGL3_Init(glsl_version);
 
    }

    void SEngine::tick(float dtime,const SEvent& ev) {
        world_->tick(dtime, ev);
    }

    void SEngine::svFrame()
    {
        if (!g_is_server)
            return;
    }

    void SEngine::clFrame()
    {
        if (g_is_server)
            return;

        //发消息给服务端
        sendMsgToServer();

        //刷新操作
        //display();
    }

    void SEngine::sendMsgToServer()
    {

    }

    SEngine* SEngine::get()
    {
        //TODO, revise to singleton
        return _instance;
    }

    void SEngine::loadGame(GameBase* game) {
        if (game) {
            world_ = game->createWorld();

            SLevel* level = world_->getCurLevel();

            if(level)
                level->prepareRender();
        }
    }

    Sun::SWorld* SEngine::getWorld()
    {
        return world_;
    }

    SLevel* SEngine::getCurLevel() {
        if (world_) {
            return world_->getCurLevel();
        }
        return nullptr;
    }

    void SEngine::run()
    {
        float lastTime = 0;
        const float fps = 60;
        float framePeriod = 1000 / fps;
        float sleepDuration = 1;// 1ms
        if (!g_platformIO) {
            ASSERT(false);
            return;
        }
        // 加入初始化逻辑
        world_->prepare();


        while (!g_platformIO->windowShouldClose()) {
            bool close = g_platformIO->windowShouldClose();

            float startTime = Sys_Milliseconds();
            ///////////////////////更新游戏逻辑以及渲染////////////////
            //获取网络快照
            //事件
            //获取本地事件
            SEvent ev = g_platformIO->getEvent();


            //服务器网络接收处理
            svFrame();

            //客户端网络接收处理
            clFrame();

            //更新对象状态
            int time = Sys_Milliseconds();
            //to second
            float dtime = (time - lastTime) * 0.001;
            lastTime = time;
            tick(dtime, ev);

            /////////////////////////////////////////////////////////

            //////////////////////////系统输入处理/////////////////////////
            //鼠标事件单独捕获
            //platformIO_->inFrame();
            g_platformIO->generateEvents();

            //控制帧率，为了防止sleep超时，采用分段小间隔sleep的方式
            //到临近帧周期时，不间断轮询判断
            int endTime = Sys_Milliseconds();
            int frameTime = endTime - startTime;
            do{
                if (framePeriod - frameTime > sleepDuration) {
                    std::this_thread::sleep_for(std::chrono::milliseconds((int)sleepDuration));
                }
                else {
                    while (framePeriod > frameTime) {
                        endTime = Sys_Milliseconds();
                        frameTime = endTime - startTime;
                    }
                    break;
                }
                //生成本地事件(执行系统消息循环，可能生成多个事件)
                //platformIO_->generateEvents();

                //
                endTime = Sys_Milliseconds();
                frameTime = endTime - startTime;
            } while (true);
        }

        g_platformIO->terminate();
    }

}
