#version 430 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gMRA;

struct PointLight {
    vec3 lightPos;
    vec3 lightColor;
    
    float Linear;
    float Quadratic;
    float Radius;
};
uniform int numPtLight;
uniform PointLight ptLights[100];


uniform vec3 viewPos;

const float PI = 3.14159265359;

///////////////////////////PRB//////////////////////////////
//正太分布函数
float D_GGX_TR(vec3 n, vec3 h, float roughness)
{
   float a2 = roughness*roughness;
   //add a squre for make light more smooth by epic
   a2  =a2*a2;

   float nh = dot(n,h);
   float nh2 = nh*nh;
   float denom = nh2*(a2-1)+1;
   denom = PI*denom*denom;
   return a2/denom;
}

//几何函数
float GeometrySchlickGGX(float nv, float roughness){
   float r = (roughness + 1.0);
   float k =(r*r)/8.0;

   return nv/(nv*(1-k)+k);
}
float GeometrySmith(vec3 n,vec3 v, vec3 l, float roughness){
   float nv = max(dot(n,v),0.0);
   float nl = max(dot(n,l),0.0);
   float ggx1 = GeometrySchlickGGX(nv,roughness);
   float ggx2 = GeometrySchlickGGX(nl,roughness);
   return ggx1 * ggx2;
}

//菲涅尔方程
vec3 fresnelSchlick(float cosTheta, vec3 F0){
   return F0+(1-F0)*pow(1-cosTheta,5);
}

void pbrLight(){
    // Retrieve data from gbuffer
    vec3 WorldPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    //vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    //float Specular = texture(gAlbedoSpec, TexCoords).a;

    vec3 albedo = texture(gAlbedo,TexCoords).rgb;
    float metallic = texture(gMRA,TexCoords).r;
    float roughness = texture(gMRA,TexCoords).g;
    float ao  = 0.0;


   //法线
   vec3 N = normalize(Normal);
   //视点矢量
   vec3 V = normalize(viewPos-WorldPos);

   vec3 F0 = vec3(0.04);
   F0 = mix(F0,albedo, metallic);
   vec3 Lo = vec3(0.0);
   for(int i=0;i<numPtLight;++i){
      vec3 L = normalize(ptLights[i].lightPos - WorldPos);
      vec3 H = normalize(L+V);
      //灯光到hit点的距离
      float dis = length(ptLights[i].lightPos- WorldPos);
      //距离造成的衰减因子
      float attenuation = 1.0/(dis*dis);
      vec3 radiance = ptLights[i].lightColor * attenuation;

      //计算BRDF
      
      vec3 F = fresnelSchlick(max(dot(H,V),0),F0);

      float D = D_GGX_TR(N,H,roughness);

      float G = GeometrySmith(N,V,L,roughness);

      //cook- torrance specular bdrf item
      vec3 nom = (D*F*G);
      float denom = clamp(4.0*max(dot(N,V),0.0)*max(dot(N,L),0.0),0.001,4.0);
      vec3 specular = nom/denom;

      vec3 ks = F;
      vec3 kd = vec3(1.0)-ks;
      kd *= (1.0- metallic);

      float nl = max(dot(N,L),0.0);
      Lo += (kd * albedo / PI +specular)*radiance*nl;

   }

   //add环境光
   vec3 ambient = vec3(0.03) * albedo * ao;
   vec3 color = ambient + Lo;

   //防止color>1 + gammar矫正
   color = color/(color+vec3(1.0));
   color = pow(color,vec3(1.0/2.2));

   //float coff = step(0.001, ao);
   //FragColor = vec4(color,1.0)*coff + texture(ourTexture,TexCoord);
   //FragColor = vec4(color,1.0);
   FragColor = vec4(color,1.0);//+texture(material.albedoMap,TexCoord)
}

//////////////////////////PRB-end//////////////////////////

void main()
{             
    pbrLight();
}
