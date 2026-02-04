#include "GltfLoader2.h"

#include <fstream>
#include <set>
#include <map>
#include <deque>
#include <memory>

#include "Util/Define.h"
#include "Component/PbrMaterialCom.h"
#include "Render/GraphicsCache.h"
#include "Assets/Animation.h"
#include "Assets/Serialize.h"
#include "Component/SkeletalMeshComponent.h"
#include "Component/AnimationComponent.h"
#include "Entity/GameObject.h"
#include "Util/Factory.h"
#include "Primitive/StaticMeshPrimitive.h"
#include "Primitive/SkeletalMeshPrimitive.h"
#include "Assets/SkeletalMeshSection.h"
#include "System/AnimationSystem.h"

namespace Sun {

    std::vector<GameObject*> GltfLoader2::exportActors() {
        return actors_;
    }

    std::vector<SystemBase*> GltfLoader2::exportSystems() {
        return systems_;
    }

    void GltfLoader2::loadScene(const std::string& path) {
        path_ = path;

        size_t pos1 = path.find_last_of('\\');
        size_t pos2 = path.find_last_of('/');
        if (pos1 < path.size())
            dir_ = path.substr(0, pos1);
        else if (pos2 < path.size())
            dir_ = path.substr(0, pos2);
        else
            ASSERT(false);

        std::ifstream fstream(path);
        if (!fstream.is_open())
            return;
        fstream >> js_;

        json& scenes = js_["scenes"];
        ASSERT(scenes.is_array());
 
        preProcessNodes();
        processTextures();
        processMaterials();
        processAnimations();
        preProcessSkeletons();

        numNode_ = js_["nodes"].size();
        rotors_.resize(numNode_);
        for (auto& scene : scenes) {              
            json& nodes = scene["nodes"];
            for (auto& it : nodes) {
                Transform3D identity;
                //创建actor;
                GameObject* actor = ActorFactory<GameObject>::get()->malloc();
                actor->setComponent(SharedPtr<LightTransformCom>(new LightTransformCom()));

                actors_.emplace_back(actor);
                int nodeIndex = it.get<int>();
                //递归遍历node
                travelNode(actor,nodeIndex, identity );
            }
        }
        //for test;
        for (int i = 0; i < rotors_.size(); ++i) {
            if (rotors_[i]==nullptr) {
                ASSERT(false);
            }
        }
        //处理骨骼
        processSkeletons();
        //更新动画关联的rotor,并把动画挂载到actor
        //gltf中未标明如何关联，这里假定动画中的某个节点属于这个actor,则关联到这个actor
        sucProcessAnimations();
       
    }

    void GltfLoader2::travelNode(GameObject* rotor, int nodeIndex, Transform3D& transform) {
        rotors_[nodeIndex] = rotor;
        
        Transform3D trans = getNodeTransform(nodeIndex);
        rotor->setRelativeTransformation(trans);

        transform = transform * trans;
        bool findMesh = processElements(rotor, nodeIndex, transform);
        if (findMesh) {
            //is there any need to process subnodes?FIXME
            return;
        }
        json& node = js_["nodes"][nodeIndex];
        json& children = node["children"];
        for (auto& it : children) {
            GameObject* subRotor = RotorFactory<GameObject>::get()->malloc();
            subRotor->setComponent(SharedPtr<LightTransformCom>(new LightTransformCom()));
            subRotor->attachTo(rotor);
            travelNode(subRotor, it.get<int>(), transform);
        }
    }

