#ifndef __KD_TREE_H
#define __KD_TREE_H

#include "Math/Box.h"
#include <vector>
#include <functional>

namespace Sun {

	class KDTreeNode {
	public:
		AABB box;

		//分离平面法线所对应的轴
		char axis;
		//平面在该轴的投影的坐标
		float d;

		//指向挂载在该节点下的第一个对象的索引,-1 for free
		int objIndex;

		//左子节点
		int left;
		//右子节点
		int right;

		bool isLeaf() {
			return left == -1;
		}
	};

	//对用户数据_T的封装，非侵入式的实现
	template<class _T>
	class KDTreeObj {
	public:
		//标记该对象挂在哪个节点下
		int nodeIndex;
		//用户数据
		_T obj;
		//和该对象在同一个节点下的下一个对象的索引
		int next;
		//标记这个对象是否被释放了
		bool isFreed = false;
	};

	enum Axis {
		X_AXIS = 0,
		Y_AXIS = 1,
		Z_AXIS = 2,
	};

	struct AxisPlane {
		char axis;  //is Axis
		float d;
	};


	template<class _T>
	class KDTree {
	public:
		//调用之后等价于用默认构造函数初始化之后的状态
		void clear();
		//建树 
		void build(const AABB& rootBox);

		//obj: 对象
		//proxyId: 对象在tree中的句柄,如果对象不在tree中，must赋值为-1
		//box: obj的AABB
		//return : obj在KDTree中的新句柄，如果添加失败返回-1
		//作用:将对象添加到KDTree中
		//如果proxyId在tree中（意味着对象已经被添加到tree)，则该操作相当于remove,再addobj
		int addObj(const _T& obj ,int proxyId, const AABB& box);

		void removeObj(int proxyId);
		
		//输入一个box，对所有与该box相交的对象，依次调用callback（顺序不定)
		void query(const AABB& box, std::function<void(const _T& crossObj)> callback) const;
		//返回所有与该box相交的对象
		std::vector<_T> query(const AABB& box) const;
	protected:
		void build_r(int curNode ,int depth);
		//计算最优分离轴
		AxisPlane queryAxis(const AABB& box) const;
		void query_r(const AABB& box, std::function<void(const _T& crossObj)> callback, int nodeIndex) const;
		int root_ = 0 ;
		AABB box_;

		//树最大深度
		int maxHeight_ = 6;

		//节点的数据存在这里
		std::vector<KDTreeNode> nodes_;
		
		//数据存放在这里，为什么数据和节点要分开存储?而不是数据直接存放在节点中?
		//原因有2点:
		// 1,一个节点里可以挂多个数据，这些数据通常以链表方式连接在一起，
		//这种实现保持了内存使用连续性.
		// 2，对于KDTree,其节点初始化后不会改变，但是数据经常要添加删除.
		std::vector<KDTreeObj<_T>> objs_;

		int count_ = 0; //实际个数,<=objs_.size()
		//指向objs_
		int freeList_ = -1;
	};

}


#endif