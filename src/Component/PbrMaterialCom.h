/*****************************************************************************
* @brief : material
* 材质，决定了网格如何被渲染
* 1 纹理复合
* 2 光照计算
* @author : acedtang
* @date : 2021/3/20
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/

#ifndef __PBR_MATERIAL_COM_H
#define __PBR_MATERIAL_COM_H

#include <memory>
#include <string>
#include "Math/Vector4D.h"
#include "Primitive/Texture.h"
#include "Util/Guid.h"
#include "MaterialCom.h"

namespace Sun {
    enum class PBRModel {
        METALLIC_ROUGHNESS,
        SPECULAR_GLOSSINESS,
    };

    struct TextureIndex {
        int index_ = -1;
        int textureCoordSn_ = 0; //TEXCOORD_0:0,TEXCOORD_1:1 ...
    };

    struct PbrMaterialCreateInfo {
        PBRModel model = PBRModel::METALLIC_ROUGHNESS;

        std::string albedo_texture_path;
        std::string metallic_texture_path;
        std::string roughness_texture_path;
        std::string normal_texture_path;

        //only used for METALLIC_ROUGHNESS
        vec4 albedo_;
        float metallic_;
        float roughness_;

        //only used for SPECULAR_GLOSSINESS
        vec3 ambient_;
        vec3 diffuse_;
        vec3 specular_;
        float shininess_;

        PbrMaterialCreateInfo(){
            albedo_ = vec4(0.0, 0.0, 0.0, 1.0);
            metallic_ = 0.0;
            roughness_ = 0.0;
            shininess_ = 0.0;
        }
    };

    class PbrMaterialCom : public MaterialCom
    {
    public:
        PbrMaterialCom();
        PbrMaterialCom(const PbrMaterialCreateInfo& createInfo);

        void bind(const ShaderProgram& pro) override;
        void unBind(const ShaderProgram& pro) override;
        void allocGpu() override;
        void releaseGpu() override;

        PBRModel pbrModel_;

        //asset包中的纹理索引,下同
        //TextureIndex baseColorTextureIndex_;
        //TextureIndex metallicFactorTextureIndex_;
        //TextureIndex roughnessFactorTextureIndex_;

        //
        union Info{
            Info() {
                /*ambient_ = { 1.0,0.5,0.31 };
                diffuse_ = { 1.0,0.5,0.31 };
                specular_ = { 0.5,0.5,0.5 };
                shininess_ = 32.f;*/
            }
            ~Info() {

            }
            struct {
                vec3 ambient_;
                vec3 diffuse_;
                vec3 specular_;
                float shininess_;
            };
            struct {
                vec4 albedo_;
                float metallic_;
                float roughness_;
                //float ao_;
                
            };
        } data_;
        //can not put in union
        Texture maps_[4];
        //Texture textures_[4];

        Guid guid_;
    private:
        void init(const PbrMaterialCreateInfo& createInfo);
    };
}

#endif