    bool GltfLoader2::processElements(GameObject* gameobj, int nodeIndex, Transform3D& trans) {
        json& node = js_["nodes"][nodeIndex];
        //有网格
        bool hasMesh = node.count("mesh") > 0;
        bool hasSkin = node.count("skin") > 0;
        //骨骼蒙皮网格
        if (hasMesh && hasSkin) {
            json& mesh = js_["meshes"][node["mesh"].get<int>()];
            int skinIndex = node["skin"].get<int>();
            json& skin = js_["skins"][skinIndex];

            //SSkeletalMeshComponent2* com = new SSkeletalMeshComponent2();
            //RenderAbleCom* com = new RenderAbleCom();

            //std::shared_ptr<SkeletalMeshComponent> pMesh(new SkeletalMeshComponent);
            

            if (mesh.count("primitives")) {
                for (auto& it : mesh["primitives"]) {
                    GameObject* com = processPrimitive(it, skinIndex);
                    
                    //pMesh->addMeshSection(section);
                    gameobj->addChildObj(com);
                }
            }
            //com->setSkeleton(Singleton<AssetsMgr>::get()->getMgr<Skeleton2>().getHandle(skinIndex));
            //com->addMeshCom(pMesh);

            //这句逻辑移动到了 processPrimitive里面
            //com->setSkeleton(skeletons_[skinIndex]);
            

            // 
            //how to releated the animations? the meshComponent
            //related the mesh and animation!
            //仅仅取第一个动画
            /*THandle<Animation> anim = Singleton<AssetsMgr>::get()->animationMgr().getHandle(0);
            if (anim.isValid()) {
                com->addAnimation(anim);
            }*/

            //rotor->addComponent(com);
            return true;
        }
        //静态网格
        else if (hasMesh) {
            return true;
        }

        if (node["name"].get<std::string>() == "Camera") {
            
        }
        
        return false;
    }

