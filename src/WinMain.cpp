#include "Engine.h"
#include "Math/Vector2D.h"
#include "Math/Vector4D.h"
#include "Util/Util.h"
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <iomanip>
#include "Util/exception.h"
#include "Physics/BSPTree.h"
#include "Physics/polyhedron.h"
#include "Math/Random.h"
#include "Entity/GameObject.h"
#include "Entity/Level.h"

#include "windows.h"

//#include "Gui/imgui.h"
//#include "Gui/imgui_impl_opengl3.h"
//#include "Gui/imgui_impl_glfw.h"


#define USE_3


//this will be moved ,the gamedemo will be a dll TODO!
#include "Game/GameDemo2.h"

using namespace Sun;

#ifdef USE_1
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    GameDemo demo;
    demo.buildStaticModels();

    return 0;
}
#endif

//for test
#ifdef USE_2
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    int c = 13;
    for (int i = 0; i < 10000; ++i) {
        for (int j = 0; j < 10000; ++j) {
            c = c * c;
        }
    }


    Plane plane;
    plane._normal = { 0.745771527,-0.628760397,-0.220193684 };
    plane._directed_dis = 3.62392521;
    Vector3D pt = { 64.8701401,61.6870651,60.5670319 };
    int ff = plane.pointSide(pt);
    double distance = plane.distance(pt);


    //造数据
    MeteorPhysics::PolyhedronG polytope;
    //std::vector<Vector3D> inputData
     //   = { {200,50,50} ,{0,0,0},{0,100,0},{100,50,0},{0,50,100 } ,{50,150,50} };

    std::vector<Vector3D> inputData = {
        {76,68,34},
        {76,73,24},
        {51,49,19},
        {78,70,87},
        {99,12,63}//,
        //{72,27,68},
        //{13,64,29},
        //{93,47,31},
        //{25,11,84},
        //{97,34,31}
    };

    //randomSeed();
    //std::vector<Vector3D> inputData = randomIntVector3Ds(0, 100, 0, 100, 0, 100, 10);
    //static std::fstream fs("./RandomVector3Ds.txt", std::ios::out|std::ios::app);
    //fs << "///////////////////////////////new start!\n";
    //int pc = 1;
    //for (int i = 0; i < inputData.size(); ++i) {
    //    fs << "{"<< 
    //        /*std::setprecision(pc)<<*/inputData[i].x<<","<<
    //        /*std::setprecision(pc) <<*/inputData[i].y<<","<<
    //        /*std::setprecision(pc) << */inputData[i].z << "},\n";
    //}
    //fs.flush();

    bool ok = polytope.generateConvexHull(inputData);

    //MeteorPhysics::BSPTree<MeteorPhysics::Polyhedron> tree;
    //tree.build(polyVec);
    bool testok = MeteorPhysics::testPolytope(polytope);

    //随机生成凸包
    int n = 200;
    randomSeed();
    for (int i = 0; i < n; ++i) {
        std::vector<Vector3D> pts = randomVector3Ds(0, 100, 0, 100, 0, 100, 20);
        MeteorPhysics::PolyhedronG polytope;
        bool ok = polytope.generateConvexHull(pts);
        ASSERT(ok == true);
        bool testok = MeteorPhysics::testPolytope(polytope);
        ASSERT(testok == true);
    }

    //BSP split!

    return 0;
}
#endif


template<class _T>
struct MyT :public std::pair<_T,_T>
{
    _T goHome() {
        return std::pair<_T, _T>::first;
    }
};

//this is a game demo!
#ifdef USE_3
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    Sun::RunStruct stru;
    stru.hInstance = (long long)(hInstance);
    stru.preInstance = (long long)(hPrevInstance);
    stru.cmdline = lpCmdLine;
    stru.cmdshow = nCmdShow;
    stru.window_width = SCR_WIDTH;
    stru.window_height = SCR_HEIGHT;
    //stru.cmdshow = SHOW_FULLSCREEN;

    SEngine* engine = SUN;
    engine->initEnvironment(stru);
    
    //export the game FIXME ,here write hard code
    GameBase* game = new MyGame2();
    engine->loadGame(game);
    delete game;
    
    //engine->init();
    engine->run();

    return 0;
}
#endif



int main(int argc, char** argv) {

    //ImGui::CreateContext();
    //ImGui_ImplGlfw_InitForOpenGL(nullptr, true);
    //ImGui_ImplOpenGL3_Init("version");

    SEngine* engine = SUN;
    Sun::RunStruct stru;
    stru.window_width = SCR_WIDTH;
    stru.window_height = SCR_HEIGHT;
    engine->initEnvironment(stru);

    GameBase* game = new MyGame2();
    engine->loadGame(game);
    delete game;

    //engine->init();
    engine->run();

    return 0;
}