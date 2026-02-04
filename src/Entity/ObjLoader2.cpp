#include "ObjLoader2.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
using namespace tinyobj;

#include "Util/StringUtil.h"
#include "Component/MeshComponent.h"
#include "Component/PbrMaterialCom.h"
#include "Entity/GameObject.h"
#include "Common/Define.h"
#include "Render/ShaderManager.h"
#include <iostream>
#include <fstream>
#include <cassert>

namespace Sun {

    bool ObjLoader2::load(const char* path, bool triangle) {

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn;
        std::string err;

        std::string filename = getFileName(path);
        std::string dir = getFileDir(path, true);
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str(),
            dir.c_str(), triangle);

        GameObject* com = new GameObject();
        SharedPtr<MeshComponent> mesh(new MeshComponent);
        com->setComponent(mesh);

        SharedPtr<LightTransformCom> trans(new LightTransformCom);
        com->setComponent(trans);

        SharedPtr<PbrMaterialCom> material(new PbrMaterialCom);
        com->setComponent(material);
        
        material->allocGpu();

        //std::unordered_map<Vertex, uint32_t> uniqueVertices = {};
        //for (const auto& shape : shapes) {
        //    for (const auto& index : shape.mesh.indices) {
        //        Vertex vertex;

        //        vertex.pos[2] = attrib.vertices[3 * index.vertex_index + 0];
        //        vertex.pos[0] = attrib.vertices[3 * index.vertex_index + 1];
        //        vertex.pos[1] = attrib.vertices[3 * index.vertex_index + 2];

        //        vertex.texture[0] = attrib.texcoords[2 * index.texcoord_index];
        //        vertex.texture[1] = 1.0 - attrib.texcoords[2 * index.texcoord_index + 1];
        //        //vertex.texture[1] = attrib.texcoords[2 * index.texcoord_index + 1];

        //        vertex.color[0] = 1.0f;
        //        vertex.color[1] = 1.0f;
        //        vertex.color[2] = 1.0f;

        //        vertex.normal[0] = attrib.normals[index.normal_index * 3 + 0];
        //        vertex.normal[1] = attrib.normals[index.normal_index * 3 + 1];
        //        vertex.normal[2] = attrib.normals[index.normal_index * 3 + 2];

        //        if (uniqueVertices.count(vertex) == 0) {
        //            uniqueVertices[vertex] = static_cast<uint32_t>(mesh->vertices_.size());
        //            mesh->vertices_.push_back(vertex);
        //        }
        //        mesh->indices_.push_back(uniqueVertices[vertex]);
        //    }
        //}

        com->allocGpu(EDrawMode::STATIC_DRAW);
        com->setShaderProgram(g_shaderManager->getShaderProgram(SP_NORMAL_PASS));

        return true;
    }


}