    GameObject* GltfLoader2::processPrimitive(json& primitive, int skinIndex) {
        json& attributes = primitive["attributes"];
        //引用accessor
        int indice = -1;
        if (primitive.count("indices")) {
            indice = primitive["indices"];
        }
        int material = -1;
        if (primitive.count("material")) {
            material = primitive["material"];
        }
        int mode = 4;//三角形
        if (primitive.count("mode")) {
            mode = primitive["mode"];
        }
        //target关于变形动画，暂不处理,FIXME
        if (primitive.count("targets")) {
            json& targets = primitive["targets"];
        }

        int position = -1;
        if (attributes.count("POSITION")) {
            position = attributes["POSITION"].get<int>();
        }
        int color = -1;  //make sure has color!TODO
        if (attributes.count("COLOR")) {
            color = attributes["COLOR"].get<int>();
        }
        int normal = -1;
        if (attributes.count("NORMAL"))
            normal = attributes["NORMAL"].get<int>();
        int tangent = -1;
        if (attributes.count("TANGENT"))
            tangent = attributes["TANGENT"].get<int>();
        //BITANGENT?TODO

        int texcoord[3] = { -1,-1,-1 };
        if (attributes.count("TEXCOORD_0"))
            texcoord[0] = attributes["TEXCOORD_0"].get<int>();
        if (attributes.count("TEXCOORD_1"))
            texcoord[1] = attributes["TEXCOORD_1"].get<int>();
        if (attributes.count("TEXCOORD_2"))
            texcoord[2] = attributes["TEXCOORD_2"].get<int>();

        int joints[1] = { -1 };
        if (attributes.count("JOINTS_0"))
            joints[0] = attributes["JOINTS_0"].get<int>();

        int weights[1] = { -1 };
        if (attributes.count("WEIGHTS_0"))
            weights[0] = attributes["WEIGHTS_0"].get<int>();

        //解析gltf_primitive
        //SkeletalMeshSection section;
        //section.rawMesh_ = new SkeletalMeshData;
        GameObject* com = new GameObject();
        SharedPtr<SkeletalMeshComponent> section(new SkeletalMeshComponent);
        com->setComponent(section);
        section->setSkeleton(skeletons_[skinIndex]);

        //根据情况构造不同的顶点
        //VertexBlockDescription des = {};
        Data positionData;
        Data colorData;
        Data normalData;
        Data tangentData;
        Data texcoordData[3];
        Data jointsData;
        Data weightsData;
        Data indiceData;
        //Data materialData;
        if (position >= 0)
        {
            positionData = getDataOfAccessor(js_["accessors"][position]);
            DataType dtype = positionData.dataType;
            ASSERT(dtype.comType == FieldType::FLOAT);
            ASSERT(dtype.type == Type::VEC3);
        }
        if (color >= 0) {
            colorData = getDataOfAccessor(js_["accessors"][color]);
            ASSERT(colorData.count == positionData.count);
        }
        if (normal >= 0) {
            normalData = getDataOfAccessor(js_["accessors"][normal]);
            ASSERT(normalData.count == positionData.count);
        }
        if (tangent >= 0) {
            //this is vector4, last component w is 1 or -1 for handness, we assume it awalys 1 here,FIXEME
            tangentData = getDataOfAccessor(js_["accessors"][tangent]);
            ASSERT(tangentData.count == positionData.count);
        }
        int texNum = 0;
        if (texcoord[0] >= 0) {
            texNum += 1;
            texcoordData[0] = getDataOfAccessor(js_["accessors"][texcoord[0]]);
            ASSERT(texcoordData[0].count == positionData.count);
        }
        if (texcoord[1] >= 0) {
            texNum += 1;
            texcoordData[1] = getDataOfAccessor(js_["accessors"][texcoord[1]]);
            ASSERT(texcoordData[1].count == positionData.count);
        }
        if (texcoord[2] >= 0) {
            texNum += 1;
            texcoordData[2] = getDataOfAccessor(js_["accessors"][texcoord[2]]);
            ASSERT(texcoordData[2].count == positionData.count);
        }
        int numWeights = 0;
        if (joints[0] >= 0 && weights[0] >= 0) {
            jointsData = getDataOfAccessor(js_["accessors"][joints[0]]);
            weightsData = getDataOfAccessor(js_["accessors"][weights[0]]);
            ASSERT(jointsData.dataType.getComCount() == weightsData.dataType.getComCount());
            numWeights = jointsData.dataType.getComCount();
            ASSERT(jointsData.count == positionData.count);
        }
        if (indice >= 0) {
            indiceData = getDataOfAccessor(js_["accessors"][indice]);
        }
        if (material >= 0) {
            json& js_material = js_["materials"][material];
        }

        section->vertices_.resize(positionData.count);

        ASSERT(skinIndex >= 0); //skinIndex标记了骨骼索引
        //Skeleton2* skeletal = Singleton<AssetsMgr>::get()->getMgr<Skeleton2>().getData(skinIndex);
        SharedPtr<Skeleton2> skeletal = skeletons_[skinIndex];
        ASSERT(skeletal != nullptr);
        for (int i = 0; i < positionData.count; ++i/*,++iter*/) {
            if (position >= 0)
                section->vertices_[i].pos = positionData.getContent<vec3>(i);
            if (normal >= 0)
                section->vertices_[i].normal = normalData.getContent<vec3>(i);
            if (tangent >= 0)
                section->vertices_[i].tangentX = tangentData.getContent<vec3>(i, 0);
            //section.rawMesh_->datas_[i].tangentY = tangentData.getContent <vec3>(i,1);

            if (color >= 0)
                section->vertices_[i].color = colorData.getContent<vec4>(i);
            else
                section->vertices_[i].color = vec4(0.2, 0.3, 0.8, 1);

            for (int j = 0; j < std::min<int>(texNum, MAX_TEXCOORDS); ++j) {
                section->vertices_[i].textures[j] = texcoordData[j].getContent<vec2>(i);
            }

            ASSERT(numWeights <= MAX_TOTAL_INFLUENCES);
            for (int j = 0; j < std::min<int>(numWeights, MAX_TOTAL_INFLUENCES); ++j) {
                //this is the index of skinArray!
                uchar oriIndex = jointsData.getContent<uchar>(i, j);
                ASSERT(oriIndex >= 0 && oriIndex < skeletal->joints_.size());
                section->vertices_[i].jointIndex[j] = oriIndex;
            }
            float total = 0.0;
            for (int j = 0; j < std::min<int>(numWeights, MAX_TOTAL_INFLUENCES); ++j) {
                float weight = weightsData.getContent<float>(i, j);
                section->vertices_[i].jointWeight[j] = weight;
                total += weight;
            }
            if (fabs(total-1.0)>0.1) {
                ASSERT(false);
            }
        }
        if (indice >= 0) {
            section->indices_.resize(indiceData.count);
            for (int j = 0; j < indiceData.count; ++j) {
                unsigned short index = indiceData.getContent<unsigned short>(j);
                section->indices_[j] = index;
                ASSERT(index >= 0 && index < positionData.count);
            }
        }
        if (material >= 0) {
            com->setComponent(materials_[material]);
        }

        return com;
    }

