/*****************************************************************************
* @brief :GameInclude
* just a filehead, the engine Users only need to include it, then he will have
* everything he need to create a whole game!
* 一个头文件，引擎使用者只需要包含它，就拥有创建一个完整游戏所需要的一切元素
* 高级用户自己选择其中的头文件而不用包含全部!
* @author : acedtang
* @date : 2021/4/13
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#include "Primitive/ImageCache.h"
#include "Entity/GameObject.h"
#include "Util/Util.h"
#include "Util/Factory.h"
#include "Entity/GltfLoader2.h"
#include "Entity/ObjLoader2.h"
#include "Entity/PlyLoader.h"
#include "Entity/Level.h"
#include "Entity/World.h"
#include "Entity/DirectLight.h"
#include "Component/MeshComponent.h"
#include "Component/SkeletalMeshComponent.h"
#include "Component/SphereComponent.h"
#include "Component/LightComponent.h"
#include "Entity/PointLight.h"
#include "Component/CubeComponent.h"
#include "Component/Fluid2dComponent.h"
#include "Component/SimpleTransformCom.h"
#include "Component/LightTransformCom.h"
#include "System/SystemBase.h"
#include "System/PlayerControlSystem.h"
#include "System/FieldSystem.h"

#include "Component/RayTracingComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/AABBCom.h"

#include "Render/GraphicsCache.h"
#include "Render/ShaderManager.h"

#include "Math/MathUtils.h"
#include "Math/Quaternion.h"

#include "BuildIn/buildin.h"

//this macros for user to use the engint more convenient


