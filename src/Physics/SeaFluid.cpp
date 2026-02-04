#include "SeaFluid.h"
#include "Math/MathConstants.h"
#include <cmath>
#include <random>

namespace Sun {

	void SeaFluid::run(float t) {

		calField(t, h_ ,FieldType::HEIGHT);
		calField(t, grad_x, FieldType::XGRAD);
		calField(t, grad_z, FieldType::ZGRAD);
		calNormalField();
	}

	//对Phillips spectrum执行IFFT ,频谱取值范围: 
	//k_x 2*pi*n/L  -N/2<=n<=N/2-1
	//k_z 2*pi*m/L  -N/2<=m<=N/2-1
	//x=uL/N   -N/2<=u<=N/2-1
	//z=vL/N    -N/2<=v<=N/2-1 ，令L=N,简化为:
	//k_x 2*pi*n/N  k_z 2*pi*m/N  x=u z= v
	void SeaFluid::calField(float t, std::complex<float> h[1<< SEA_FLUID_POWER][1<< SEA_FLUID_POWER],FieldType type) {
		
		//计算频谱
		constexpr int N = (1 << SEA_FLUID_POWER);
		int L = N;

		float k_x = 0;
		float k_z = 0;

		std::complex<float> B1[N][N];
		
		if (type == FieldType::HEIGHT) {
			calHeightFieldInitSpectrum(h, t);
		}
		else if (type == FieldType::XGRAD) {
			calXDirGradInitSpectrum(h, t);
		}
		else if (type == FieldType::ZGRAD) {
			calYDirGradInitSpectrum(h, t);
		}
		else {
			assert(false);
			return;
		}
		

		for (int m = 0; m < N; ++m) {
			fft_.run_ifft(&h[m][0], &h[m][N], B1[m]);
		}
		for (int m = 0; m < N; ++m) {
			for (int u = 0; u < N; ++u) {
				B1[m][u] = ((m + u - (N >> 1)) & 1) == 0 ? B1[m][u] : (-B1[m][u]);
			}
		}
		//为了方便fft,转置B1
		Math::transpose(B1, N);

		for (int u = 0; u < N; ++u) {
			fft_.run_ifft(&B1[u][0], &B1[u][N], h[u]);
		}

		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				h[i][j] = ((j - (N >> 1)) & 1) == 0 ? h[i][j] : (-h[i][j]);
			}
		}

	}

	void SeaFluid::calNormalField() {
		const int N = 1 << SEA_FLUID_POWER;
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				normal_[i][j] = vec3(-grad_x[i][j].real(), 1, -grad_z[i][j].real());
			}
		}
		
		
	}

	void SeaFluid::calHeightFieldInitSpectrum(std::complex<float> h[1 << SEA_FLUID_POWER][1 << SEA_FLUID_POWER],float t) {
		const int N = 1 << SEA_FLUID_POWER;
		const int L = N;
		std::complex<float> unit(0, -1);
		float k_x = -A_PI;
		float k_y = -A_PI;
		float d = 2 * A_PI / L;
		float k_dn = 0;// 2 * A_PI / L;
		float k_dm = 0;
		for (int m = 0; m < N; ++m) {
			for (int n = 0; n < N; ++n) {
				//第一输入参数为: 2 * A_PI * (n - N / 2) / L = d*n-A_PI
				//第二输入参数为: 2 * A_PI * (m - N / 2) / L  = d*m - A_PI
				h[m][n] = phillipsSpectrum(k_x + k_dn, k_y + k_dm, t);
				//calculate D1
				h[m][n] = (n & 1) == 0 ? h[m][n] : (-h[m][n]);
				k_dn += d;
			}
			k_dn = 0;
			k_dm += d;
		}
	}

	void SeaFluid::calXDirGradInitSpectrum(std::complex<float> h[1 << SEA_FLUID_POWER][1 << SEA_FLUID_POWER], float t) {
		const int N = 1 << SEA_FLUID_POWER;
		const int L = N;
		std::complex<float> unit(0, -1);
		float k_x = -A_PI;
		float k_y = -A_PI;
		float d = 2 * A_PI / L;
		float k_dn = 0;// 2 * A_PI / L;
		float k_dm = 0;
		for (int m = 0; m < N; ++m) {
			for (int n = 0; n < N; ++n) {
				//第一输入参数为: 2 * A_PI * (n - N / 2) / L = d*n-A_PI
				//第二输入参数为: 2 * A_PI * (m - N / 2) / L  = d*m - A_PI
				h[m][n] = phillipsSpectrum(k_x + k_dn, k_y + k_dm, t) * unit * (k_x + k_dn);
				//calculate D1
				h[m][n] = (n & 1) == 0 ? h[m][n] : (-h[m][n]);
				k_dn += d;
			}
			k_dn = 0;
			k_dm += d;
		}
	}

	void SeaFluid::calYDirGradInitSpectrum(std::complex<float> h[1 << SEA_FLUID_POWER][1 << SEA_FLUID_POWER], float t) {
		const int N = 1 << SEA_FLUID_POWER;
		const int L = N;
		std::complex<float> unit(0, -1);
		float k_x = -A_PI;
		float k_y = -A_PI;
		float d = 2 * A_PI / L;
		float k_dn = 0;// 2 * A_PI / L;
		float k_dm = 0;
		for (int m = 0; m < N; ++m) {
			for (int n = 0; n < N; ++n) {
				//第一输入参数为: 2 * A_PI * (n - N / 2) / L = d*n-A_PI
				//第二输入参数为: 2 * A_PI * (m - N / 2) / L  = d*m - A_PI
				h[m][n] = phillipsSpectrum(k_x + k_dn, k_y+k_dm, t)* unit* (k_y+k_dm);
				//calculate D1
				h[m][n] = (n & 1) == 0 ? h[m][n] : (-h[m][n]);
				k_dn += d;
			}
			k_dn = 0;
			k_dm += d;
		}
	}

	std::complex<float> SeaFluid::phillipsSpectrum(float k_x, float k_z,float t) {
		float A = A0; //可修改为时间t的函数
		//生成随机数
		
		float rand_x, rand_z;
		auto pr = generateNormalDistribution(t);
		rand_x = std::get<0>(pr);
		rand_z = std::get<1>(pr);

		SeaFluid::Wind wind = generateWind(t);

		std::complex<float> h0 = phillipsSpectrumBase(A, k_x, k_z, rand_x, rand_z, wind.dir, wind.speed, g0);
		std::complex<float> h0_conj = std::conj(h0);

		float k = sqrt(k_x * k_x + k_z * k_z);
		float wt = sqrt(g0 * k)*t;
		std::complex<float> ext(cos(wt), sin(wt));

		return h0 * ext + h0_conj * std::conj(ext);

	}

	std::complex<float> SeaFluid::phillipsSpectrumBase(float A, float k_x, float k_z, float rand_x, float rand_y,
		const vec3 wind_dir, float wind_speed ,float g) {
		std::complex<float> res;
		float ph = p_h(A, k_x, k_z, wind_dir, wind_speed, g);
		res = std::complex<float>(rand_x, rand_y) * ((float)(INV_SQRT2)*sqrt(ph));
		return res;
	}

	// P_h(k)= A * exp(-1/(|k|*L)^2) / k^4 * dot(k,w)^2
	float SeaFluid::p_h(float A, float k_x, float k_z, const vec3& wind_dir, float wind_speed, float g) {
		float k2 = k_x * k_x + k_z * k_z;
		float Dot = k_x * wind_dir.x + k_z * wind_dir.z;
		float invk4 = 1.0 / (k2 * k2);
		
		float v4 = wind_speed * wind_speed;
		v4 = v4 * v4;
		float et = exp(-g * g / (k2 * v4));

		float ph = A * invk4 * et * Dot * Dot;
		return ph;
	}

	std::tuple<float, float> SeaFluid::generateNormalDistribution(float t) {
		static std::tuple<float, float> res;

		static std::default_random_engine generator;
		static std::normal_distribution<double> distribution(0.0, 1.0);

		std::get<0>(res) = distribution(generator);
		std::get<1>(res)= distribution(generator);
		return res;
	}

	SeaFluid::Wind SeaFluid::generateWind(float t) {
		SeaFluid::Wind wind;
		wind.dir = wind_dir0;
		wind.speed = wind_speed0;

		//do something change with time

		return wind;
	}
}