    DataType GltfLoader2::getDataTypeOfAccessor(const json& accessor) {

        DataType dtype;
        int comType = accessor["componentType"].get<int>();
        std::string type = accessor["type"].get<std::string>();
        if (type == "SCALAR")
            dtype.type = Type::SCALAR;
        else if (type == "VEC2")
            dtype.type = Type::VEC2;
        else if (type == "VEC3")
            dtype.type = Type::VEC3;
        else if (type == "VEC4")
            dtype.type = Type::VEC4;
        else if (type == "MAT2")
            dtype.type = Type::MAT2;
        else if (type == "MAT3")
            dtype.type = Type::MAT3;
        else if (type == "MAT4")
            dtype.type = Type::MAT4;
        else
            ASSERT(false);

        if (comType == GLTF_UNSIGNED_SHORT)
            dtype.comType = FieldType::USHORT;
        else if (comType == GLTF_FLOAT)
            dtype.comType = FieldType::FLOAT;
        else if (comType == GLTF_UNSIGNED_INT)
            dtype.comType = FieldType::UINT;
        else if (comType == GLTF_UNSIGNED_BYTE)
            dtype.comType = FieldType::UBYTE;
        else if (comType == GLTF_BYTE)
            dtype.comType = FieldType::BYTE;
        else if (comType == GLTF_SHORT)
            dtype.comType = FieldType::SHORT;
        else
            ASSERT(false);

        return dtype;
    }

    Data GltfLoader2::getDataOfAccessor(const json& accessor) {
        Data data;
        data.dataType = getDataTypeOfAccessor(accessor);

        int bufferViewIndex = accessor["bufferView"].get<int>();
        int viewOffset = 0;
        if (accessor.count("byteOffset")) {
            viewOffset = accessor["byteOffset"].get<int>();
            ASSERT(false);
        }

        int count = accessor["count"].get<int>();
        //sparse, TODO
        //byteStride here or in View?
        if (accessor.count("byteStride") > 0) {
            data.byteStride = accessor["byteStride"];
        }
        data.bufViewIndex = bufferViewIndex;

        json& buffView = js_["bufferViews"][bufferViewIndex];
        if (buffView.count("byteStride") > 0) {
            data.byteStride = buffView["byteStride"];
        }
        int buffIndex = buffView["buffer"].get<int>();
        int byteOffset = buffView["byteOffset"].get<int>();
        int byteLen = buffView["byteLength"].get<int>();

        //(34962表示ARRAY_BUFFER)，(34963表示ELEMENT_ARRAY_BUFFER)
#define TYPE_ARRAY_BUFFER 34962
#define TYPE_ELEMENT_ARRAY_BUFFER 34963
        int target = TYPE_ELEMENT_ARRAY_BUFFER;
        if (buffView.count("target"))
            target = buffView["target"].get<int>();

        //get data
        json& buf = js_["buffers"][buffIndex];
        int bufByteLen = buf["byteLength"].get<int>();
        std::string uri = buf["uri"].get<std::string>();
        //make sure the uri is inner or ourdata,here assume it is out data

        std::string path = dir_ + "/" + uri;
        if (bufs_.size() < buffIndex + 1) {
            bufs_.resize(buffIndex + 1);
        }
        //读取数据
        if (bufs_[buffIndex].empty()) {
            std::ifstream fs(path, std::ios::binary);
            if (fs.is_open()) {
                bufs_[buffIndex].resize(bufByteLen);
                fs.read(bufs_[buffIndex].data(), bufByteLen);
            }
            else {
                ASSERT(false);
            }
            fs.close();
        }

        //提取数据
        int totalOffset = viewOffset + byteOffset;
        char* res = bufs_[buffIndex].data() + totalOffset;
        data.byteLen = byteLen;
        data.data = res;
        data.count = count;

        data.check();

        return data;
    }

    void GltfLoader2::preProcessNodes() {
        int siz = js_["nodes"].size();
        rootMap.resize(siz, -1);
        //统计每个节点的根节点
        json& scenes = js_["scenes"];
        for (auto& it : scenes) {
            json& nodes = it["nodes"];
            for (auto& node : nodes) {
                int id = node.get<int>();
                std::vector<int> vec = getAllNodes(id);
                for (int i = 0; i < vec.size(); ++i) {
                    rootMap[vec[i]] = id;
                }
            }
        }
    }

    void GltfLoader2::processTextures() {
        if (js_.count("textures")) {
            json& textures = js_["textures"];
            ASSERT(textures.is_array());
            for (auto& it : textures) {
                //Texture texture;
                //std::shared_ptr<Image> texture = acquireImage();
                Texture texture;
                textures_.push_back(texture);
                if (it.count("sampler")) {
                    int sampler = it["sampler"];
                    json& jsSampler = js_["samplers"][sampler];
                    if (jsSampler.count("magFilter"))
                        texture.magFilter_ = jsSampler["magFilter"];
                    if (jsSampler.count("minFilter"))
                        texture.minFilter_ = jsSampler["minFilter"];
                    if (jsSampler.count("wrapS"))
                        texture.wrapS_ = jsSampler["wrapS"];
                    if (jsSampler.count("wrapT"))
                        texture.wrapT_ = jsSampler["wrapT"];
                }
                if (it.count("source")) {
                    int source = it["source"];
                    json& image = js_["images"][source];
                    texture.source_ = dir_ + "/" + image["uri"].get<std::string>();
                }
            }
        }
    }
    
