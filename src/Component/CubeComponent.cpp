#include "CubeComponent.h"
#include "Assets/AssetsMgr.h"

namespace Sun {

    CubeComponent::CubeComponent() {

    }
    
    CubeComponent::CubeComponent(const vec3& center, const float& halfLen, const vec4& color) {
        StaticMeshVertex vertex;
        std::vector<StaticMeshVertex> vertices;
        std::vector<int> indices;
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
        vertex.pos = v3;
        vertex.normal = { 0,0,-1 };
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

        for (int i = 0; i < vertices.size(); ++i) {
            indices.push_back(i);
        }
        setVertices(std::move(vertices));

    }

    CubeComponent::~CubeComponent() {

    }


}