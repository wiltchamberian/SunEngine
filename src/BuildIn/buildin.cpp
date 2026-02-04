#include "buildin.h"
#include "Math/Vector3D.h"
#include "Math/Geometry.h"
#include "Util/Factory.h"
#include "Component/LightTransformCom.h"
#include "Component/AABBCom.h"
#include "Util/QuickVector.h"
#include "Entity/World.h"
#include "Entity/GameObject.h"
#include "Entity/Missile.h"
#include "Entity/PointLight.h"
#include "Render/ShaderManager.h"
#include "Util/Crc.h"
#include "Assets/ResourceManager.h"

namespace Sun {
   
    SharedPtr<SimpleTransformCom> buildSimpleTransform(const SimpleTransformCreateInfo& createInfo) {
        SharedPtr<SimpleTransformCom> ptr(new SimpleTransformCom(createInfo));
        return ptr;
    }

    SharedPtr<LightTransformCom> buildLightTransform(const LightTransformCreateInfo& createInfo) {
        SharedPtr<LightTransformCom> ptr(new LightTransformCom(createInfo));
        return ptr;
    }

    SharedPtr<SimpleMaterialCom> buildSimpleMaterial(const SimpleMaterialCreateInfo& createInfo) {
        return SharedPtr<SimpleMaterialCom>(new SimpleMaterialCom(createInfo));
    }

    SharedPtr<PbrMaterialCom> buildDefaultPbrMaterial(int id) {
        std::string prefix;
        std::string albedo;
        std::string ao;
        std::string roughness;
        std::string normal;
        std::string metallic;
        if (id == 0) {
            prefix = "Copper-scuffed_Unreal-Engine/";
            albedo = getResourceDir() + prefix+ "Copper-scuffed_basecolor.png";
            metallic = getResourceDir()+prefix+ "Copper-scuffed_metallic.png";
            roughness = getResourceDir()+prefix+ "Copper-scuffed_roughness.png";
            normal = getResourceDir()+prefix+ "Copper-scuffed_normal.png";
        }
        else if (id == 1) {
            prefix = "bamboo-wood-semigloss-Unreal-Engine/";
            albedo = getResourceDir() + prefix + "bamboo-wood-semigloss-albedo.png";
            metallic = getResourceDir() + prefix + "bamboo-wood-semigloss-metal.png";
            roughness = getResourceDir() + prefix + "bamboo-wood-semigloss-roughness.png";
            normal = getResourceDir() + prefix + "bamboo-wood-semigloss-normal.png";
        }
        else {
            assert(false);
        }

        std::string str = albedo;
        Hash hash = Crc::StrCrc32(str.c_str(), str.size());
        auto mgr = getResourceMgr();
        if (mgr->hasElement(hash)) {
            return mgr->getElement<PbrMaterialCom>(hash);
        }

        SharedPtr<PbrMaterialCom> ptr = mgr->createElement<PbrMaterialCom>(hash);

        ptr->maps_[ALBEDO].source_ = albedo;
        ptr->maps_[METALLIC].source_ = metallic;
        ptr->maps_[ROUGHNESS].source_ = roughness;
        ptr->maps_[NORMAL].source_ = normal;

        return ptr;
    }

    SharedPtr<PbrMaterialCom> buildPbrMaterial(const PbrMaterialCreateInfo& createInfo) {
        auto mgr = getResourceMgr();
        Hash hash = Crc::StrCrc32(createInfo.albedo_texture_path.c_str(), createInfo.albedo_texture_path.size());
        if (mgr->hasElement(hash)) {
            return mgr->getElement<PbrMaterialCom>(hash);
        }
        SharedPtr<PbrMaterialCom> ptr = mgr->createElement<PbrMaterialCom>(hash);

        ptr->maps_[ALBEDO].source_ = createInfo.albedo_texture_path;
        ptr->maps_[METALLIC].source_ = createInfo.metallic_texture_path;
        ptr->maps_[ROUGHNESS].source_ = createInfo.roughness_texture_path;
        ptr->maps_[NORMAL].source_ = createInfo.normal_texture_path;

        return ptr;
    }

