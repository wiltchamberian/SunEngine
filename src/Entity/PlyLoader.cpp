#include "PlyLoader.h"
#include <fstream>
#include <iostream>
#include "tinyply.h"
#include "Util/exception.h"
#include "Math/Random.h"
#include "Component/MeshComponent.h"
#include "Assets/AssetsMgr.h"
#include "Component/PbrMaterialCom.h"
#include "Component/LightTransformCom.h"
#include "Common/BasicDataType.h"
using namespace tinyply;

namespace Sun {

    GameObject* PlyLoader::load(const std::string& filepath) {
        std::unique_ptr<std::istream> file_stream;
        std::vector<uint8_t> byte_buffer;

        file_stream.reset(new std::ifstream(filepath, std::ios::binary));
        if (!file_stream || file_stream->fail()) {
            ASSERT(false);
            return nullptr;
        }

        file_stream->seekg(0, std::ios::end);
        const float size_mb = file_stream->tellg() * float(1e-6);
        file_stream->seekg(0, std::ios::beg);

        PlyFile file;
        file.parse_header(*file_stream);

        SharedPtr<PlyData> vertices, normals, colors, texcoords, faces, tripstrip;

        try { vertices = file.request_properties_from_element("vertex", { "x", "y", "z" }); }
        catch (const std::exception& e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

        try { normals = file.request_properties_from_element("vertex", { "nx", "ny", "nz" }); }
        catch (const std::exception& e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

        try { colors = file.request_properties_from_element("vertex", { "red", "green", "blue", "alpha" }); }
        catch (const std::exception& e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

        try { colors = file.request_properties_from_element("vertex", { "r", "g", "b", "a" }); }
        catch (const std::exception& e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

        try { texcoords = file.request_properties_from_element("vertex", { "u", "v" }); }
        catch (const std::exception& e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

        // Providing a list size hint (the last argument) is a 2x performance improvement. If you have 
        // arbitrary ply files, it is best to leave this 0. 
        try { faces = file.request_properties_from_element("face", { "vertex_indices" }, 3); }
        catch (const std::exception& e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

        // Tristrips must always be read with a 0 list size hint (unless you know exactly how many elements
        // are specifically in the file, which is unlikely); 
        try { tripstrip = file.request_properties_from_element("tristrips", { "vertex_indices" }, 0); }
        catch (const std::exception& e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

        file.read(*file_stream);

        GameObject* act = ActorFactory<GameObject>::get()->malloc();
        SharedPtr<MeshComponent> mesh(new MeshComponent());
        act->setComponent(mesh);
        SharedPtr<LightTransformCom> trans(new LightTransformCom);
        act->setComponent(trans);

#if 0   // a example 
        {
            const size_t numVerticesBytes = vertices->buffer.size_bytes();
            std::vector<float3> verts(vertices->count);
            std::memcpy(verts.data(), vertices->buffer.get(), numVerticesBytes);
        }
#endif
        std::vector<Vertex> meshVertices;
        std::vector<int> indices;
        randomSeed();
        {    
            SharedPtr<PbrMaterialCom> material(new PbrMaterialCom);
            act->setComponent(material);

            material->data_.albedo_ = defaultColor_;

            const size_t numVerticesBytes = vertices->buffer.size_bytes();
            unsigned int count = vertices->count;
            ASSERT(count * 12 == numVerticesBytes);

            
            uint8_t* buf = vertices->buffer.get();
            for (int i = 0; i < count; ++i) {
                meshVertices.emplace_back();
                meshVertices.back().pos = *(vec3*)(buf + i * sizeof(float) * 3);
                //设置默认颜色
                meshVertices.back().color = defaultColor_;
            
                //随机数颜色
                //float r = rand48();
                //float g = rand48();
                //float b = rand48();
                //section.staticMeshData_.datas_.back().color = { r,g,b,1.0 };
            }
        }
        {
            const size_t numFaceBytes = faces->buffer.size_bytes();
            unsigned int faceCount = faces->count;
            ASSERT(faceCount * 12 == numFaceBytes);
            uint8_t* buf = faces->buffer.get();
            indices.resize(faceCount * 3);
            memcpy(indices.data(), buf, numFaceBytes);

            //手动计算法线
            for (int i = 0; i < indices.size()/3; ++i) {
                int index0 = indices[i * 3];
                int index1 = indices[i * 3 + 1];
                int index2 = indices[i * 3 + 2];
                vec3 v1 = meshVertices[index0].pos;
                vec3 v2 = meshVertices[index1].pos;
                vec3 v3 = meshVertices[index2].pos;
                vec3 normal = (v2 - v1).crossProduct(v3 - v1);
                meshVertices[index0].normal += normal;
                meshVertices[index1].normal += normal;
                meshVertices[index2].normal += normal;
            }
        }
        mesh->setVertices(std::move(meshVertices));
        mesh->setIndices(std::move(indices));
        mesh->allocGpu(EDrawMode::STATIC_DRAW);
        return act;
    }

    void PlyLoader::setDefaultColor(const vec4& color) {
        defaultColor_ = color;
    }
}