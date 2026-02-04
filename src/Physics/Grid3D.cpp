#include "Grid3D.h"

namespace Sun {

	Index3D operator+(const Index3D& in1, const Index3D& in2) {
		Index3D ind;
		ind.x = in1.x + in2.x;
		ind.y = in1.y + in2.y;
		ind.z = in1.z + in2.z;
		return ind;
	}

	template<class _T>
	Grid3D<_T>::Grid3D<_T>()
		:numX_(1),numY_(1),numZ_(1),dx_(1),dy_(1),dz_(1)
	{
		init();
	}

	template<class _T>
	Grid3D<_T>::Grid3D(float dx, float dy, float dz, int numX, int numY, int numZ)
		:numX_(numX),numY_(numY),numZ_(numZ),dx_(dx),dy_(dy),dz_(dz)
	{
		init();
	}

	template<class _T>
	Grid3D<_T>::Grid3D(float d, int numX, int numY, int numZ)
		:numX_(numX),numY_(numY),numZ_(numZ),dx_(d),dy_(d),dz_(d)
	{
		init();
	}

	template<class _T>
	void Grid3D<_T>::init() {
		num_ = numX_ * numY_ * numZ_;
		datas_.resize(num_);
	}

	template<class _T>
	float Grid3D<_T>::getXLen() const {
		return dx_ * numX_;
	}

	template<class _T>
	float Grid3D<_T>::getYLen() const {
		return dy_ * numY_;
	}

	template<class _T>
	float Grid3D<_T>::getZLen() const {
		return dz_ * numZ_;
	}

	template<class _T>
	_T* Grid3D<_T>::getData(int x, int y, int z) {
		int k = numX_ * numY_ * z + numX_ * y + x;
		if (k < 0 || k >= num_)return nullptr;
		return datas_[numX_ * numY_ * z + numX_ * y + x];
	}

	template<class _T>
	_T* Grid3D<_T>::getData(const Index3D& index) {
		int k = numX_ * numY_ * z + numX_ * y + x;
		if (k < 0 || k >= num_)return nullptr;
		return datas_[numX_ * numY_ * index.z + numX_ * index.y + index.x];
	}

	template<class _T>
	Index3D Grid3D<_T>::getIndex(float x, float y, float z) {
		Index3D index;
		index.x = int(x / dx_);
		index.y = int(y / dy_);
		index.z = int(z / dz_);
		return index;
	}

	template<class _T>
	Index3D Grid3D<_T>::getIndex(const vec3& v) {
		return getIndex(v.x, v.y, v.z);
	}

	template<class _T>
	Index3D Grid3D<_T>::getClampIndex(float x, float y, float z) {
		Index3D index = getIndex(x, y, z);
		index.x = index.x < 0 ? 0 : (index.x >= numX_ ? (numX_ - 1) : index.x);
		index.y = index.y < 0 ? 0 : (index.y >= numY_ ? (numY_ - 1) : index.y);
		index.z = index.z < 0 ? 0 : (index.z >= numZ_ ? (numZ_ - 1) : index.z);
		return index;
	}

	template<class _T>
	Index3D Grid3D<_T>::getClampIndex(const vec3& v) {
		return getClampIndex(v.x, v.y, v.z);
	}


}