    /*
    *     v4 --   v7               y
    *    /|     / |                |
       v0 v5 --v3-v6               0 --x
        |/     | /                /
       v1  --  v2                z
    */
    void buildCube(std::vector<Vertex>& vertices, std::vector<int>& indices, const AABB& box, const vec4& color) {
        Vertex vertex;

        vec3 v0 = { box.mins_.x, box.maxs_.y ,box.maxs_.z };
        vec3 v1 = { box.mins_.x , box.mins_.y, box.maxs_.z };
        vec3 v2 = { box.maxs_.x , box.mins_.y, box.maxs_.z };
        vec3 v3 = { box.maxs_.x ,box.maxs_.y ,box.maxs_.z };
        vec3 v4 = { box.mins_.x,box.maxs_.y,box.mins_.z };
        vec3 v5 = { box.mins_.x,box.mins_.y,box.mins_.z };
        vec3 v6 = { box.maxs_.x, box.mins_.y, box.mins_.z };
        vec3 v7 = { box.maxs_.x,box.maxs_.y,box.mins_.z };

        int siz = vertices.size();
        int siz2 = indices.size();

        vertex.color = color;
        //front 
        vertex.pos = v0;
        vertex.normal = { 0,0,1 };
        vertices.push_back(vertex);
        vertex.pos = v1;
        vertices.push_back(vertex);
        vertex.pos = v2;
        vertices.push_back(vertex);
        vertex.pos = v0;
        vertices.push_back(vertex);
        vertex.pos = v2;
        vertices.push_back(vertex);
        vertex.pos = v3;
        vertices.push_back(vertex);

        //left
        vertex.pos = v4;
        vertex.normal = { -1,0,0 };
        vertices.push_back(vertex);
        vertex.pos = v5;
        vertices.push_back(vertex);
        vertex.pos = v1;
        vertices.push_back(vertex);
        vertex.pos = v4;
        vertices.push_back(vertex);
        vertex.pos = v1;
        vertices.push_back(vertex);
        vertex.pos = v0;
        vertices.push_back(vertex);

        //right 
        vertex.pos = v3;
        vertex.normal = { 1,0,0 };
        vertices.push_back(vertex);
        vertex.pos = v2;
        vertices.push_back(vertex);
        vertex.pos = v6;
        vertices.push_back(vertex);
        vertex.pos = v3;
        vertices.push_back(vertex);
        vertex.pos = v6;
        vertices.push_back(vertex);
        vertex.pos = v7;
        vertices.push_back(vertex);

        //up
        vertex.pos = v3;
        vertex.normal = { 0,1,0 };
        vertices.push_back(vertex);
        vertex.pos = v7;
        vertices.push_back(vertex);
        vertex.pos = v4;
        vertices.push_back(vertex);
        vertex.pos = v3;
        vertices.push_back(vertex);
        vertex.pos = v4;
        vertices.push_back(vertex);
        vertex.pos = v0;
        vertices.push_back(vertex);

        //down
        vertex.pos = v1;
        vertex.normal = { 0,-1,0 };
        vertices.push_back(vertex);
        vertex.pos = v5;
        vertices.push_back(vertex);
        vertex.pos = v6;
        vertices.push_back(vertex);
        vertex.pos = v1;
        vertices.push_back(vertex);
        vertex.pos = v6;
        vertices.push_back(vertex);
        vertex.pos = v2;
        vertices.push_back(vertex);

        //back
        vertex.pos = v7;
        vertex.normal = { 0,0,-1 };
        vertices.push_back(vertex);
        vertex.pos = v6;
        vertices.push_back(vertex);
        vertex.pos = v4;
        vertices.push_back(vertex);
        vertex.pos = v4;
        vertices.push_back(vertex);
        vertex.pos = v6;
        vertices.push_back(vertex);
        vertex.pos = v5;
        vertices.push_back(vertex);

        /*for (int i = siz; i < vertices.size(); ++i) {
            indices.push_back(i);
        }*/

    }

