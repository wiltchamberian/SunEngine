#include "GameDemo2.h"
#include "GameInclude.h"
#include "Loadings.h"
#include "Entity/ObjLoader2.h"

void MyGame2::loadDirectLight() {
    DirectLight* light = ActorFactory<DirectLight>::get()->malloc();
    curLevel_->addActor(light);
    SharedPtr<LightTransformCom>  trans(new LightTransformCom);
    trans->setRelativeTranslation(vec3(20, 20, 20));
    light->setDirection(vec3(-20, -20, -20).getNormalized());
    light->setComponent<LightTransformCom>(trans);
    SharedPtr<MeshComponent> mesh(new MeshComponent);
    buildDefaultCube(mesh->getVertices(), mesh->getIndices(), vec4(0, 1, 0, 1));
    mesh->allocGpu(EDrawMode::STATIC_DRAW);
    light->setComponent<MeshComponent>(mesh);
    SharedPtr<PbrMaterialCom> material = buildDefaultPbrMaterial(1);
    light->setComponent<PbrMaterialCom>(material);
    material->allocGpu();

    light->setShaderProgram(g_shaderManager->getShaderProgram(SP_NORMAL_PASS));

#if 0 //显示光源的初始视锥
    GameObject* obj = buildDefaultGameObject(/*EGOF::ALLOC_MESH| */EGOF::ALLOC_MATERIAL | EGOF::DEFAULT_TRANSFORM);
    obj->setName("meshViewVolume");
    //obj->setRelativeTransformation(Transform3D().getTranslate(vec3(2.0, 0.5, -1.0)));
    
    SharedPtr<MeshComponent> vmesh = light->generateViewVolumeMesh();
    obj->setComponent(vmesh);
    vmesh->allocGpu(EDrawMode::STATIC_DRAW);
    curLevel_->addActor(obj);
#endif

    return;
}

void MyGame2::loadGround() {
    GameObject* act = createGround("E:/GIT/testproj/SunEngine/src/GameResource/floor.jpg",1);
    curLevel_->addActor(act, true);
}

void MyGame2::loadGravity() {
    //加入重力场
    FieldCreateInfo createinfo;
    createinfo.level = curLevel_;
    createinfo.accel = vec3(0, -3, 0); //10 m/s^2
    FieldSystem* gravity = new FieldSystem(createinfo);
    curLevel_->addSystem(gravity);
}

void MyGame2::loadStandfordBunny() {
    Sun::PlyLoader plyLoader;
    plyLoader.setDefaultColor({ 0.3,0.7,0.3,0.5 });
    GameObject* act = plyLoader.load("D:/Download/bunny.tar/bunny/reconstruction/bun_zipper.ply");
    act->setShaderProgram(g_shaderManager->getShaderProgram(SP_NORMAL_PASS));
    //plyLoader.load("D:/GIT/testproj/SunEngine/src/GameResource/dragon_stand/dragonStandRight_0.ply");
    //plyLoader.load("D:/GIT/testproj/SunEngine/src/GameResource/dragon_stand/dragonStandRight_24.ply");
    //plyLoader.load("D:/GIT/testproj/SunEngine/src/GameResource/dragon_stand/dragonStandRight_48.ply");
    //plyLoader.load("D:/GIT/testproj/SunEngine/src/GameResource/dragon_stand/dragonStandRight_72.ply");
    //plyLoader.load("D:/GIT/testproj/SunEngine/src/GameResource/dragon_stand/dragonStandRight_96.ply");
    //plyLoader.load("D:/GIT/testproj/SunEngine/src/GameResource/dragon_stand/dragonStandRight_120.ply");
    //plyLoader.load("D:/GIT/testproj/SunEngine/src/GameResource/dragon_stand/dragonStandRight_144.ply");
    //plyLoader.load("D:/GIT/testproj/SunEngine/src/GameResource/dragon_stand/dragonStandRight_168.ply");
    //plyLoader.load("D:/GIT/testproj/SunEngine/src/GameResource/dragon_stand/dragonStandRight_192.ply");
    //plyLoader.load("D:/GIT/testproj/SunEngine/src/GameResource/dragon_stand/dragonStandRight_216.ply");
    //plyLoader.load("D:/GIT/testproj/SunEngine/src/GameResource/dragon_stand/dragonStandRight_240.ply");
    //plyLoader.load("D:/GIT/testproj/SunEngine/src/GameResource/dragon_stand/dragonStandRight_264.ply");
    //plyLoader.load("D:/GIT/testproj/SunEngine/src/GameResource/dragon_stand/dragonStandRight_288.ply");
    //plyLoader.load("D:/GIT/testproj/SunEngine/src/GameResource/dragon_stand/dragonStandRight_312.ply");
    //plyLoader.load("D:/GIT/testproj/SunEngine/src/GameResource/dragon_stand/dragonStandRight_336.ply");

    act->setName("bunny");
    act->move(0, 0, -5);
    act->scale(10, 10, 10);
    curLevel_->addActor(act);

    //加载斯坦福旋转动画
    YRotateSystem* sys = new YRotateSystem();
    sys->actor_ = act;
    curLevel_->addSystem(sys);
}

