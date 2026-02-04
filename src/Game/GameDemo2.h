/*****************************************************************************
* @brief : GameDemo2
* @author : acedtang
* @date : 2021/4/20
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __GAME_DEMO_2
#define __GAME_DEMO_2

#include "Engine.h"
#include "Component/MeshComponent.h"
using namespace Sun;

#include <vector>

extern MeshComponent* buildRegularTetrahedron();

class MyGame2 :public GameBase
{
public: 
    SWorld* createWorld() override;

protected:
    SWorld* world_ = nullptr;
    SLevel* curLevel_ = nullptr;
    void loadDirectLight();
    void loadGround();
    void loadGravity();
    void loadStandfordBunny();
    void loadLights();
    void loadSkeletalModel();
    void loadPointCloud();
    void loadMaze();
    void loadTetrahedron();
    void loadPool();
    void loadFluid();
    void loadSpheres();
    void loadGroundRotateAnimation();
    void loadParticles();
    void loadSkyPicture();
    void loadStandFordDragon();
};  

#endif