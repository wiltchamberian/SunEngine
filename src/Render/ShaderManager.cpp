#include "ShaderManager.h"
#include <string>
#include <memory>
#include <fstream>
#include "Util/Util.h"
#include "Util/exception.h"
#include "Math/Matrix4x4.h"
#include "Render/VertexArray.h"
#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Primitive/VertexBufferLayout.h"
#include "Render/Shader.h"
#include "Render/Program.h"
#include "Render/GLFunctions.h"
#include "Primitive/SkeletalMeshPrimitive.h"
#include "Primitive/StaticMeshPrimitive.h"
#include "Render/GraphicsLib.h"
#include "ThirdParty/json/json.hpp"
#include "PCH.h"
using json = nlohmann::json;


namespace Sun {

    ShaderManager* g_shaderManager;

    void ShaderManager::createShaders() {
        loadShaders();
        //GL实例化渲染_DEPTH_TEST
        g_graphicsLib->enable(ECapability::DEPTH_TEST);
        return;

#if 0
        std::string dir = getGlobalPath() + SHADERS_RELATIVE_PATH;
        ProgramCreateInfo info;
        info.dir_ = dir;

        info.infos_ = { {"StaticMeshVertex.glsl",EShaderType::VERTEX_SHADER},{"StaticMeshFragment.glsl",
        EShaderType::FRAGMENT_SHADER} };
        info.shaderProgramName_ = SP_STATIC_MESH_SHADER;
        info.uniformInfos_ = { {UniformType::MAT4,U_VP,Matrix4x4(),1},{UniformType::MAT4,U_MODEL,Matrix4x4(),1} };
        shaderMap_[info.shaderProgramName_] = g_graphicsLib->createShaderProgram(info);

        info.infos_ = { {"SkinVertex.glsl",EShaderType::VERTEX_SHADER},{"SkinFragment.glsl",EShaderType::FRAGMENT_SHADER} };
        info.shaderProgramName_ = SP_SKELETAL_MESH_SHADER;
        info.uniformInfos_ = { {UniformType::MAT4,U_VP,Matrix4x4(),1},{UniformType::MAT4,U_GBONES,Matrix4x4(),100} };
        shaderMap_[info.shaderProgramName_] = g_graphicsLib->createShaderProgram(info);

        info.infos_ = { {"ParticleVertex.glsl",EShaderType::VERTEX_SHADER},{"ParticleFragment.glsl",EShaderType::FRAGMENT_SHADER} };
        info.shaderProgramName_ = SP_PARTICLE_SHOW_SHADER;
        info.uniformInfos_ = { {UniformType::MAT4,U_VP,Matrix4x4(),1} };
        shaderMap_[info.shaderProgramName_] = g_graphicsLib->createShaderProgram(info);

        info.infos_ = { {"ParticleCompute.glsl",EShaderType::COMPUTE_SHADER} };
        info.shaderProgramName_ = SP_PARTICLE_SHADER;
        info.uniformInfos_ = {};
        shaderMap_[info.shaderProgramName_] = g_graphicsLib->createShaderProgram(info);

        //延时着色
        info.infos_ = { {"StaticMeshVertexGeoDeffered.glsl",EShaderType::VERTEX_SHADER},
            {"StaticMeshFragmentGeoDeffered.glsl",EShaderType::FRAGMENT_SHADER} };
        info.shaderProgramName_ = SP_STATIC_MESH_GEO_DEFFERED_SHADER;
        info.uniformInfos_ = { {UniformType::MAT4,U_VP,Matrix4x4(),1},{UniformType::MAT4,U_MODEL,Matrix4x4(),1} };
        shaderMap_[info.shaderProgramName_] = g_graphicsLib->createShaderProgram(info);

        info.infos_ = { {"StaticMeshVertexRenderDeffered.glsl",EShaderType::VERTEX_SHADER},
            {"StaticMeshFragmentRenderDeffered.glsl",EShaderType::FRAGMENT_SHADER} };
        info.shaderProgramName_ = SP_STATIC_MESH_RENDER_DEFFERED_SHADER;
        info.uniformInfos_ = { {UniformType::INT,"gPosition",(int)0,1},
            {UniformType::INT,"gNormal",(int)1,1},
            {UniformType::INT,"gAlbedo",(int)2,1},
            {UniformType::INT,"gMRA",(int)1,1} };
        shaderMap_[info.shaderProgramName_] = g_graphicsLib->createShaderProgram(info);

        //frambebuffer
        info.infos_ = { {"frameBufferVS.glsl",EShaderType::VERTEX_SHADER},
            {"frameBufferFS.glsl",EShaderType::FRAGMENT_SHADER} };
        info.shaderProgramName_ = SP_FRAME_BUFFER;
        info.uniformInfos_ = { {UniformType::SAMPLER2D,U_SCREEN_TEXTURE,1} };
        shaderMap_[SP_FRAME_BUFFER] =  g_graphicsLib->createShaderProgram(info);

        //GL实例化渲染_DEPTH_TEST
        g_graphicsLib->enable(ECapability::DEPTH_TEST);
#endif
    }