void MyGame2::loadLights() {
    auto act0 = buildPointLight({ 1000.0,1000.0,1000.0 }, { -10.0,10.0,10.0 });
    auto act1 = buildPointLight({ 300.0,300.0,300.0 }, { 10.0,10.0,10.0 });
    auto act2 = buildPointLight({ 300.0,300.0,300.0 }, { -10.0,-10.0,10.0 });
    auto act3 = buildPointLight({ 300.0,300.0,300.0 }, { 10.0,-10.0,10.0 });
    act0->setName("light0");
    act1->setName("light1");
    act2->setName("light2");
    act3->setName("light3");
    curLevel_->addActors({ act0,act1,act2,act3 });
    curLevel_->getMsgCenter()->addGameObjects({ act0, act1, act2, act3 });
}

void MyGame2::loadSkeletalModel() {
    Sun::GltfLoader2 gltfloader;
    gltfloader.loadScene("E:\\GIT\\testproj\\AssimpTest\\game\\femal_walk.gltf");
    std::vector<GameObject*> vec = gltfloader.exportActors();
    std::vector<SystemBase*> sys = gltfloader.exportSystems();
    curLevel_->addActors(vec);
    curLevel_->addSystems(sys);
}

void MyGame2::loadPointCloud() {
#if 0
    vec3 translate = { 0.8,1.6,-1 };
    PointLightComponent* ptLight = new PointLightComponent();
    ptLight->color_ = { 1.0,1.0,1.0 };
    CubeComponent* cubeCom = new CubeComponent({ 0,0,0 }, 0.1);

    GameObject* act = ActorFactory<GameObject>::get()->malloc();
    act->addComponent(ptLight);
    act->addComponent(cubeCom);
    Transform3D trans;
    trans.setTranslation(translate);
    act->setRelativeTransformation(trans);

    curLevel_->addObject(act);
    curLevel_->getMsgCenter()->addComponent(ptLight);
#endif
}

void MyGame2::loadMaze() {
    //加载迷宫
    std::vector<GameObject*> acts = createMaze(100, 100, vec2(3, 3), 0, 0, 99, 99, 3);
    for (int i = 0; i < acts.size(); ++i) {
        curLevel_->addActor(acts[i], true);
    }
}

void MyGame2::loadTetrahedron() {
    GameObject* act = ActorFactory<GameObject>::get()->malloc();
    SharedPtr<MeshComponent> mesh(new MeshComponent);
    act->setComponent<MeshInterface>(mesh);

    //正四面体
    StaticMeshVertex vertex;
    std::vector<vec3> pos = { {0,0,0},{-0.5,0,-1},{0.5,0,-1},{0,1,-0.5} };
    vec3 move = { 0,-1,0 };
    for (int i = 0; i < pos.size(); ++i) {
        pos[i] = pos[i] + move;
    }
    vec4 color(1, 0, 0, 1);
    std::vector<Vertex> vertices;
    for (int i = 0; i < pos.size(); ++i) {
        vertex.pos = pos[i];
        vertex.color = color;
        vertices.push_back(vertex);
    }
    std::vector<int> vs = { 0,1,2,0,3,1,2,3,0 };
    std::vector<int> indices;
    for (int i = 0; i < vs.size(); ++i) {
        indices.push_back(vs[i]);
    }
    mesh->setVertices(std::move(vertices));
    mesh->setIndices(std::move(indices));

    curLevel_->addActor(act);
}