    void buildDefaultCube(std::vector<Vertex>& vertices, std::vector<int>& indices, const vec4& color) {
        Vertex vertex;
        vertex.color = color;
        vec3 uint[3] = { vec3(1,0,0),vec3(0,1,0),vec3(0,0,1) };
        vec3 vec[8];
        int index[4];
        //eight vertex
        for (int i = 0; i < 8; ++i) {
            vec[i] = vec3(0, 0, 0);
            for (int k = 0; k < 3; ++k) {
                vec[i] += uint[k] * (((i >> k) & 1) >> 1 - 1) * 0.5;
            }
        }
        //six sides
        for (int i = 0; i < 6; ++i) {
            int axis = i >> 1;
            int dir = i & 1;
            int axis1 = (axis + 1)%3;
            int axis2 = (axis + 2)%3;
            for (int j = 0; j < 4; ++j) {
                index[j] = (dir << axis) + (j&1) << axis1 + ((j>>1)&1) << axis2;
            }
            vertex.normal = uint[axis] * (dir * 2-1);
            for (int k = 0; k < 3; ++k) {
                int ind = (dir > 0) ? k : (2 - k);
                vertex.pos = vec[index[ind]];
                vertices.push_back(vertex);
            }
            for (int k = 3; k >=1 ; --k) {
                int ind = dir > 0 ? k : (4 - k);
                vertex.pos = vec[index[ind]];
                vertices.push_back(vertex);
            }
        }
        return;
    }

#if 0
	void buildDefaultCube(std::vector<Vertex>& vertices, std::vector<int>& indices , const vec4& color) {
        Vertex vertex;
        //std::vector<StaticMeshVertex> vertices;
        //std::vector<int> indices;
        float halfLen = 0.5;
        //sides
        vec3 v0 = { -halfLen,halfLen,halfLen };
        vec3 v1 = { -halfLen , -halfLen, halfLen };
        vec3 v2 = { halfLen, -halfLen, halfLen };
        vec3 v3 = { halfLen, halfLen, halfLen };
        vec3 v4 = { -halfLen , halfLen, -halfLen };
        vec3 v5 = { -halfLen, -halfLen, -halfLen };
        vec3 v6 = { halfLen, -halfLen, -halfLen };
        vec3 v7 = { halfLen, halfLen, -halfLen };

        vertex.color = color;
        //front 
        vertex.pos = v0;
        vertex.normal = { 0,0,1 };
        vertices.push_back(vertex);
        vertex.pos = v1;
        vertices.push_back(vertex);
        vertex.pos = v2;
        vertices.push_back(vertex);
        vertex.pos = v0;
        vertices.push_back(vertex);
        vertex.pos = v2;
        vertices.push_back(vertex);
        vertex.pos = v3;
        vertices.push_back(vertex);

        //left
        vertex.pos = v4;
        vertex.normal = { -1,0,0 };
        vertices.push_back(vertex);
        vertex.pos = v5;
        vertices.push_back(vertex);
        vertex.pos = v1;
        vertices.push_back(vertex);
        vertex.pos = v4;
        vertices.push_back(vertex);
        vertex.pos = v1;
        vertices.push_back(vertex);
        vertex.pos = v0;
        vertices.push_back(vertex);

        //right 
        vertex.pos = v3;
        vertex.normal = { 1,0,0 };
        vertices.push_back(vertex);
        vertex.pos = v2;
        vertices.push_back(vertex);
        vertex.pos = v6;
        vertices.push_back(vertex);
        vertex.pos = v3;
        vertices.push_back(vertex);
        vertex.pos = v6;
        vertices.push_back(vertex);
        vertex.pos = v7;
        vertices.push_back(vertex);

        //up
        vertex.pos = v3;
        vertex.normal = { 0,1,0 };
        vertices.push_back(vertex);
        vertex.pos = v7;
        vertices.push_back(vertex);
        vertex.pos = v4;
        vertices.push_back(vertex);
        vertex.pos = v3;
        vertices.push_back(vertex);
        vertex.pos = v4;
        vertices.push_back(vertex);
        vertex.pos = v0;
        vertices.push_back(vertex);

        //down
        vertex.pos = v1;
        vertex.normal = { 0,-1,0 };
        vertices.push_back(vertex);
        vertex.pos = v5;
        vertices.push_back(vertex);
        vertex.pos = v6;
        vertices.push_back(vertex);
        vertex.pos = v1;
        vertices.push_back(vertex);
        vertex.pos = v6;
        vertices.push_back(vertex);
        vertex.pos = v2;
        vertices.push_back(vertex);

        //back
        vertex.pos = v7;
        vertex.normal = { 0,0,-1 };
        vertices.push_back(vertex);
        vertex.pos = v6;
        vertices.push_back(vertex);
        vertex.pos = v4;
        vertices.push_back(vertex);
        vertex.pos = v4;
        vertices.push_back(vertex);
        vertex.pos = v6;
        vertices.push_back(vertex);
        vertex.pos = v5;
        vertices.push_back(vertex);

        //for (int i = 0; i < vertices.size(); ++i) {
        //    indices.push_back(i);
        //}

	}
#endif