    void GltfLoader2::processMaterials() {
        if (js_.count("materials")) {
            json& materials = js_["materials"];
            ASSERT(materials.is_array());
            for (auto& it : materials) {
                SharedPtr<PbrMaterialCom> material(new PbrMaterialCom);
                materials_.push_back(material);
                if (it.count("pbrMetallicRoughness")) {
                    json& pbr = it["pbrMetallicRoughness"];
                    if (pbr.count("baseColorFactor")) {
                        json& ar = pbr.at("baseColorFactor");
                        material->data_.albedo_ = vec4(ar[0].get<float>(), ar[1].get<float>(), ar[2].get<float>(), ar[3].get<float>());
                    }
                    if (pbr.count("baseColorTexture")) {
                        json& texture = pbr["baseColorTexture"];
                        //material->baseColorTextureIndex_.index_ = texture["index"];
                        //material->baseColorTextureIndex_.textureCoordSn_ = texture["texCoord"];
                        int index = texture["index"].get<int>();
                        int coord = texture["texCoord"].get<int>();
                        if (index >= 0 && index < textures_.size()) {
                            material->maps_[ALBEDO] = textures_[index];
                        }
                        else {
                            ASSERT(false);
                        }
                    }
                    if (pbr.count("metallicFactor")) {
                        material->data_.metallic_ = pbr["metallicFactor"].get<float>();
                    }
                    if (pbr.count("roughnessFactor")) {
                        material->data_.roughness_ = pbr["roughnessFactor"].get<float>();
                    }
                }
                //TODO other physical model
            }

        }
    }
  
    void GltfLoader2::processAnimations() {
        if (js_.count("animations")) {
            json& animationArray = js_["animations"];
            ASSERT(animationArray.is_array());
            for (auto& it : animationArray) {
                //Animation* anim = new Animation();
                AnimationSystem* sys = new AnimationSystem();
                //THandle<Animation> handle;
                //Animation* anim = Singleton<AssetsMgr>::get()->getMgr<Animation>().acquire(handle);
                SharedPtr<Animation> anim(new Animation);
                anim->animSys = sys;
                json& samplers = it["samplers"];
                json& channels = it["channels"];
                std::map<int, NodeAnim> nodeMap;
                for (auto& channel : channels) {
                    int samplerIndex = channel["sampler"].get<int>();
                    int node = channel["target"]["node"].get<int>();
                    std::string path = channel["target"]["path"].get<std::string>();
                    NodeAnim& nodeAnim = nodeMap[node];
                    nodeAnim.oriNodeIndex_ = node;

                    json& sampler = samplers[samplerIndex];
                    int input = sampler["input"].get<int>();
                    int output = sampler["output"].get<int>();
                    std::string interpolation = sampler["interpolation"].get<std::string>();
                    if (interpolation == "LINEAR") {
                        nodeAnim.preState = AnimBehaviour_LINEAR;
                        nodeAnim.postState = AnimBehaviour_LINEAR;
                    }
                    else {
                        ASSERT(false);
                    }
                    Data inputData = getDataOfAccessor(js_["accessors"][input]);
                    //应该是时间数组
                    ASSERT(inputData.dataType.comType == FieldType::FLOAT && inputData.dataType.type == Type::SCALAR);

                    Data outputData = getDataOfAccessor(js_["accessors"][output]);
                    if (path == "weights") {
                        ASSERT(false);
                    }
                    else if (path == "rotation") {
                        ASSERT(outputData.comType() == FieldType::FLOAT && outputData.type() == Type::VEC4);
                        ASSERT(inputData.count == outputData.count);
                        for (int i = 0; i < inputData.count; ++i) {
                            QuaternionKey key;
                            key.time_ = inputData.getContent<float>(i);
                            key.value_ = outputData.getContent<Quaternion>(i);
                            nodeAnim.rotationKeys.push_back(key);
                        }
                    }
                    else if (path == "translation") {
                        ASSERT(outputData.comType() == FieldType::FLOAT && outputData.type() == Type::VEC3);
                        ASSERT(inputData.count == outputData.count);
                        for (int i = 0; i < inputData.count; ++i) {
                            VectorKey key;
                            key.time_ = inputData.getContent<float>(i);
                            key.value_ = outputData.getContent<vec3>(i);
                            nodeAnim.positionKeys.push_back(key);
                        }
                    }
                    else if (path == "scale") {
                        ASSERT(outputData.comType() == FieldType::FLOAT && outputData.type() == Type::VEC3);
                        ASSERT(inputData.count == outputData.count);
                        for (int i = 0; i < inputData.count; ++i) {
                            VectorKey key;
                            key.time_ = inputData.getContent<float>(i);
                            key.value_ = outputData.getContent<vec3>(i);
                            nodeAnim.scalingKeys.push_back(key);
                        }
                    }

                }
                for (auto it = nodeMap.begin(); it != nodeMap.end(); ++it) {
                    anim->channels.push_back(std::move(it->second));
                }
                sys->setHandle(anim);
                systems_.push_back(sys);
            }
        }
    }

