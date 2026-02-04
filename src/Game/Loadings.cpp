#include "Loadings.h"
#include "GameInclude.h"
#include <memory>

namespace Sun {

    std::vector<StaticMeshVertex> buildTriangle(const vec3& p1, const vec3& p2, const vec3& p3, bool hasNormal) {
        std::vector<StaticMeshVertex> triangle;
        vec3 normal = (p2 - p1).crossProduct(p3 - p1);
        normal.normalize();
        normal = hasNormal ? normal : vec3(0, 0, 0);
        StaticMeshVertex v;
        v.pos = p1;
        v.normal = normal;
        triangle.push_back(v);
        v.pos = p2;
        v.normal = normal;
        triangle.push_back(v);
        v.pos = p3;
        v.normal = normal;
        triangle.push_back(v);

        return triangle;
    }

    std::vector<StaticMeshVertex> buildRectangle(const vec3& p1, const vec3 p2, const vec3 p3, bool hasNormal) {
        std::vector<StaticMeshVertex> rectangle;
        std::vector<StaticMeshVertex> triangle = buildTriangle(p1, p2, p3, hasNormal);
        vec3 p4 = p2 + (p1 - p2) + (p3 - p2);
        std::vector<StaticMeshVertex> triangle2 = buildTriangle(p3, p4, p1, hasNormal);
        rectangle = std::move(triangle);
        rectangle.insert(rectangle.end(), triangle2.begin(), triangle2.end());
        return rectangle;
    }

    SharedPtr<MaterialCom> loadMaterial(int id) {
        SharedPtr<PbrMaterialCom> ptr;
        switch (id) {
            case 0: 
            {
                ptr = acquire<std::string, PbrMaterialCom>("rustediron2_basecolor.png");
                ptr->maps_[ALBEDO].source_ = getGlobalPath() + "GameResource/" + "rustediron2_basecolor.png";
                ptr->maps_[METALLIC].source_ = getGlobalPath() + "GameResource/" + "rustediron2_metallic.png";
                ptr->maps_[ROUGHNESS].source_ = getGlobalPath() + "GameResource/" + "rustediron2_roughness.png";
                ptr->maps_[NORMAL].source_ = getGlobalPath() + "GameResource/" + "rustediron2_normal.png";
            }
            break;
            case 1:
            {
                ptr = acquire<std::string, PbrMaterialCom>("gray-bricks1-albedo.png");
                ptr->maps_[ALBEDO].source_ = getGlobalPath() + "GameResource/" + "gray-bricks1-albedo.png";
                ptr->maps_[METALLIC].source_ = getGlobalPath() + "GameResource/" + "gray-bricks1-Metallic.png";
                ptr->maps_[ROUGHNESS].source_ = getGlobalPath() + "GameResource/" + "gray-bricks1-Roughness.png";
                ptr->maps_[NORMAL].source_ = getGlobalPath() + "GameResource/" + "gray-bricks1-Normal-dx.png";
            }
            break;
            default:
                break;
        }

        return ptr;
    }

    //水体网格  leftbottom->rightbottom->lefttop  leftop->rightbottom->righttop
    GameObject* loadFluid(const vec3& p1, const vec3& p2, const vec3& p3, int n, int m){
        GameObject* com = ActorFactory<GameObject>::get()->malloc();
        SharedPtr<MeshComponent> mesh(new MeshComponent());
        com->setComponent(mesh);

        std::vector<StaticMeshVertex> vec;
        std::vector<int> indices;

        vec3 normal = (p3 - p2).crossProduct(p1 - p2).getNormalized();

        vec3 dx = (p3 - p2) * (1.0 / n);
        vec3 dy = (p1 - p2) * (1.0 / m);
        for (int j = 0; j <=m; ++j) {
            for (int i = 0; i <= n; ++i) {
                vec3 p = p2 + dx * i + dy * j;
                
                StaticMeshVertex vertex;
                vertex.pos = p;
                vertex.normal = normal;
               
                vec.push_back(vertex);       
            }
        }
        for (int j = 0; j < m; ++j) {
            for (int i = 0; i < n; ++i) {
                int left_bottom= j * (n + 1) + i;
                int right_bottom = left_bottom + 1;
                int left_top = (j+1) * (n + 1) + i;
                int right_top = left_top + 1;

                indices.push_back(left_bottom);
                indices.push_back(right_bottom);
                indices.push_back(left_top);

                indices.push_back(left_top);
                indices.push_back(right_bottom);
                indices.push_back(right_top);
            }
        }
        mesh->setVertices(std::move(vec));
        mesh->setIndices(std::move(indices));

        return com;
    }

