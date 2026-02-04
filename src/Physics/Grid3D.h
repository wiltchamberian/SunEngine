#ifndef __GRID3D_H
#define __GRID3D_H

#include "Math/Vector3D.h"
#include <vector>

namespace Sun {

	struct Index3D {
		int x;
		int y;
		int z;
		friend Index3D operator + (const Index3D& in1, const Index3D& in2);
	};
	extern Index3D operator+(const Index3D& in1, const Index3D& in2);

	template<class _T>
	class Grid3D {
	public:
		Grid3D();
		Grid3D(float dx, float dy, float dz, int numX, int numY, int numZ);
		Grid3D(float d, int numX,int numY,int numZ);
		Index3D getIndex(float x, float y, float z);
		Index3D getIndex(const vec3& v);
		Index3D getClampIndex(float x, float y, float z);
		Index3D getClampIndex(const vec3& v);
		void init();

		float getXLen() const;
		float getYLen() const;
		float getZLen() const;

		_T* getData(int x, int y, int z);
		_T* getData(const Index3D& index);

		int numX_;
		int numY_;
		int numZ_;
		float dx_;
		float dy_;
		float dz_;

		std::vector<_T> datas_;

		//derived
		int num_;
	};
}


#endif