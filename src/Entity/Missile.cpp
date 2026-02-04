#include "Missile.h"
#include "BuildIn/buildin.h"
#include "Component/BVSphereCom.h"
#include "Platform/PlatformMisc.h"
#include "Render/ShaderManager.h"

namespace Sun {

	Missile::Missile() {

	}

	Missile::Missile(const MissileCreateInfo& createInfo) {
		objType_ = EGameObjectType::DYNAMIC;
		setComponent(SharedPtr<BVSphereCom>(new BVSphereCom(createInfo.sphere)));

		SharedPtr<PhysicalStateCom> stateCom(new PhysicalStateCom());
		setComponent(stateCom);
		stateCom->velocity = createInfo.state.velocity;
		stateCom->invMass = createInfo.state.invMass;
		stateCom->accel = createInfo.state.accel;

		SharedPtr<MeshComponent> meshCom = buildSphereMesh();
		if (!meshCom->isAlloced()) {
			meshCom->allocGpu(EDrawMode::STATIC_DRAW);
		}
		setComponent<MeshComponent>(meshCom);

		SharedPtr< PbrMaterialCom> material = buildPbrMaterial(PbrMaterialCreateInfo());
		material->maps_[ALBEDO].source_ = getResourceDir() + + "rustediron2_basecolor.png";
		material->maps_[METALLIC].source_ = getResourceDir() +  + "rustediron2_metallic.png";
		material->maps_[ROUGHNESS].source_ = getResourceDir() + + "rustediron2_roughness.png";
		material->maps_[NORMAL].source_ = getResourceDir() + + "rustediron2_normal.png";
		material->allocGpu();
		setComponent<MaterialCom>(material);

		auto trans = buildLightTransform();
		trans->setRelativeTranslation(createInfo.state.pos);
		trans->scaleRelativeTransform(vec3(createInfo.sphere.r));
		setComponent<TransformCom>(trans);

		this->updateWorldTransform();

		this->setShaderProgram(g_shaderManager->getShaderProgram(SP_NORMAL_PASS));
	}

	Missile::~Missile() {

	}

	void Missile::tick(float dtime, const SEvent& ev) {
		if (auto it  = getComponent<PhysicalStateCom>()) {
			rigidTransform_.translate = it->velocity * dtime;
		}
	}

	bool Missile::supportCollideQuery() const {
		return true;
	}

	bool Missile::needAddToBVH() const {
		return false;
	}
}