    void GltfLoader2::sucProcessAnimations() {       
        //AssetsMgr::AnimationMgr& mgr = Singleton<AssetsMgr>::get()->getMgr<Animation>();
        uint32 numAnim = g_assetsMgr->animations_.size();

#if 0
        for (int i = 0; i < numAnim; ++i) {
            THandle<Animation> handle = mgr.getHandle(i);
            Animation* anim = mgr.dereference(handle);
            ASSERT(anim != nullptr);
            AnimationSystem* animSys = anim->animSys;                   
            ASSERT(animSys != nullptr);
            for (NodeAnim& nodeAnim : anim->channels) {
                nodeAnim.rotor_ = rotors_[nodeAnim.oriNodeIndex_];
            }
            if (anim->channels.empty())
                ASSERT(false);
#if 1
            if (!anim->channels.empty()) {
                NodeAnim& nA = anim->channels[0];
                SActor* actor = nA.rotor_->getActor();
                
                //这一段让actor的rootRotor持有动画，这种实现感觉不太灵活，因为动画可以作用范围更广
                //SAnimationComponent* animCom = new SAnimationComponent();
                //actor->addComponent(animCom);
                //animCom->animhandle_ = handle;

                //将动画作为一个独立存在实体（类似Unity的ECS架构的System,使得其作用于Actor，
                //不同的是ECS中system不能有状态，我们允许动画中有状态
                animSys->actor_ = actor;
            }
#endif
            //直接让动画作用于所有actor
            
        }
#endif 

        for (auto& it: g_assetsMgr->animations_) {
            SharedPtr<Animation> anim = it.second;
            ASSERT(anim != nullptr);
            AnimationSystem* animSys = anim->animSys;
            ASSERT(animSys != nullptr);
            for (NodeAnim& nodeAnim : anim->channels) {
                nodeAnim.gameobj_ = rotors_[nodeAnim.oriNodeIndex_];
            }
            if (anim->channels.empty())
                ASSERT(false);
#if 1
            if (!anim->channels.empty()) {
                NodeAnim& nA = anim->channels[0];
                GameObject* actor = nA.gameobj_;

                //这一段让actor的rootRotor持有动画，这种实现感觉不太灵活，因为动画可以作用范围更广
                //SAnimationComponent* animCom = new SAnimationComponent();
                //actor->addComponent(animCom);
                //animCom->animhandle_ = handle;

                //将动画作为一个独立存在实体（类似Unity的ECS架构的System,使得其作用于Actor，
                //不同的是ECS中system不能有状态，我们允许动画中有状态
                animSys->actor_ = actor;
            }
#endif
            //直接让动画作用于所有actor

        }
    }
    
    void GltfLoader2::preProcessSkeletons() {
        json& skins = js_["skins"];
        ASSERT(skins.is_array());
        for (auto& it : skins) {
            SharedPtr<Skeleton2> skeleton(new Skeleton2);
            skeletons_.push_back(skeleton);

            json& joints = it["joints"];
            ASSERT(joints.is_array());
            int numJoints = joints.size();

            int ks = rootMap[joints[0].get<int>()];

            skeleton->joints_.resize(numJoints);
            skeleton->invBindMatrixes_.resize(numJoints);
            //建立原始索引到joints数组索引的映射
            for (int i = 0; i < numJoints; ++i) {
                int oriIndex = joints[i].get<int>();
                skeleton->lookupMap_[oriIndex] = i;
            }
            //搜索根索引
            skeleton->oriRootIndex = getSkinSkeleton(it);
        }
    }