void MyGame2::loadPool() {
    const int length = 5;
    const int width = 5;
    const int height = 1;
    const vec3 bottomCenter = vec3(0, 0, 0);

    //加载游泳池
    GameObject* obj = ActorFactory<GameObject>::get()->malloc();
    SharedPtr<MeshComponent> mesh(new MeshComponent);
    obj->setComponent<MeshInterface>(mesh);

    vec3 p1 = { length / 2,0,width / 2 };
    vec3 p2 = { length / 2, 0, -width / 2 };
    vec3 p3 = { -length / 2,0,-width / 2 };
    vec3 p4 = { -length / 2, 0 , width / 2 };
    vec3 p5 = p1 + vec3(0, height, 0);
    vec3 p6 = p2 + vec3(0, height, 0);
    vec3 p7 = p3 + vec3(0, height, 0);
    vec3 p8 = p4 + vec3(0, height, 0);

    std::vector<Vertex> vertices;
    StaticMeshVertex vertex;
    vertex.pos = p1;
    vertex.normal = { -1,0,0 };
    vertex.texture = { 0,0 };
    vertices.push_back(vertex);

    vertex.pos = p5;
    vertex.normal = { -1,0,0 };
    vertex.texture = { 0,1 };
    vertices.push_back(vertex);

    vertex.pos = p2;
    vertex.normal = { -1,0,0 };
    vertex.texture = { 1,0 };
    vertices.push_back(vertex);

    vertex.pos = p2;
    vertex.normal = { -1,0,0 };
    vertex.texture = { 1,0 };
    vertices.push_back(vertex);

    vertex.pos = p5;
    vertex.normal = { -1,0,0 };
    vertex.texture = { 0, 1 };
    vertices.push_back(vertex);

    vertex.pos = p6;
    vertex.normal = { -1,0,0 };
    vertex.texture = { 1, 1 };
    vertices.push_back(vertex);


    //face
    vertex.pos = p4;
    vertex.normal = { 0,0,-1 };
    vertex.texture = { 0,0 };
    vertices.push_back(vertex);

    vertex.pos = p8;
    vertex.normal = { 0,0,-1 };
    vertex.texture = { 0,1 };
    vertices.push_back(vertex);

    vertex.pos = p5;
    vertex.normal = { 0,0,-1 };
    vertex.texture = { 1,1 };
    vertices.push_back(vertex);

    vertex.pos = p5;
    vertex.normal = { 0,0,-1 };
    vertex.texture = { 1,1 };
    vertices.push_back(vertex);

    vertex.pos = p1;
    vertex.normal = { 0,0,-1 };
    vertex.texture = { 1, 0 };
    vertices.push_back(vertex);

    vertex.pos = p4;
    vertex.normal = { 0,0,-1 };
    vertex.texture = { 0, 0 };
    vertices.push_back(vertex);

    //face
    vertex.pos = p3;
    vertex.normal = { 1,0,0 };
    vertex.texture = { 0,0 };
    vertices.push_back(vertex);

    vertex.pos = p7;
    vertex.normal = { 1,0,0 };
    vertex.texture = { 0,1 };
    vertices.push_back(vertex);

    vertex.pos = p8;
    vertex.normal = { 1,0,0 };
    vertex.texture = { 1,1 };
    vertices.push_back(vertex);

    vertex.pos = p8;
    vertex.normal = { 1,0,0 };
    vertex.texture = { 1,1 };
    vertices.push_back(vertex);

    vertex.pos = p4;
    vertex.normal = { 1,0,0 };
    vertex.texture = { 1, 0 };
    vertices.push_back(vertex);

    vertex.pos = p3;
    vertex.normal = { 1,0,0 };
    vertex.texture = { 0, 0 };
    vertices.push_back(vertex);

    //face
    vertex.pos = p2;
    vertex.normal = { 0,0,1 };
    vertex.texture = { 0,0 };
    vertices.push_back(vertex);

    vertex.pos = p6;
    vertex.normal = { 0,0,1 };
    vertex.texture = { 0,1 };
    vertices.push_back(vertex);

    vertex.pos = p7;
    vertex.normal = { 0,0,1 };
    vertex.texture = { 1,1 };
    vertices.push_back(vertex);

    vertex.pos = p7;
    vertex.normal = { 0,0,1 };
    vertex.texture = { 1,1 };
    vertices.push_back(vertex);

    vertex.pos = p3;
    vertex.normal = { 0,0,1 };
    vertex.texture = { 1, 0 };
    vertices.push_back(vertex);

    vertex.pos = p2;
    vertex.normal = { 0,0,1 };
    vertex.texture = { 0, 0 };
    vertices.push_back(vertex);

    for (int i = 0; i < vertices.size(); ++i) {
        vertices[i].pos += bottomCenter;
    }
    mesh->setVertices(std::move(vertices));

    obj->setComponent<MaterialCom>(loadMaterial(1));


    obj->setShaderProgram(g_shaderManager->getShaderProgram(SP_NORMAL_PASS));
    obj->setName("pool");

    curLevel_->addActor(obj);
}