    void buildTriangle(std::vector<Vertex>& IN_OUT vertices, std::vector<int>& IN_OUT indices, const TriangleCreateInfo& createInfo) {
        vec3 normal = (createInfo.v1 - createInfo.v0).crossProduct(createInfo.v2 - createInfo.v0);
        normal.normalize();
        StaticMeshVertex v;
        v.pos = createInfo.v0;
        v.normal = normal;
        v.color = createInfo.color0;
        v.texture = createInfo.tex0;
        vertices.push_back(v);
        v.pos = createInfo.v1;
        v.normal = normal;
        v.color = createInfo.color1;
        v.texture = createInfo.tex1;
        vertices.push_back(v);
        v.pos = createInfo.v2;
        v.normal = normal;
        v.color = createInfo.color2;
        v.texture = createInfo.tex2;
        vertices.push_back(v);

        indices.push_back(indices.size());
        indices.push_back(indices.size());
        indices.push_back(indices.size());
    }

    void buildTriangle(std::vector<Vertex>& IN_OUT vertices, std::vector<int>& IN_OUT indices, const Vertex& v0, const Vertex& v1, const Vertex& v2) {
        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(v2);
        indices.push_back(indices.size());
        indices.push_back(indices.size());
        indices.push_back(indices.size());
    }

    void buildParallelogramAutoNormal(std::vector<Vertex>& vertices, std::vector<int>& indices, const Vertex& v0, const Vertex& v1, const Vertex& v2) {
        vec3 normal = (v1.pos-v0.pos).crossProduct(v2.pos-v0.pos);
        normal.normalize();
        Vertex v3;
        v3.pos = v1.pos+v2.pos-v0.pos;
        v3.color = triangle_interpolate<vec4>(v3.pos, v0.pos, v1.pos, v2.pos, v0.color, v1.color, v2.color);
        v3.normal = normal;
        v3.texture = triangle_interpolate<vec2>(v3.pos, v0.pos, v1.pos, v2.pos, v0.texture, v1.texture, v2.texture);
        
        vertices.push_back(v0);
        vertices[0].normal = normal;
        vertices.push_back(v1);
        vertices[1].normal = normal;
        vertices.push_back(v2);
        vertices[2].normal = normal;
        vertices.push_back(v3);

        indices.push_back(vertices.size()-4);  //0
        indices.push_back(vertices.size() - 3);  //1
        indices.push_back(vertices.size() - 2);  //2

        indices.push_back(vertices.size() - 2); //2
        indices.push_back(vertices.size() - 3); //1
        indices.push_back(vertices.size() - 1); //3

        return;
    }