    void ShaderManager::loadShaders() {
        std::string global = getGlobalPath();
        std::string dir = global + SHADERS_RELATIVE_PATH;
        std::string path = dir + "ShaderMgr.json";

        json js;
        std::ifstream fstream(path);
        if (!fstream.is_open()) {
            assert(false);
            return;
        }
        fstream >> js;

        if (js.count("Shaders") == 0) {
            assert(false);
            return;
        }
        json& shaders = js["Shaders"];
        ASSERT(shaders.is_array());
        size_t numShader = shaders.size();

        if (js.count("RelativeDir") == 0) {
            assert(false);
            return;
        }
        std::string relativeDir = js["RelativeDir"].get<std::string>();
        for (auto& shader : shaders) {
            ProgramCreateInfo info;

            info.dir_ = global + relativeDir;
            info.shaderProgramName_ = shader["name"].get<std::string>();

            json& subShaders = shader["SubShaders"];
            for (auto& subShader : subShaders) {
                ShaderCreateInfo subInfo;
                if (subShader["type"] == "VERTEX_SHADER") {
                    subInfo.type = EShaderType::VERTEX_SHADER;
                    subInfo.name = subShader["filename"].get<std::string>();
                }
                else if (subShader["type"] == "FRAGMENT_SHADER") {
                    subInfo.type = EShaderType::FRAGMENT_SHADER;
                    subInfo.name = subShader["filename"].get<std::string>();
                }
                else if (subShader["type"] == "COMPUTE_SHADER") {
                    subInfo.type = EShaderType::COMPUTE_SHADER;
                    subInfo.name = subShader["filename"].get<std::string>();
                }
                else if (subShader["type"] == "GEOMETRY_SHADER") {
                    subInfo.type = EShaderType::GEOMETRY_SHADER;
                    subInfo.name = subShader["filename"].get<std::string>();
                }
                info.infos_.emplace_back(subInfo);
            }
            for (auto& uniformJs : shader["Uniforms"]) {
                Uniform uniform;
                if (uniformJs["type"] == "mat4") {
                    uniform.type = UniformType::MAT4;
                    if (uniformJs.count("value") == 0) {
                        uniform.var = Matrix4x4();
                    }
                    else {
                        assert(uniformJs["value"].is_array());
                        if (uniformJs["value"].is_array()) {
                            Matrix4x4 mat;
                            json& arr = uniformJs["value"];
                            size_t i = 0;
                            for (auto& it : arr) {
                                mat.entries[i] = it.get<float>();
                                ++i;
                            }
                            uniform.var = mat;
                        }
                    }
                }
                else if (uniformJs["type"] == "mat3") {
                    uniform.type = UniformType::MAT3;
                    if (uniformJs.count("value") == 0) {
                        uniform.var = Matrix3x3();
                    }
                    else {
                        assert(uniformJs["value"].is_array());
                        if (uniformJs["value"].is_array()) {
                            Matrix3x3 mat;
                            json& arr = uniformJs["value"];
                            size_t i = 0;
                            for (auto& it : arr) {
                                mat.entries[i] = it.get<float>();
                                ++i;
                            }
                            uniform.var = mat;
                        }
                    }
                }
                else if (uniformJs["type"] == "vec3") {
                    uniform.type = UniformType::VEC3;
                    if (uniformJs.count("value") == 0) {
                        uniform.var = vec3();
                    }
                    else {
                        assert(uniformJs["value"].is_array());
                        if (uniformJs["value"].is_array()) {
                            json& arr = uniformJs["value"];
                            vec3 v;
                            size_t i = 0;
                            for (auto& it : arr) {
                                v[i] = it.get<float>();
                                ++i;
                            }
                            uniform.var = v;
                        }
                    }
                }
                else if (uniformJs["type"] == "float") {
                    uniform.type = UniformType::FLOAT;
                    if (uniformJs.count("value") == 0) {
                        uniform.var = 0.0;
                    }
                    else {
                        assert(uniformJs["value"].is_number());
                        if (uniformJs["value"].is_number()) {
                            uniform.var = uniformJs["value"].get<float>();
                        }
                    }
                }
                else if (uniformJs["type"] == "int") {
                    uniform.type = UniformType::INT;
                    if (uniformJs.count("value") == 0) {
                        uniform.var = (int)(0);
                    }
                    else {
                        assert(uniformJs["value"].is_number_integer());
                        if (uniformJs["value"].is_number_integer()) {
                            uniform.var = uniformJs["value"].get<int>();
                        }
                    }
                }
                else if (uniformJs["type"] == "sampler2D") {
                    uniform.type = UniformType::SAMPLER2D;
                    if (uniformJs.count("value") == 0) {
                        uniform.var = (int)0;
                    }
                    else {
                        assert(uniformJs["value"].is_number_integer());
                        if (uniformJs["value"].is_number_integer()) {
                            uniform.var = uniformJs["value"].get<int>();
                        }
                    }
                }
                else {
                    assert(false);
                }
                uniform.name = uniformJs["name"].get<std::string>();
                if (uniformJs.count("num") == 0) {
                    uniform.num = 1;
                }
                else {
                    uniform.num = uniformJs["num"].get<int>();
                }
                info.uniformInfos_.emplace_back(uniform);
            }
            shaderMap_[info.shaderProgramName_] = g_graphicsLib->createShaderProgram(info);
        }
    }

