#version 430 core
out vec4 FragColor;

in vec4 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 WorldPos; //world coordinate

//in vec3 FluidPos; //for fluid;

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

struct PointLight{
   vec3 lightPos;
   vec3 lightColor;
};
uniform PointLight ptLights[100];
uniform int numPtLight;

struct DirectionLight{
   vec3 lightPos;
   vec3 lightColor;
};
uniform DirectionLight dirLights[100];
uniform int numDirLight;

uniform sampler2D ourTexture;

const float PI = 3.14159265359;

//#include Common/Uniforms.glsl
//环境光
uniform vec3 ambientLightColor;
//单个点光源
uniform vec3 lightPos;
uniform vec3 lightColor;
//照相机视点位置
uniform vec3 viewPos;

//for fluid, 指定光照目标材质是否为流体表面
struct FluidPool{
   vec3 p0; //top_left
   vec3 p1; //bottom_left;
   vec3 p2; //bottom_right;
   float height;
   float fluidAlpha;
   float refractive;
   //水底直接用单个普通纹理吧
   sampler2D albedoMap;  
};
uniform FluidPool pool;

//fluid
vec3 refractive_point(vec3 V, vec3 N, vec3 hintPt ,float h){

   //气体折射率（默认为空气1.0)
   float n1 = 1.0;
   float n2 = 1.3; //暂时写死,Refractive;
   float sin_w1 = length(cross(N,V));
   float sin_w2 = n1*sin_w1/n2;
   float cos_w2 = sqrt(1-sin_w2*sin_w2);
   //schimit proj
   vec3 N_ =  normalize(dot(V,N)*N - V);
   vec3 V2 = N_*sin_w2+(-N)*cos_w2;

   //vUp = normalize(vUp);
   vec3 vUp = normalize(cross(pool.p2-pool.p1,pool.p0-pool.p1));
   float cos_up_V2 = dot(V2,-vUp);
   //vec3 vh = h*(-vUp);
   vec3 pt = h*(1/cos_up_V2)*V2 + hintPt;

   return pt;
}


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

//////////////////////////PRB-end//////////////////////////


void pbrLight(){
   //法线
   vec3 N = normalize(Normal);
   //视点矢量
   vec3 V = normalize(viewPos-WorldPos);
   
   //give up the alpha ,FIXME!
   vec3 albedo = material.albedo.xyz + pow(texture(material.albedoMap,TexCoord).rgb,vec3(2.2));
   float metallic = material.metallic + texture(material.metallicMap,TexCoord).r;
   float roughness = material.roughness + texture(material.roughnessMap,TexCoord).r;
   float ao = 0.0;//g_ao + texture(aoMap, TexCoord).r;

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
      
      vec3 F = fresnelSchlick(max(dot(H,V),0.0),F0);

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
   FragColor = texture(material.albedoMap,TexCoord);

   //for fluid
   vec3 FluidPos = refractive_point(V,N,WorldPos,pool.height);
   vec2 tex; 
   vec3 vx = normalize(pool.p2-pool.p1);
   vec3 vy = normalize(pool.p0-pool.p1);
   vec3 v = FluidPos-pool.p1;
   vec3 v1 = vx* dot(v,vx);
   vec3 v2 = vy * dot(v,vy);
   tex.x = length(v1)/length(pool.p2-pool.p1);
   tex.y = length(v2)/length(pool.p0-pool.p1);
   float alpha =material.albedo.w;
   //FragColor = FragColor * alpha + (1.0 - alpha)*texture(pool.albedoMap,tex);
}

void main()
{
   pbrLight();
   //FragColor = vec4(viewPos,1.0);

   //phoneLight();

};

//struct Material {
//   vec3 ambient;
//   vec3 diffuse;
//    vec3 specular;
//    float shininess;

//    vec3 albedo;
//   float metallic;
//    float roughness;
//    float ao;
//}; 




//////////////////////////bili-phone lighting///////////////////////////
//计算环境光
vec3 calAmbient(vec3 norm){
   vec3 ambient = ambientLightColor * material.ambient;
  
   return ambient;
}

//计算漫反射diffuse
vec3 calDiffuse(vec3 norm){
   // vec3(0.8,1.6,-0.2)
   vec3 lightDir = normalize( lightPos- WorldPos);
   float cosine = max(dot(norm,lightDir),0.0);
   vec3 diffuse = cosine * lightColor * material.diffuse;
   return diffuse;
}

//计算镜面反射specular
vec3 calSpecular(vec3 norm){
   float specularStrength = 0.5;
   vec3 viewDir = normalize( viewPos- WorldPos);
   vec3 lightDir = normalize( lightPos- WorldPos);
   vec3 reflectDir = reflect(-lightDir,norm);
   float cosine2 = max(dot(viewDir,reflectDir),0.0);
   //32 是高光的反光度Shininess
   float spec = pow(cosine2,material.shininess);
   vec3 specular = lightColor * (spec * material.specular); 
   return specular;
}

void phoneLight(){
   vec3 norm = normalize(Normal);

   vec3 ambient = calAmbient(norm);
   vec3 diffuse = calDiffuse(norm);
   vec3 specular = calSpecular(norm);
   
   vec3 result = ambient + diffuse + specular;

   FragColor = vec4(result, ourColor.w) + texture(ourTexture, TexCoord);
}
//////////////////////////bili-phone lighting -- end///////////////////////////