    //构建随机迷宫墙壁部分
    std::vector<GameObject*> createMaze(int m, int n, vec2 grid_siz, int start_x, int start_y, int end_x, int end_y,float h) {
        std::vector<GameObject*> vec;

        Maze maze;
        maze.init(m, n);
        maze.setStartPoint(start_x, start_y);
        maze.setEndPoint(end_x, end_y);
        maze.setGridSize(grid_siz);
        maze.randomPrim();

        SharedPtr<PbrMaterialCom> mt(new PbrMaterialCom);
        mt->maps_[ALBEDO].source_ = "E:/GIT/testproj/SunEngine/src/GameResource/ACE.jpg";
        mt->allocGpu();

        Vertex v0;
        v0.pos = { 0,0,0 };
        v0.texture = { 0,0 };
        Vertex v1;
        v1.pos = { grid_siz.x,0,0 };
        v1.texture = { 1,0 };
        Vertex v2;
        v2.pos = { 0,h,0 };
        v2.texture = { 0,1 };

        SharedPtr<MeshComponent> vMesh = buildParallelogramMeshComponent(v0, v1, v2);
        vMesh->allocGpu(EDrawMode::STATIC_DRAW);
        //生成所有水平墙壁
        for (int i = 0; i < maze.horizontals.size(); ++i) {
            if (maze.horizontals[i] == 0)continue;
            //计算墙壁左端点的坐标
            vec2 v = maze.horizontalCoordinate(i);

            //构建一个矩形gameobj
            GameObject* act = ActorFactory<GameObject>::get()->malloc();         
            act->setComponent<MeshInterface>(vMesh);
            act->setComponent<MaterialCom>(mt);
            SharedPtr<LightTransformCom> trans(new LightTransformCom);
            act->setComponent<TransformCom>(trans);
            act->move(v.x, 0, v.y);
            vec.push_back(act);

            SharedPtr<AABBCom> bv(new AABBCom());
            bv->aabb_.mins_ = vec3(v.x, 0, v.y);
            bv->aabb_.maxs_ = vec3(v.x+grid_siz.x , h, v.y);
            act->setComponent<BoundingVolumeCom>(bv);
            
            act->setShaderProgram(g_shaderManager->getShaderProgram(SP_NORMAL_PASS));
            act->setName("wall_h_" + std::to_string(i));
            act->setFaceCull(false);
        }

        Vertex w0;
        w0.pos = { 0,0,0 };
        w0.texture = { 0,0 };
        Vertex w1;
        w1.pos = {0, 0,grid_siz.y };
        w1.texture = { 1,0 };
        Vertex w2;
        w2.pos = { 0,h,0 };
        w2.texture = { 0,1 };

        SharedPtr<MeshComponent> hMesh = buildParallelogramMeshComponent(w0, w1, w2);
        hMesh->allocGpu(EDrawMode::STATIC_DRAW);

        //生成所有垂直墙壁
        for (int i = 0; i < maze.verticals.size(); ++i) {
            if (maze.verticals[i] == 0) continue;
            //计算墙壁上端点的坐标
            vec2 v = maze.verticalCoordinate(i);

            //构建一个矩形gameobj
            GameObject* act = ActorFactory<GameObject>::get()->malloc();
            act->setComponent(hMesh);
            act->setComponent(mt);
            SharedPtr<LightTransformCom> trans(new LightTransformCom);
            act->setComponent(trans);
            act->move(v.x, 0, v.y);
            vec.push_back(act);

            SharedPtr<AABBCom> bv(new AABBCom());
            bv->aabb_.mins_ = vec3(v.x, 0, v.y); //FIXME is there necessary to diff the x?
            bv->aabb_.maxs_ = vec3(v.x, h, v.y + grid_siz.y);
            act->setComponent(bv);

            act->setShaderProgram(g_shaderManager->getShaderProgram(SP_NORMAL_PASS));
            act->setName("wall_v_" + std::to_string(i));
            act->setFaceCull(false);
        }

        return vec;
    }
}