void MyGame2::loadFluid() {
    //加载水体
    float mu = 0.; //无粘滞度
    float c = 0.5; //每秒0.5m

    GameObjectCreateInfo createInfo;
    createInfo.name = "fluid2d";
    createInfo.transformType = EComType::LIGHT_TRANSFORM_COM;
    createInfo.transform = Transform3D({ -2.5,0,-2.5 }, Quaternion(), { 1,1,1, });
    GameObject* act = ActorFactory<GameObject>::get()->malloc(createInfo);

    act->setShaderProgram(g_shaderManager->getShaderProgram(SP_NORMAL_PASS));

    SharedPtr<Fluid2dComponent> mesh(new Fluid2dComponent(10, 10, 0.5, 0.1, c, mu, 0.5, 1.3));
    mesh->hFluidTex.source_ = "E:/GIT/testproj/SunEngine/src/GameResource/floor.jpg";
    g_graphicsCache->applyForTexture(mesh->hFluidTex.source_, mesh->hFluidTex);
    act->setComponent<MeshInterface>(mesh);

    curLevel_->addActor(act);
}

void MyGame2::loadSpheres() {
    //加载球体们
    int nRows = 1;
    int nColumns = 1;
    float spacing = 2.5;

    //公共纹理
    Texture albedoMap;
    Texture metallicMap;
    Texture roughnessMap;
    Texture normalMap;
    albedoMap.source_ = getGlobalPath() + "GameResource/" + "rustediron2_basecolor.png";
    metallicMap.source_ = getGlobalPath() + "GameResource/" + "rustediron2_metallic.png";
    roughnessMap.source_ = getGlobalPath() + "GameResource/" + "rustediron2_roughness.png";
    normalMap.source_ = getGlobalPath() + "GameResource/" + "rustediron2_normal.png";

    for (int i = 0; i < nRows; ++i) {
        for (int j = 0; j < nColumns; ++j) {
            GameObject* gameObject = ActorFactory<GameObject>::get()->malloc();
            SharedPtr<MeshComponent> mesh = buildSphereMesh();
            gameObject->setComponent<MeshInterface>(mesh);

            SharedPtr<PbrMaterialCom> material = buildDefaultPbrMaterial();
            gameObject->setComponent<MaterialCom>(material);
            material->pbrModel_ = PBRModel::METALLIC_ROUGHNESS;

            material->maps_[ALBEDO] = albedoMap;
            material->maps_[METALLIC] = metallicMap;
            material->maps_[ROUGHNESS] = roughnessMap;
            material->maps_[NORMAL] = normalMap;
    
            Transform3D trans;
            trans.translate(vec3((j - (nColumns / 2)) * spacing, (i - (nRows / 2)) * spacing, -2.0f));
            gameObject->setRelativeTransformation(trans);

            curLevel_->addActor(gameObject);
        }
    }
    

}

