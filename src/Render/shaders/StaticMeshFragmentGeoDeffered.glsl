#version 430 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;
//layout (location = 3) out float gMetallic;
//layout (location = 4) out float gRoughness;
//layout (location = 5) out float gAo;
layout (location = 3) out vec3 gMRA;

in vec4 ourColor;
in vec2 TexCoord;
in vec3 WorldPos;
in vec3 Normal;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    //metallic_roughness
    vec4 albedo;
    float metallic;
    float roughness;
    //float ao;

    sampler2D albedoMap;  
    sampler2D metallicMap;
    sampler2D roughnessMap;
    sampler2D normalMap;
    //sampler2D aoMap;
}; 
uniform Material material;

//environment
uniform float g_ao;
uniform sampler2D aoMap;

//ori no use
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{    
    // Store the fragment position vector in the first gbuffer texture
    gPosition = WorldPos;
    // Also store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);

    // And the diffuse per-fragment color
    //gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoord).rgb;
    // Store specular intensity in gAlbedoSpec's alpha component
    //gAlbedoSpec.a = texture(texture_specular1, TexCoord).r;

    //give up the alpha ,FIXME!
    //gAlbedo.w = 1.0;
    //gAlbedo.xyz = material.albedo.xyz + pow(texture(material.albedoMap,TexCoord).rgb,vec3(2.2));
    //gMRA.r = material.metallic + texture(material.metallicMap,TexCoord).r;
    //gMRA.g = material.roughness + texture(material.roughnessMap,TexCoord).r;
    //gMRA.b = 0.0;//g_ao + texture(aoMap, TexCoord).r;


    gAlbedo.xyz = vec3(0.2,0.3,0.4);
    gMRA.r = 0.5;
    gMRA.g = 0.5;
    gMRA.b = 0.1;
}