#ifndef __FLUID_2D_COMPONENT_H
#define __FLUID_2D_COMPONENT_H

#include "Common/Define.h"
#include "Component/MeshComponent.h"
#include "Component/PbrMaterialCom.h"
#include "Primitive/Image.h"

namespace Sun {

	class Fluid2dComponent : public MeshComponent
	{
	public:
		Fluid2dComponent(int n, int m,float d,float t,float c,float mu ,float height,float refractive);
		
		void tick(float dtime, const SEvent& ev) override;
	
		void updateVertexData() override;
	    void bindUniformData(const ShaderProgram& pro) override;
		void unBindUniformData(const ShaderProgram& pro) override;

	protected:
		void update();
	public:
		//水平面高度场
		std::vector<float> zfieldsCur_;
		std::vector<float> zfieldsPre_;
		std::vector<float> zfieldsTmp_;

		float mu_;
		float c_;// = F/row
		float dt_; // seconds
		float d_;
		int n_;  //x direction
		int m_;  // y direction
		float height_;
		float refractive_;

		float t1_;
		float t2_;
		float t3_;

		vec3 p0_; //left-top
		vec3 p1_; //left-bottom
		vec3 p2_; //right-bottom

		SharedPtr<MaterialCom> materialCom_;
		//std::weak_ptr<Image> bottomTexture_;
		Texture hFluidTex;
	};

}

#endif 