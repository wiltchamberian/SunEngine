/*****************************************************************************
* @brief :SeaFluid
* 海洋流体仿真
* @author : acedtang
* @date : 2021/8/17
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __SEA_FLUID_H
#define __SEA_FLUID_H

#include "Fluid.h"
#include "Math/Vector3D.h"
#include "Math/FFT.h"
#include <tuple>

namespace Sun {

	//the Phillips spectrum function: k is a vector, h is a complex , j = sqrt(-1),wd is wind_dir ,
	//A is the attitude
	/*
	h(k,t) = h0(k)exp(j sqrt(g*norm(k) ) t) + conj(h0(-k))exp(-j sqrt(g*norm(k)) t); 
	with 
	h0(k) = (1/sqrt(2))*(d_r + j*d_i)sqrt(P_h(k))
	with
	d_r,d_i independtly random number with uniform 0 ,std 1  normalization distribution
	P_h(k) = Aexp(-1/(kL)^2)/k^4 * |dot(k,wd)|^2  L=wind_speed*wind_spped/g;
	*/
	
	struct SeaFluidPram {
		float g0 = 10; //重力加速度
		vec3  wind_dir0; //must be normalized 初始风向
		float wind_speed0 = 10;  //初始风速 10m/s
		float A0 = 1; //初始震幅
		//float L0;  //海波patch长度
		//constexpr int power = 6;//傅里叶采样指数
		SeaFluidPram() {
			wind_dir0 = vec3(INV_SQRT2, 0, INV_SQRT2);
		}
	};

#define SEA_FLUID_POWER 6

	class SeaFluid :public Fluid , protected SeaFluidPram {
	public:
		struct Wind {
			vec3 dir;
			float speed;
		};

		enum FieldType {
			HEIGHT,
			XGRAD,
			ZGRAD,
		};

		//main
		void run(float t);

		//对Phillips spectrum执行IFFT ,频谱取值范围: 
		//k_x 2*pi*n/L  -N/2<=n<=N/2-1
		//k_z 2*pi*m/L  -N/2<=m<=N/2-1
		//x=uL/N   -N/2<=u<=N/2-1
		//z=vL/N    -N/2<=v<=N/2-1
		//计算之后得到高度场
		void calField(float t , std::complex<float> h[1<< SEA_FLUID_POWER][1<< SEA_FLUID_POWER], FieldType type);

		void calNormalField();

		//Phillips spectrum计算公式
		std::complex<float> phillipsSpectrum(float k_x, float k_z ,float t);


	protected:
		Sun::FFT<float, SEA_FLUID_POWER> fft_;

		//output
		//高度场
		std::complex<float> h_[1<< SEA_FLUID_POWER][1<< SEA_FLUID_POWER]; //output
		//X梯度场
		std::complex<float> grad_x[1 << SEA_FLUID_POWER][1 << SEA_FLUID_POWER]; //output
		//Z梯度场
		std::complex<float> grad_z[1 << SEA_FLUID_POWER][1 << SEA_FLUID_POWER]; //output
		//法向场
		vec3 normal_[1 << SEA_FLUID_POWER][1 << SEA_FLUID_POWER]; //output

		//计算高度场对应的初始频谱
		void calHeightFieldInitSpectrum(std::complex<float> h[1 << SEA_FLUID_POWER][1 << SEA_FLUID_POWER] ,float t);
		//计算梯度场x方向对应的初始频谱
		void calXDirGradInitSpectrum(std::complex<float> h[1 << SEA_FLUID_POWER][1 << SEA_FLUID_POWER], float t);
		//计算梯度场y方向对应的初始频谱
		void calYDirGradInitSpectrum(std::complex<float> h[1 << SEA_FLUID_POWER][1 << SEA_FLUID_POWER], float t);

		//h0(k) = (1 / sqrt(2)) * (d_r + j * d_i)sqrt(P_h(k))
		std::complex<float> phillipsSpectrumBase(float A, float k_x, float k_z, float rand_x, float rand_y,
			const vec3 wind_dir, float wind_speed, float g);

		//P_h(k) = Aexp(-1/(kL)^2)/k^4 * |dot(k,wd)|^2  L=wind_speed*wind_spped/g;
		float p_h(float A, float k_x, float k_y, const vec3& wind_dir, float wind_speed, float g);

		//生成两个(0,1)正太分布随机数
		std::tuple<float, float> generateNormalDistribution(float t);

		//生成风向和风速
		Wind generateWind(float t);
	};



}

#endif