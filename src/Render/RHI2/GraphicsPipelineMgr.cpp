#include "GraphicsPipelineMgr.h"
#include "Common/Lib.h"
#include "ThirdParty/json/json.hpp"
#include "Platform/PlatformMisc.h"
#include "PCH.h"
#include <fstream>
#include <string>
#include <vector>
#include "Util/exception.h"
#include "Common/glsl.h"
#include "vulkan/vulkan.hpp"
using json = nlohmann::json;

namespace Sun {

    void GraphicsPipelineMgr::initEnvironment() {
        loadShaders();
    }

    void GraphicsPipelineMgr::loadShaders() {
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
            ShaderProgramCreateInfo info;

            info.dir_ = global + relativeDir;
            info.shaderProgramName_ = shader["name"].get<std::string>();

            json& subShaders = shader["SubShaders"];
            for (auto& subShader : subShaders) {
                ShaderStageCreateInfo subInfo;
                if (subShader["type"] == "VERTEX_SHADER") {
                    subInfo.stage = vk::ShaderStageFlagBits::eVertex;
                    subInfo.fileName = subShader["filename"].get<std::string>();
                }
                else if (subShader["type"] == "FRAGMENT_SHADER") {
                    subInfo.stage = vk::ShaderStageFlagBits::eFragment;
                    subInfo.fileName = subShader["filename"].get<std::string>();
                }
                else if (subShader["type"] == "COMPUTE_SHADER") {
                    subInfo.stage = vk::ShaderStageFlagBits::eCompute;
                    subInfo.fileName = subShader["filename"].get<std::string>();
                }
                else if (subShader["type"] == "GEOMETRY_SHADER") {
                    subInfo.stage = vk::ShaderStageFlagBits::eGeometry;
                    subInfo.fileName = subShader["filename"].get<std::string>();
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
            shaderMap_[info.shaderProgramName_] = info;
        }

    }

}