    void GltfLoader2::processSkeletons() {
        json& skins = js_["skins"];
        ASSERT(skins.is_array());
        //int numSkin = skins.size();


        //int numSkin = g_assetsMgr->skeletons2_.size();
        //ASSERT(numSkin == skeletons_.size());
        int numSkin = skeletons_.size();
        for (int i = 0; i < numSkin; ++i) {
            SharedPtr<Skeleton2> skt = skeletons_[i];
            ASSERT(skt != nullptr);

            for (auto& it : skt->lookupMap_) {
                int oriIndex = it.first;
                int index = it.second;
                skt->joints_[index] = rotors_[oriIndex];
                //逆绑定矩阵直接利用rotor计算;
                //skt->invBindMatrixes_[index] = rotors_[oriIndex]->worldMatrix().getInverse();
            }
            //用accessors中的逆绑定矩阵
            json& skin = skins[i];
            int accessor = skin["inverseBindMatrices"].get<int>();
            Data data = getDataOfAccessor(js_["accessors"][accessor]);
            ASSERT(data.comType() == FieldType::FLOAT && data.type() == Type::MAT4);
            std::vector<Matrix4x4> invBindMatrixes;
            for (int i = 0; i < skt->invBindMatrixes_.size(); ++i) {
                Matrix4x4 invBindMat = data.getContent<Matrix4x4>(i);
                invBindMat.transpose();
                invBindMatrixes.push_back(invBindMat);
                skt->invBindMatrixes_[i] = invBindMat;
            }
        }
        
    }

    int GltfLoader2::getSkinSkeleton(json& skin) {
        int skeletal = -1;
        json& joints = skin["joints"];
        if (skin.count("skeleton")) {
            skeletal = skin["skeleton"].get<int>();
        }
        else {
            //没有skeletal的时候，似乎没有找到理想的办法定位根,就用这些节点所在树的根节点
            int root = joints[0].get<int>();
            skeletal = rootMap[root];
            //skeletal = joints[0];
#if 0
            //this method is from gltf2ozz to find the root
            //gltf do not give a clear descriptin how to find it
            if (joints.size() == 0) {
                ASSERT(false);
                return -1;
            }
            std::map<int, int> parents;
            json& nodes = js_["nodes"];
            for (auto& joint : joints) {
                int nodeIndex = joint.get<int>();
                json& node = nodes[nodeIndex];
                if (node.count("children")) {
                    json& children = node["children"];
                    for (auto& child : children) {
                        int index = child.get<int>();
                        parents[index] = nodeIndex;
                    }
                }
            }
            int root = joints[0].get<int>();
            while (parents.find(root) != parents.end()) {
                root = parents[root];
            }
            skeletal = root;
#endif
        }
        return skeletal;
    }

    Transform3D GltfLoader2::getNodeTransform(int nodeIndex) {
        json& node = js_["nodes"][nodeIndex];
        Transform3D res;
        if (node.count("translation")) {
            json& tran = node["translation"];
            res.translation_ = vec3(tran[0].get<float>(), tran[1].get<float>(), tran[2].get<float>());
        }
        if (node.count("rotation")) {
            json& rot = node["rotation"];
            res.rotation_ = Quaternion(rot[0].get<float>(), rot[1].get<float>(), rot[2].get<float>(), rot[3].get<float>());
        }
        if (node.count("scale")) {
            json& scale = node["scale"];
            res.scale_ = vec3(scale[0].get<float>(), scale[1].get<float>(), scale[2].get<float>());
        }
        return res;
    }

    std::vector<int> GltfLoader2::getAllNodes(int nodeIndex) {
        std::vector<int> res;
        //广度优先遍历
        std::deque<int> deq;
        deq.push_back(nodeIndex);
        while (!deq.empty()) {
            int top = deq.front();
            res.push_back(top);
            deq.pop_front();
            json& node = js_["nodes"][top];
            if (node.count("children")) {
                json& arr = node["children"];
                for (auto& it : arr) {
                    deq.push_back(it.get<int>());
                }
            }
        }
        return res;
    }
}