#include "PbrMaterialCom.h"
#include "Render/GraphicsLib.h"
#include "Render/GraphicsCache.h"
#include <cassert>

namespace Sun {

    PbrMaterialCom::PbrMaterialCom() {
        PbrMaterialCreateInfo info;
        init(info);
    }

    PbrMaterialCom::PbrMaterialCom(const PbrMaterialCreateInfo& createInfo) {
        init(createInfo);
    }

    void PbrMaterialCom::init(const PbrMaterialCreateInfo& createInfo) {
        pbrModel_ = createInfo.model;

        if (createInfo.model == PBRModel::METALLIC_ROUGHNESS) {
            data_.albedo_ = createInfo.albedo_;
            data_.metallic_ = createInfo.metallic_;
            data_.roughness_ = createInfo.roughness_;
            maps_[ALBEDO].source_ = createInfo.albedo_texture_path;
            maps_[METALLIC].source_ = createInfo.metallic_texture_path;
            maps_[ROUGHNESS].source_ = createInfo.roughness_texture_path;
            maps_[NORMAL].source_ = createInfo.normal_texture_path;
        }
        else if (createInfo.model == PBRModel::SPECULAR_GLOSSINESS) {
            data_.ambient_ = createInfo.ambient_;
            data_.diffuse_ = createInfo.diffuse_;
            data_.specular_ = createInfo.specular_;
            data_.shininess_ = createInfo.shininess_;
        }
    }

    void PbrMaterialCom::bind(const ShaderProgram& pro) {
        //有纹理
        if (maps_[ALBEDO].handle_ != -1 && maps_[ALBEDO].textureType_ != ETextureType::INVALID) {
            g_graphicsLib->bindTexture(pro.getId(), maps_[ALBEDO].handle_, maps_[ALBEDO].textureType_, ETextureUnit::TEXTURE0, 0, "material.albedoMap");
        }
        if (maps_[METALLIC].handle_ != -1 && maps_[METALLIC].textureType_ != ETextureType::INVALID) {
            g_graphicsLib->bindTexture(pro.getId(), maps_[METALLIC].handle_, maps_[METALLIC].textureType_, ETextureUnit::TEXTURE1, 1, "material.metallicMap");
        }
        if (maps_[ROUGHNESS].handle_ != -1 && maps_[ROUGHNESS].textureType_ != ETextureType::INVALID) {
            g_graphicsLib->bindTexture(pro.getId(), maps_[ROUGHNESS].handle_, maps_[ROUGHNESS].textureType_, ETextureUnit::TEXTURE2, 2, "material.roughnessMap");
        }
        if (maps_[NORMAL].handle_ != -1 && maps_[NORMAL].textureType_ != ETextureType::INVALID) {
            g_graphicsLib->bindTexture(pro.getId(), maps_[NORMAL].handle_, maps_[NORMAL].textureType_, ETextureUnit::TEXTURE3, 3, "material.normalMap");
        }

        //有材质
        if (pbrModel_ == PBRModel::SPECULAR_GLOSSINESS) {
            g_graphicsLib->setVec3(pro.getId(), "material.ambient", data_.ambient_);
            g_graphicsLib->setVec3(pro.getId(), "material.diffuse", data_.diffuse_);
            g_graphicsLib->setVec3(pro.getId(), "material.specular", data_.specular_);
            g_graphicsLib->setFloat(pro.getId(), "material.shininess", data_.shininess_);
        }
        else if (pbrModel_ == PBRModel::METALLIC_ROUGHNESS)
        {
            //no texture
            g_graphicsLib->setVec4(pro.getId(), "material.albedo", data_.albedo_);
            g_graphicsLib->setFloat(pro.getId(), "material.metallic", data_.metallic_);
            g_graphicsLib->setFloat(pro.getId(), "material.roughness", data_.roughness_);
            //g_graphicsLib->setFloat(pro->getId(), "material.ao", mt->data_.ao_);
        }

    }

    void PbrMaterialCom::unBind(const ShaderProgram& pro) {
        std::string vec[4] = { {"material.albedoMap" }, { "material.metallicMap" },
        { "material.roughnessMap" }, { "material.normalMap" } };
        assert(4 == PBR_MAP_NUM);

        g_graphicsLib->unBindTexture(pro.getId(), maps_[ALBEDO].handle_, maps_[ALBEDO].textureType_, ETextureUnit::TEXTURE0, 0, "material.albedoMap");
        g_graphicsLib->unBindTexture(pro.getId(), maps_[METALLIC].handle_, maps_[METALLIC].textureType_, ETextureUnit::TEXTURE1, 1, "material.metallicMap");
        g_graphicsLib->unBindTexture(pro.getId(), maps_[ROUGHNESS].handle_, maps_[ROUGHNESS].textureType_, ETextureUnit::TEXTURE2, 2, "material.roughnessMap");
        g_graphicsLib->unBindTexture(pro.getId(), maps_[NORMAL].handle_, maps_[NORMAL].textureType_, ETextureUnit::TEXTURE3, 3, "material.normalMap");
    }

    void PbrMaterialCom::allocGpu() {
        if (pbrModel_ == PBRModel::METALLIC_ROUGHNESS) {
            for (int i = 0; i < PBR_MAP_NUM; ++i) {
                 g_graphicsCache->applyForTexture(maps_[i].source_, maps_[i]);
            }
        }
        else if (pbrModel_ == PBRModel::SPECULAR_GLOSSINESS) {
        }
        else {
        }
    }

    void PbrMaterialCom::releaseGpu() {
        for (int j = 0; j < PBR_MAP_NUM; ++j) {
            if (maps_[j].handle_ >= 0) {
                g_graphicsCache->applyForReleaseTexture(maps_[j].source_);
            }
        }
    }
}