    SharedPtr<MeshComponent> loadDefaultMeshComponent() {


        return nullptr;
    }

    SharedPtr<MeshComponent> buildTriangleMeshComponent(const Vertex& v0,const Vertex& v1,const Vertex& v2) {
        SharedPtr<MeshComponent> mesh(new MeshComponent);

        std::vector<Vertex> vertices;
        std::vector<int> indices;

        buildTriangle(vertices, indices, v0, v1, v2);

        mesh->setVertices(std::move(vertices));
        mesh->setIndices(std::move(indices));

        return mesh;
    }

    SharedPtr<MeshComponent> buildParallelogramMeshComponent(const Vertex& v0, const Vertex& v1, const Vertex& v2) {
        SharedPtr<MeshComponent> mesh(new MeshComponent);

        std::vector<Vertex> vertices;
        std::vector<int> indices;

        buildParallelogramAutoNormal(vertices, indices, v0, v1, v2);

        mesh->setVertices(std::move(vertices));
        mesh->setIndices(std::move(indices));

        return mesh;
    }

    /*
    *   v4 -  v5
    *  /|    /|
      v0 - v1 v6
      |/v7  |/
      v3 - v2
    */
    SharedPtr<MeshComponent> buildCuboid(float len, float width, float height) {
        SharedPtr<MeshComponent> com = createObject<MeshComponent>();
        std::vector<Vertex> vertices;
        std::vector<int> indices;

        float hLen = len * 0.5;
        float hWidth = width * 0.5;
        float hHeight = height * 0.5;
        
        float coords[3][2] = { -hWidth,hWidth,-hHeight,hHeight,-hLen,hLen };     
        //e[0],e[1],e[2]分别是沿着x,y,z轴的离散单位矢量,任意一个点的索引矢量可以表示为
        //xe[0]+ye[1]+ze[2] x,y,z,属于 {0,1};
        ivec3 e[3];
        e[0] = { 1,0,0 };
        e[1] = { 0,1,0 };
        e[2] = { 0,0,1 };
        //遍历每个面
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 2; ++j) {
                //面矢量
                ivec3 s = e[i];
                //水平矢量
                ivec3 h = e[(i + 1) % 3];
                //垂直矢量
                ivec3 v = e[(i + 2) % 3];

                //
                //取出左下，右下，左上，右上4个顶点
                int n = vertices.size();
                for (int k = 0; k < 4; ++k) {
                    int x = k & 1;
                    int y = (k >> 1) & 1;
                    ivec3 vc = s * j + h * x + v * y;

                    Vertex vx;
                    vx.pos = { coords[0][vc.x],coords[1][vc.y],coords[2][vc.z] };
                    vx.normal = vec3(0, 0, 0);
                    vx.normal[i] = j > 0 ? (1.0) : (-1.0);
                    vx.texture = {float(x),float(y)};

                    vertices.push_back(vx);
                }
                indices.push_back(n);
                indices.push_back(n + 2-j);
                indices.push_back(n + 1+j);
                indices.push_back(n + 1+j);
                indices.push_back(n + 2-j);
                indices.push_back(n + 3);
            }
        }
        com->setVertices(std::move(vertices));
        com->setIndices(std::move(indices));

        return com;
    }

    SharedPtr<MeshComponent> buildSphereMesh() {
        static SharedPtr<MeshComponent> s_sphereMesh;
        if (s_sphereMesh == nullptr) {
            SharedPtr<MeshComponent> mesh(new MeshComponent());

            std::vector<StaticMeshVertex> vec;
            std::vector<int> indices;
            const unsigned int X_SEGMENTS = 64;
            const unsigned int Y_SEGMENTS = 64;
            //经纬球单位球生成
            for (int y = 0; y <= Y_SEGMENTS; ++y) {
                for (int x = 0; x <= X_SEGMENTS; ++x) {
                    float xSegment = (float)x / (float)X_SEGMENTS;
                    float ySegment = (float)y / (float)Y_SEGMENTS;
                    float xPos = std::cos(xSegment * 2.0f * A_PI) * std::sin(ySegment * A_PI);
                    float yPos = std::cos(ySegment * A_PI);
                    float zPos = std::sin(xSegment * 2.0f * A_PI) * std::sin(ySegment * A_PI);

                    StaticMeshVertex vt;
                    vt.pos = vec3(xPos, yPos, zPos);
                    vt.normal = vec3(xPos, yPos, zPos);
                    vt.texture = vec2(xSegment, ySegment);

                    vec.push_back(vt);
                }
            }
            bool oddRow = false;
            for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
            {
                if (!oddRow) // even rows: y == 0, y == 2; and so on
                {
                    for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                    {
                        indices.push_back(y * (X_SEGMENTS + 1) + x);
                        indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    }
                }
                else
                {
                    for (int x = X_SEGMENTS; x >= 0; --x)
                    {
                        indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                        indices.push_back(y * (X_SEGMENTS + 1) + x);
                    }
                }
                oddRow = !oddRow;
            }
            mesh->setVertices(std::move(vec));
            mesh->setIndices(std::move(indices));

            mesh->triangleType_ = EPrimitiveType::TRIANGLE_STRIP;

            s_sphereMesh = mesh;
            return mesh;
        }
        else {
            return s_sphereMesh;
        }
    }

    GameObject* createGameObject(const SharedPtr<MeshInterface>& mesh, const SharedPtr<MaterialCom>& material) {
        GameObject* act = ActorFactory<GameObject>::get()->malloc();
        act->setComponent<MeshInterface>(mesh);
        act->setComponent<MaterialCom>(material);
        return act;
    }

    GameObject* createGameObject(const GameObjectCreateInfo& createInfo) {
        GameObject* act = ActorFactory<GameObject>::get()->malloc(createInfo);
        return act;
    }

    GameObject* createGround(const std::string& path,int defaultMaterialNo) {
        const int halfLen = 20;
        GameObject* obj = ActorFactory<GameObject>::get()->malloc();
        SharedPtr<MeshComponent> mesh(new MeshComponent);
        obj->setComponent<MeshInterface>(mesh);
        SharedPtr<LightTransformCom>  trans(new LightTransformCom);
        obj->setComponent(trans);

        SharedPtr< AABBCom> bv(new AABBCom());
        bv->aabb_.mins_ = vec3(-halfLen, -0.1,-halfLen);
        bv->aabb_.maxs_ = vec3(halfLen, 0.0, halfLen);
        obj->setComponent<BoundingVolumeCom>(bv);

        Vertex v0;
        Vertex v1;
        Vertex v2;
        v2.pos = { -halfLen,0,-halfLen };
        v0.pos = { -halfLen,0,halfLen };
        v1.pos = { halfLen,0,halfLen };
        
        auto& vertices = mesh->getVertices();
        auto& indices = mesh->getIndices();
        buildParallelogramAutoNormal(vertices, indices, v0,v1,v2);
        float d = halfLen*2+1;
        vertices[0].texture = { d,0 };
        vertices[1].texture = { d,d };
        vertices[2].texture = { 0,0 };
        vertices[3].texture = { 0,d };/*{ d,d };*/
        //mesh->vertices_[4].texture = { 0,d };
        //mesh->vertices_[5].texture = { 0,0 };

        mesh->allocGpu(EDrawMode::STATIC_DRAW);

        if (defaultMaterialNo>=0) {
            SharedPtr<PbrMaterialCom> material = buildDefaultPbrMaterial(defaultMaterialNo);
            material->allocGpu();
            obj->setComponent(material);
        }
        else {
            SharedPtr<PbrMaterialCom> material(new PbrMaterialCom);
            material->maps_[ALBEDO].source_ = path;
            material->allocGpu();
            obj->setComponent<MaterialCom>(material);
        }
       

        obj->setShaderProgram(g_shaderManager->getShaderProgram(SP_NORMAL_PASS));
        obj->setName("ground");
        //act->rotate({ 0,1,0 }, A_PI / 3);
        obj->setFaceCull(false);

        return obj;
    }

    GameObject* buildDefaultGameObject(EGameObjectFlag flags) {
        GameObject* obj = ActorFactory<GameObject>::get()->malloc();
        
        unsigned int iFlags = (unsigned int)flags;
        if ((iFlags & (unsigned int)EGameObjectFlag::DEFAULT_MESH)||
            (iFlags &(unsigned int)EGameObjectFlag::ALLOC_MESH)) {
            SharedPtr<MeshComponent> mesh(new MeshComponent);
            buildDefaultCube(mesh->getVertices(), mesh->getIndices(), vec4(1.0, 0.0, 0.0, 1.0));
            obj->setComponent(mesh);
            if (iFlags & (unsigned int)EGameObjectFlag::ALLOC_MESH) {
                mesh->allocGpu(EDrawMode::STATIC_DRAW);
            }
        }
        if ((iFlags & (unsigned int)EGameObjectFlag::DEFAULT_MATERIAL)||
            (iFlags & (unsigned int)EGameObjectFlag::ALLOC_MATERIAL)) {
            SharedPtr<MaterialCom> material = buildDefaultPbrMaterial();
            obj->setComponent(material);
            if (iFlags & (unsigned int)EGameObjectFlag::ALLOC_MATERIAL) {
                material->allocGpu();
            }
        }
        if (iFlags & (unsigned int)EGameObjectFlag::DEFAULT_TRANSFORM) {
            SharedPtr<LightTransformCom> trans(new LightTransformCom);
            obj->setComponent(trans);
        }
        obj->setShaderProgram(g_shaderManager->getShaderProgram(SP_NORMAL_PASS));

        return obj;
    }

    Missile* createMissile(const MissileCreateInfo& createInfo) {
        Missile* missile = GeneralFactory::get()->malloc<Missile>(createInfo);
        return missile;
    }

    GameObject* buildPointLight(const vec3& color, const vec3& translate) {
        PointLight* act = ActorFactory<PointLight>::get()->malloc();
        SharedPtr<LightTransformCom>  trans(new LightTransformCom);
        act->setComponent(trans);

        act->setShaderProgram(g_shaderManager->getShaderProgram(SP_NORMAL_PASS));
        act->color_ = color;
        vec4 objColor = { 1.0,1.0,1.0,1.0 };

        SharedPtr<MeshComponent> mesh(new MeshComponent);
        act->setComponent(mesh);
        buildDefaultCube(mesh->getVertices(), mesh->getIndices(), vec4(0.3, 0.3, 0.3, 1.0));
        mesh->allocGpu(EDrawMode::STATIC_DRAW);

        SharedPtr<PbrMaterialCom> material = buildDefaultPbrMaterial(0);
        act->setComponent(material);
        material->allocGpu();

        act->move(translate.x, translate.y, translate.z);

        return act;
    }

    SWorld* loadDefaultWorld() {
        SWorld* world = new SWorld();
        Sun::UserSetting& setting = world->getUserSetting();
        setting.setMoveSpeed(6.f);

        world->addMainPlayer();
        Transform3D trans;

        //trans.rotation_ = Quaternion(0.483536034822464, 0.33687159419059753,
        //    -0.20870360732078552, 0.7804827094078064);
        //trans.translation_ = vec3(7.358891487121582,
        //            4.958309173583984,
        //            6.925790786743164);

        sp<Pawn> pawn = world->getMainPlayerPtr();
        trans.translation_ = vec3(0, 1.7, 2);
        pawn->setRelativeTransformation(trans);

        trans.translation_ = vec3(0, 0, 1);

        world->setCameraComRelativePosition(trans.translation_);

        SLevel* level = world->createLevel();

        return world;
    }

    void test(MissileCreateInfo createInfo) {
        createInfo.sphere.center = vec3(0, 1, 2);
    }
}