    void ShaderManager::createRayTracingShader() {
        {
            std::string rayPath;
            //光线追踪-射线生成着色器
            rayPath = getGlobalPath() + SHADERS_RELATIVE_PATH + "RayTracingGenRay.glsl";

            Shader shader = g_graphicsLib->createShader(rayPath.c_str(), EShaderType::COMPUTE_SHADER, true);
            rayGenProgram_ = g_graphicsLib->createProgram(shader);

            g_graphicsLib->deleteShader(shader);
        }
        {
            std::string rayPath;
            //光线追踪计算着色器
            rayPath = getGlobalPath() + SHADERS_RELATIVE_PATH + "RayTracingCompute.glsl";
            Shader shader2 = g_graphicsLib->createShader(rayPath.c_str(), EShaderType::COMPUTE_SHADER, true);
            rayTracingProgram_ = g_graphicsLib->createProgram(shader2);
            g_graphicsLib->deleteShader(shader2);
        }
        {
            //顶点着色器和片段着色器程序
            std::string showPath = getGlobalPath() + SHADERS_RELATIVE_PATH + "RayTracingVertex.glsl";
            Shader vertex = g_graphicsLib->createShader(showPath.c_str(), EShaderType::VERTEX_SHADER, true);
            showPath = getGlobalPath() + SHADERS_RELATIVE_PATH + "RayTracingFragment.glsl";
            Shader fragment = g_graphicsLib->createShader(showPath.c_str(), EShaderType::FRAGMENT_SHADER, true);
            rayTracingShowProgram_ = g_graphicsLib->createProgram({ vertex,fragment });

            g_graphicsLib->deleteShader(vertex);
            g_graphicsLib->deleteShader(fragment);
        }

        shaderMap_[TO_STR(RAY_GEN_SHADER)] = rayGenProgram_;
        shaderMap_[TO_STR(RAY_TRACING_SHADER)] = rayTracingProgram_;
        shaderMap_[TO_STR(RAY_TRACING_SHOW_SHADER)] = rayTracingShowProgram_;
    }

    void ShaderManager::createRayTracingShader2() {
 
        std::string rayPath;
        //光线追踪计算着色器
        rayPath = getGlobalPath() + SHADERS_RELATIVE_PATH + "RayTracingCompute.glsl";
        Shader shader_compute = g_graphicsLib->createShader(rayPath.c_str(), EShaderType::COMPUTE_SHADER, true);
        //顶点着色器
        std::string showPath = getGlobalPath() + SHADERS_RELATIVE_PATH + "RayTracingVertex.glsl";
        Shader vertex = g_graphicsLib->createShader(showPath.c_str(), EShaderType::VERTEX_SHADER, true);
        //片段着色器
        showPath = getGlobalPath() + SHADERS_RELATIVE_PATH + "RayTracingFragment.glsl";
        Shader fragment = g_graphicsLib->createShader(showPath.c_str(), EShaderType::FRAGMENT_SHADER, true);

        rayTracingProgram_ = g_graphicsLib->createProgram({ shader_compute,vertex,fragment });  

        g_graphicsLib->deleteShader(shader_compute);
        g_graphicsLib->deleteShader(vertex);
        g_graphicsLib->deleteShader(fragment);
    }

    ShaderProgram ShaderManager::getShaderProgram(const std::string& name) const {
        auto it = shaderMap_.find(name);
        if (it == shaderMap_.end()) {
            return ShaderProgram();
        }
        return it->second;
    }

}