void MyGame2::loadGroundRotateAnimation() {
    GroundRotateSystem* sys = new GroundRotateSystem();
    sys->rotor_ = world_->getCameraCom();
    curLevel_->addSystem(sys);
}

void MyGame2::loadParticles() {
    SharedPtr<ParticleComponent> com(new ParticleComponent());
    GameObject* act = ActorFactory<GameObject>::get()->malloc();
    act->setComponent<MeshInterface>(com);
    curLevel_->addActor(act);
}

void MyGame2::loadSkyPicture() {
    std::string path = "E:/GIT/testproj/SunEngine/src/GameResource/toyou.jpg";

    GameObject* act = ActorFactory<GameObject>::get()->malloc();
    SharedPtr<MeshComponent> mesh(new MeshComponent);
    act->setComponent<MeshInterface>(mesh);

    //100m * 100m 的地面方形，中心位于(0,0,0),水平放置(x-z平面)其中每个纹理占1m*1m,假定1m对应的就是引擎尺度的1
    std::vector<Vertex> vertices;
    StaticMeshVertex vertex;
    vertex.color = { 0.0,0.0,0.0,0.0 };
    vertex.normal = { 0,1,0 };
    float dx = 1.0;
    float dz = 1.0;
    float R = 4;
    float dzz = 3.0;
    vertex.pos = { -dx,R,-dz + dzz };
    vertex.texture = { 0,0 };
    vertices.push_back(vertex);

    vertex.pos = { dx,R,-dz + dzz };
    vertex.texture = { 1,0 };
    vertices.push_back(vertex);

    vertex.pos = { -dx,R,dz + dzz };
    vertex.texture = { 0,1 };
    vertices.push_back(vertex);

    /// //////////////////////
    vertex.pos = { -dx,R,dz + dzz };
    vertex.texture = { 0,1 };
    vertices.push_back(vertex);

    vertex.pos = { dx,R,-dz + dzz };
    vertex.texture = { 1,0 };
    vertices.push_back(vertex);

    vertex.pos = { dx,R,dz + dzz };
    vertex.texture = { 1,1 };
    vertices.push_back(vertex);

    mesh->setVertices(std::move(vertices));

    SharedPtr<PbrMaterialCom> material(new PbrMaterialCom);
    material->maps_[ALBEDO].source_ = path;
    act->setComponent<MaterialCom>(material);

    curLevel_->addActor(act);

    return ;
}

void MyGame2::loadStandFordDragon() {
    ObjLoader2 loader;
    std::string dir = getResourceDir();
    bool ok = loader.load((dir + "dragon.obj").c_str());
    if (ok == false) {
        assert(false);
        return;
    }
   
}

SWorld* MyGame2::createWorld() {

    world_ = loadDefaultWorld();
    curLevel_ = world_->getCurLevel();

    loadDirectLight();

    //auto obj = createDefaultCube();
    //obj->moveUp(3);
    //curLevel_->addActor(obj);

    //loadGravity();
    loadGround();
    //loadPool();
    //loadFluid();
    //loadSpheres();
    //loadSkyPicture();
    //loadParticles();
    loadStandfordBunny();
    loadLights();
    //loadMaze();
    //loadSkeletalModel();
    //loadTetrahedron();
    //loadGroundRotateAnimation();



    //sp<SActor> computeRotor = level->createActor<SActor>();
    //RayTracingComponent* testCom = new RayTracingComponent();
    //computeRotor->addComponent(testCom);
    //level->addObject(computeRotor);
    
    world_->save("./DemoGame2.world");

    return world_;
}