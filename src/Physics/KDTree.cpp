#include "KDTree.h"
#include "Math/Geometry.h"

namespace Sun {

	template<class _T>
	void KDTree<_T>::clear() {

	}

	template<class _T>
	void KDTree<_T>::build(const AABB& rootBox) {
		KDTreeNode node;
		node.objIndex = -1;
		AxisPlane plane = queryAxis(rootBox);
		node.axis = plane.axis;
		node.d = plane.d;
		node.box = rootBox;

		nodes_.push_back(node);
	
		build_r(0);
	}

	template<class _T>
	void KDTree<_T>::build_r(int curNode ,int depth) {
		if (depth == maxHeight_) {
			curNode->left = -1;
			curNode->right = -1;
			return;
		}

		KDTreeNode& node = nodes_[curNode];
		AABB& box = nodes_[curNode].box;
		
		AABB box1; 
		box1.mins_ = box.mins_;
		AABB box2;
		box2.maxs_ = box.maxs_;
		if (axis == X_AXIS) {
			box1.maxs_ = vec3((box.maxs_.x + box.mins_.x) * 0.5, box_maxs.y, box.maxs_.z);
			box2.mins_ = vec3((box.maxs_.x + box.mins_.x) * 0.5, box.mins_.y, box.mins_.z);
		}
		else if (axis == Y_AXIS) {
			box1.maxs_ = vec3(box.maxs_.x, (box.maxs_.y + box.mins_.y) * 0.5, box.maxs_.z);
			box2.mins_ = vec3(box.mins_.x, (box.maxs_.y + box.mins_.y) * 0.5, box.mins_.z);
		}
		else if (axis == Z_AXIS) {
			box1.maxs_ = vec3(box.maxs_.x, box.maxs_.y, (box.maxs_.z + box.mins_.z) * 0.5);
			box2.mins_ = vec3(box.mins_.x, box.mins_.y, (box.maxs_.z + box.mins_.z) * 0.5);
		}

		KDTreeNode left;
		left.box = box1;
		AxisPlane plane = queryAxis(box1);
		left.axis = plane.axis;
		left.d = plane.d;
		left.objIndex = -1;

		KDTreeNode right;
		right.box = box2;
		plane = queryAxis(box2);
		right.axis = plane.axis;
		right.d = plane.d;
		right.objIndex = -1;

		nodes_.push_back(left);
		nodes_.push_back(right);

		node.left = nodes_.size() - 2;
		node.right = nodes_.size() - 1;

		build_r(node.left, depth + 1);
		build_r(node.right, depth + 1);
	}

	template<class _T>
	int KDTree<_T>::addObj(const _T& obj,int proxyId, const AABB& box) {
		if (root_ < 0 || root_>=nodes_.size()) {
			assert(false);
			return -1;
		}

		//如果proxyId合法，则先删除对象
		if (proxyId >= 0 && proxyId < objs_.size() && !objs_[proxyId].isFreed) {
			removeObj(proxyId);
		}

		bool hint = hintAABB_AABB(box, nodes_[root_].box);
	
		if (!hint)
			return -1;

		int objIndex = -1;
		if (freeList_ != -1) {
			objIndex = freeList_;
			freeList_ = objs_[objIndex].next;

		}
		else {
			KDTreeObj<_T> tobj;
			tobj.obj = obj;
			tobj.next = -1;
			objs_.push_back(tobj);
			objIndex = objs_.size() - 1;
		}

		//找到应该将obj插入到哪个节点
		while (true) {
			KDTreeNode& node = nodes_[nodeIndex];
			if (node.isLeaf()) {
				break;
			}
			if (box.maxs_[node.axis] < node.d) {
				nodeIndex = node.left;
			}
			else if (box.mins_[node.axis] > node.d) {
				nodeIndex = node.right;
			}
			else {
				break;
			}
		}

		//把objIndex对应的对象 链接到节点node的对象链表中
		objs_[objIndex].next = node.objIndex;
		objs_[objIndex].nodeIndex = nodeIndex;
		node.objIndex = objIndex;

		return objIndex;
	}

	//输入参数proxyId是objs_数组的下标索引，这个参数一般作为成员存在T变量内部
	template<class _T>
	void KDTree<_T>::removeObj(int proxyId) {
		if (proxyId < 0 || proxyId >= objs_.size()) {
			return;
		}
		KDTreeObj<_T>& obj = objs_[proxyId];
		int nodeIndex = obj.nodeIndex;

		if (obj.nodeIndex < 0 || obj.nodeIndex >= objs_.size()) {
			assert(false);
			return;
		}

		KDTreeNode<_T>& node = nodes_[nodeIndex];
		int objIndex = node.objIndex;
		if (objIndex == proxyId) {
			//将obj从节点的链表中删除
			node.objIndex = obj.next; 
			//将obj挂载到freeList_下
			obj.next = freeList_;
			freeList_ = objIndex;
			return;
		}
		//找个循环就是在节点的链表中找到proxyId,并从链表中删除
		for( ; objIndex >= 0; objIndex = objs_[objIndex].next){
			if (objs_[objIndex].next == proxyId) {
				objs_[objIndex].next = objs_[proxyId].next;
				objs_[proxyId].next = freeList_;
				freeList_ = proxyId;
				return;
			}
		}
	}

	template<class _T>
	void KDTree<_T>::query(const AABB& box, std::function<void(const _T& crossObj)> callback) const {
		if (!hintAABB_AABB(box, nodes_[root_].box)) {
			//说明物体移动到rootAAB之外了
			assert(false);
			return false;
		}
		query_r(box, callback, root_);
	}

	template<class _T>
	void KDTree<_T>::query_r(const AABB& box, std::function<void(const _T& crossObj)> callback, int nodeIndex) const {
		KDTreeNode& node = nodes_[nodeIndex];


		int objIndex = node.objIndex;
		while (objIndex != -1) {
			callback(objs_[objIndex]);
			objIndex = objs_[objIndex].next;
		}

		if (box.maxs_[node.axis] < node.d) {
			query_r(box, callback, node.left);
		}
		if (box.mins_[node.axis] > node.d) {
			query_r(box, callback, node.right);
		}
	}

	//计算最优分离轴
	template<class _T>
	AxisPlane KDTree<_T>::queryAxis(const AABB& box) const {
		AxisPlane plane{ 0,FLT_MAX };
		for (int i = 0; i < 3; ++i) {
			float d = box.maxs_[i] - box.mins_[i];
			if (d<plane.d) {
				plane.d = d;
				plane.axis = i;
			}
		}
		plane.d = (box.maxs_[plane.axis] + box.mins_[plane.axis]) * 0.5;
		return plane;
	}

	//TODO
	template<class _T>
	std::vector<_T> KDTree<_T>::query(const AABB& box) const {

		return {};
	}
}