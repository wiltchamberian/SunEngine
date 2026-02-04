/*****************************************************************************
* @brief : BVHTree
* @author : acedtang
* 主要参考box2d的实现,部分借鉴bullet3的实现，加上一些自己的理解，同时升级为3d
* @date : 2021/6/19
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __BVH_H
#define __BVH_H

#include <vector>
#include <functional>
#include <cassert>
#include "Common/Define.h"
#include "Util/QuickVector.h"
#include "Math/Box.h"
#include "Math/Vector3D.h"
#include "Math/Geometry.h"
#include "Util/Stack.h"
#include "Collision.h"

namespace Sun {

#define BVH_NULL_NODE (-1)
#define BVH_NODE_AABB_EXTENSION  0.1f

	template<class _T>
	using UStack = QuickVector<_T>;

	template<class _T>
	class BVHNode {
	public:	
		AABB aabb;
		_T userData;

		union {
			int32 parent;
			int32 next;
		};

		int32 left;
		int32 right;

		int32 height; //以该节点为根的子树高度

		FORCE_INLINE bool isLeaf() const {
			return left == BVH_NULL_NODE;
		}
		FORCE_INLINE bool isInternal() const {
			return left != BVH_NULL_NODE;
		}
		FORCE_INLINE void clear() {
			parent = BVH_NULL_NODE;
			left = BVH_NULL_NODE;
			right = BVH_NULL_NODE;
			height = 0;
		}

		template<class _Tree>
		std::vector<BVHNode<_T>*> getChildren(_Tree* tree) {
			if (isLeaf()) return {};
			std::vector<BVHNode<_T>*> vec;
			vec.push_back(&tree->nodes_[left]);
			vec.push_back(&tree->nodes_[right]);
			return vec;
		}
	};

	enum BVHSubNodeType {
		LEFT = -1,
		RIGHT = 1,
		CUR = 0,
	};

	//默认的节点之间相似度计算 算子 policy模式
	//用户自定义相似度算子，并以模板参数传入
	class SimilarityArea {
	public:
		inline BVHSubNodeType run(const AABB& nodeBox, const AABB& left, bool leftIsLeaf, const AABB& right,bool rightIsLeaf, const AABB& leaf) {
			//from box2d reason not clear
			float cost = 2.0f * nodeBox.combine(leaf).getOneQuaterOfPerimeter();
			float area = nodeBox.getOneQuaterOfPerimeter();
			float inheritanceCost = cost - 2.0 * area;

			float cost1;
			if (leftIsLeaf) {
				AABB aabb = leaf.combine(left);
				cost1 = aabb.getOneQuaterOfPerimeter() + inheritanceCost;
			}
			else {
				AABB aabb;
				aabb = leaf.combine(left);
				float darea = aabb.getOneQuaterOfPerimeter() - left.getOneQuaterOfPerimeter();
				cost1 = darea + inheritanceCost;
			}
			float cost2;
			if (rightIsLeaf) {
				AABB aabb = leaf.combine(right);
				cost2 = aabb.getOneQuaterOfPerimeter() + inheritanceCost;
			}
			else {
				AABB aabb;
				aabb = leaf.combine(right);
				float darea = aabb.getOneQuaterOfPerimeter() - right.getOneQuaterOfPerimeter();
				cost2 = darea + inheritanceCost;
			}
			if (cost < cost1 && cost < cost2)
				return BVHSubNodeType::CUR;

			if (cost1 < cost2) {
				return BVHSubNodeType::LEFT;
			}
			return BVHSubNodeType::RIGHT;
		}
	};


	template<class _T,class _SimilarityTrait = SimilarityArea>
	class BVHTree {
	public:
		friend class BVHNode<_T>;
		BVHTree();
		~BVHTree();
		
		int32 createProxy(const AABB& aabb,const _T& userData);

		void destroyProxy(int32 proxyId);

		bool moveProxy(int32 proxyId, const AABB& aabb);

		_T& getUserData(int32 proxyId) const;

		void query(std::function<bool(_T&,_T&)>, _T& obj, const AABB& box);

		void rayCast(std::function<float(BVHTree<_T,_SimilarityTrait>*,int32,RayCastInput)>, const RayCastInput& input) const;

		int32 computeHeight() const;

		BVHNode<_T>* getRoot();
	protected:
		int32 allocateNode();
		void freeNode(int32 nodeId);
		void insertLeaf(int32 node);
		void removeLeaf(int32 node);
		void updateBoundingVolume(int32 node);
		//左右旋操作
		int32 balance(int32 node);

		
		int32 computeHeight(int32 nodeId) const;

		int32 root_ = -1;
		QuickVector<BVHNode<_T>> nodes_; //存着所有节点
		int32 count_ = 0; //真实的inUse节点个数
		int32 freeList_;
		int32 insertionCount_ = 0;//插入节点的次数

		_SimilarityTrait trait_;
	};

	//moved  to inl FIXME	
	template<class _T,class _SimilarityTrait>
	BVHTree<_T, _SimilarityTrait>::BVHTree() {
		root_ = BVH_NULL_NODE;
		nodes_.resize(16);

		int32 i = 0;
		for (; i < nodes_.size() - 1; ++i) {
			nodes_[i].next = i + 1;
			nodes_[i].height = -1;
		}
		nodes_[nodes_.size() - 1].next = BVH_NULL_NODE;
		nodes_[i].height = -1;

		freeList_ = 0;
	}

	template<class _T, class _SimilarityTrait>
	BVHTree<_T, _SimilarityTrait>::~BVHTree() {
		nodes_.clear();
	}

	template<class _T, class _SimilarityTrait>
	int32 BVHTree<_T, _SimilarityTrait>::createProxy(const AABB& aabb, const _T& userData) {
		int32 proxyId = allocateNode();
		
		nodes_[proxyId].userData = userData;
		nodes_[proxyId].height = 0;
		nodes_[proxyId].aabb = aabb;

		//flat AABB
		//vec3 r(BVH_NODE_AABB_EXTENSION, BVH_NODE_AABB_EXTENSION, BVH_NODE_AABB_EXTENSION);
		//nodes_[proxyId].aabb.mins_ -= r;
		//nodes_[proxyId].aabb.maxs_ += r;

		insertLeaf(proxyId);

		return proxyId;
	}

	template<class _T, class _SimilarityTrait>
	void BVHTree<_T, _SimilarityTrait>::destroyProxy(int32 proxyId) {
		assert(0 <= proxyId && proxyId < nodes_.size());
		assert(nodes_[proxyId].isLeaf());

		removeLeaf(proxyId);
		freeNode(proxyId);
	}

	template<class _T, class _SimilarityTrait>
	bool BVHTree<_T, _SimilarityTrait>::moveProxy(int32 proxyId, const AABB& aabb) {
		assert(0 <= proxyId && proxyId < nodes_.size());
		assert(nodes_[proxyId].isLeaf());

		if (nodes_[proxyId].aabb.contains(aabb))
		{
			return false;
		}

		removeLeaf(proxyId);

		// Extend AABB.
		//AABB b = aabb;
		//vec3 r(BVH_NODE_AABB_EXTENSION);
		//b.lowerBound = b.lowerBound - r;
		//b.upperBound = b.upperBound + r;

		nodes_[proxyId].aabb = aabb;

		insertLeaf(proxyId);

		return true;
	}

	template<class _T, class _SimilarityTrait>
	_T& BVHTree<_T, _SimilarityTrait>::getUserData(int32 proxyId) const {
		assert(0 <= proxyId && proxyId < nodes_.size());
		return nodes_[proxyId].userData;
	}

	template<class _T,class _SimilarityTrait>
	void BVHTree<_T, _SimilarityTrait>::query(std::function<bool(_T&,_T&)> callback,_T& obj, const AABB& aabb) {
		QuickVector<int32> stack;
		stack.push(root_);

		while (!stack.empty()) {
			int32 nodeId = stack.pop();
			if (nodeId == BVH_NULL_NODE) {
				continue;
			}
			BVHNode<_T>* node = nodes_.data() + nodeId;
			if (node->aabb.overLap(aabb)) {
				if (node->isLeaf()) {
					bool go_on = callback(obj,node->userData);
					if (go_on == false) {
						return;
					}
				}
				else {
					stack.push(node->right);
					stack.push(node->left);
				}
			}
		}
		return;
	}

	template<class _T,class _SimilarityTrait>
	int32 BVHTree<_T, _SimilarityTrait>::allocateNode() {
		if (freeList_ == BVH_NULL_NODE) {
			size_t oldSiz = nodes_.size();
			nodes_.resize(oldSiz * 2);
			int32 siz = nodes_.size() - 1;
			for (int32 i = oldSiz; i < siz; ++i) {
				nodes_[i].next = i + 1;
				nodes_[i].height = -1;
			}
			nodes_[siz].next = BVH_NULL_NODE;
			nodes_[siz].height = -1;

			freeList_ = oldSiz;
		}

		int32 node = freeList_;
		freeList_ = nodes_[node].next;
		nodes_[node].clear();
		count_++;

		return node;
	}

	template<class _T,class _SimilarityTrait>
	void BVHTree<_T, _SimilarityTrait>::freeNode(int32 nodeId) {
		assert(0 <= nodeId && nodeId < nodes_.size());

		nodes_[nodeId].next = freeList_;
		nodes_[nodeId].height = -1;
		freeList_ = nodeId;
		--count_;
	}

	template<class _T,class _SimilarityTrait>
	void BVHTree<_T, _SimilarityTrait>::insertLeaf(int32 leaf) {
		++insertionCount_;

		if (root_ == BVH_NULL_NODE)
		{
			root_ = leaf;
			nodes_[root_].parent = BVH_NULL_NODE;
			return;
		}

		// Find the best sibling for this node
		AABB leafAABB = nodes_[leaf].aabb;
		int32 index = root_;
		//这个循环从根节点向下遍历树，但是每次根据条件只选择一个分支,
		while (nodes_[index].isLeaf() == false)
		{
			int32 left = nodes_[index].left;
			int32 right = nodes_[index].right;

			bool isLeftLeaf = nodes_[left].isLeaf();
			bool isRightLeaf = nodes_[right].isLeaf();
			BVHSubNodeType subType  = trait_.run(nodes_[index].aabb, nodes_[left].aabb, isLeftLeaf, nodes_[right].aabb, isRightLeaf, leafAABB);
			
			if (subType == BVHSubNodeType::CUR) {
				break;
			}
			if (subType == BVHSubNodeType::LEFT) {
				index = left;
			}
			else {
				index = right;
			}
		}

		int32 sibling = index;

		// Create a new parent.
		//以下这一段创建一个新节点，该节点作为sibling的新父节点，并作为slibing的老父节点的子节点
		//ori : oldparent
		//      /       \
		//    slibing    other
		// -> 
		//new:  oldparent
		//       /
		//    newparent
		//     /     \
		//  slibing  leaf(the new node)

		int32 oldParent = nodes_[sibling].parent;
		int32 newParent = allocateNode();
		nodes_[newParent].parent = oldParent;
		nodes_[newParent].userData = nullptr;
		nodes_[newParent].aabb = leafAABB.combine(nodes_[sibling].aabb);
		nodes_[newParent].height = nodes_[sibling].height + 1;

		if (oldParent != BVH_NULL_NODE)
		{
			// The sibling was not the root.
			if (nodes_[oldParent].left == sibling)
			{
				nodes_[oldParent].left = newParent;
			}
			else
			{
				nodes_[oldParent].right = newParent;
			}

			nodes_[newParent].left = sibling;
			nodes_[newParent].right = leaf;
			nodes_[sibling].parent = newParent;
			nodes_[leaf].parent = newParent;
		}
		else
		{
			// The sibling was the root.
			nodes_[newParent].left = sibling;
			nodes_[newParent].right = leaf;
			nodes_[sibling].parent = newParent;
			nodes_[leaf].parent = newParent;
			root_ = newParent;
		}

		// Walk back up the tree fixing heights and AABBs
		index = nodes_[leaf].parent;
		updateBoundingVolume(index);

	}

	template<class _T, class _SimilarityTrait>
	void BVHTree<_T, _SimilarityTrait>::updateBoundingVolume(int32 index) {
		while (index != BVH_NULL_NODE)
		{
			index = balance(index);

			int32 child1 = nodes_[index].left;
			int32 child2 = nodes_[index].right;

			assert(child1 != BVH_NULL_NODE);
			assert(child2 != BVH_NULL_NODE);

			nodes_[index].height = 1 + std::max<float>(nodes_[child1].height, nodes_[child2].height);
			nodes_[index].aabb = nodes_[child1].aabb.combine(nodes_[child2].aabb);

			index = nodes_[index].parent;
		}
	}

	// ori :  A         (if C.height - B.height > 1 and F.height > G.height ,other case is similar)
	//       / \
	//      B   C
	//         / \
	//        F   G
	//   ->
	//       C
	//      / \
	//     A   F
	//    / \
	//   B  G
	 //基本就是上图的操作
	template<class _T, class _SimilarityTrait>
	int32 BVHTree<_T, _SimilarityTrait>::balance(int32 iA) {
		assert(iA != BVH_NULL_NODE);
		BVHNode<_T>* a = nodes_.data()+iA;
		if (a->isLeaf() || a->height < 2) {
			return iA;
		}

		int32 iB = a->left;
		int32 iC = a->right;
		assert(0 <= iB && iB < nodes_.size());
		assert(0 <= iC && iC < nodes_.size());

		BVHNode<_T>* b = nodes_.data() + iB;
		BVHNode<_T>* c = nodes_.data() + iC;

		int32 balance = c->height - b->height;
		if (balance >= -1 && balance <= 1) {
			return iA;
		}
		if (balance < -1) {
			std::swap(b, c);
			std::swap(iB, iC);
		}

		int32 iF = c->left;
		int32 iG = c->right;
		BVHNode<_T>* f = nodes_.data() + iF;
		BVHNode<_T>* g = nodes_.data() + iG;

		assert(0 <= iF && iF < nodes_.size());
		assert(0 <= iG && iG < nodes_.size());

		if (f->height < g->height) {
			std::swap(f, g);
			std::swap(iF, iG);
		}
		
		if (balance < -1) {
			c->right = iA;
		}
		else {
			c->left = iA;
		}
		c->parent = a->parent;
		a->parent = iC;

		if (c->parent != BVH_NULL_NODE) {
			if (nodes_[c->parent].left == iA) {
				nodes_[c->parent].left = iC;
			}
			else {
				nodes_[c->parent].right = iC;
			}
		}
		else {
			root_ = iC;
		}

		if (balance > 1)
			c->right = iF;
		else
			c->left = iF;
		if(balance>1)
			a->right = iG;
		else 
			a->left = iG;

		g->parent = iA;
		a->aabb = b->aabb.combine(g->aabb);
		c->aabb = a->aabb.combine(f->aabb);

		a->height = 1 + std::max<float>(b->height, g->height);
		c->height = 1 + std::max<float>(a->height, f->height);

		return iC;
	}

	//ori :      grandParent
	//       /   \
	//    parent
	//        /  \
	//   sibling  leaf
	//to
	//new:    grandParent
	 //           /
	//	     sibling

	//或者   parent
	//      /     \
	//   sibling  leaf
	//to    sibling
	//      /  \
	//    次函数的操作示意图见上图
	template<class _T, class _SimilarityTrait>
	void BVHTree<_T, _SimilarityTrait>::removeLeaf(int32 leaf) {
		if (leaf == root_)
		{
			root_ = BVH_NULL_NODE;
			return;
		}

		//确定叶子节点的父节点，祖父节点和兄弟节点
		int32 parent = nodes_[leaf].parent;
		int32 grandParent = nodes_[parent].parent;
		int32 sibling;
		if (nodes_[parent].child1 == leaf)
		{
			sibling = nodes_[parent].child2;
		}
		else
		{
			sibling = nodes_[parent].child1;
		}

		//如果祖父节点不为空
		if (grandParent != BVH_NULL_NODE)
		{
			// Destroy parent and connect sibling to grandParent.
			if (nodes_[grandParent].left == parent)
			{
				nodes_[grandParent].left = sibling;
			}
			else
			{
				nodes_[grandParent].right = sibling;
			}
			nodes_[sibling].parent = grandParent;
			freeNode(parent);

			// Adjust ancestor bounds.
			int32 index = grandParent;
			while (index != BVH_NULL_NODE)
			{
				index = balance(index);

				int32 left = nodes_[index].left;
				int32 right = nodes_[index].right;

				nodes_[index].aabb = nodes_[left].aabb.combine(nodes_[right].aabb);
				nodes_[index].height = 1 + std::max<float>(nodes_[left].height, nodes_[right].height);

				index = nodes_[index].parent;
			}
		}
		else
		{
			root_ = sibling;
			nodes_[sibling].parent = BVH_NULL_NODE;
			freeNode(parent);
		}
	}

	template<class _T, class _SimilarityTrait>
	int32 BVHTree<_T, _SimilarityTrait>::computeHeight() const{
		int32 height = computeHeight(root_);
		return height;
	}

	template<class _T, class _SimilarityTrait>
	int32 BVHTree<_T, _SimilarityTrait>::computeHeight(int32 nodeId) const {
		assert(0 <= nodeId && nodeId < nodes_.size());
		const BVHNode<_T>* node = nodes_.data() + nodeId;

		if (node->isLeaf())
		{
			return 0;
		}

		int32 height1 = computeHeight(node->left);
		int32 height2 = computeHeight(node->right);
		return 1 + std::max<int32>(height1, height2);

	}

	template<class _T, class _SimilarityTrait>
	void BVHTree<_T, _SimilarityTrait>::rayCast(std::function<float(BVHTree<_T, _SimilarityTrait>*, int32, RayCastInput)>, const RayCastInput& input) const {
		vec3 p0 = input.p0;
		vec3 p1 = input.p1;
		vec3 r = p1 - p0;
		float fraction = input.fraction;

		assert(r.getSquaredLength() > 0.0f);
		r.normalize();

		UStack<int32> stack;
		stack.reserve(256);
		stack.push(root_);

		while (!stack.empty()) {
			int32 id = stack.pop();
			const auto* node = nodes_.data() + id;
			
			bool hint = hintAABB_Segment(node->box, p0, p0 + (p1 - p0) * fraction);
			if (!hint) {
				continue;
			}

			if (node->isLeaf()) {
				RayCastInput subInput;
				subInput.p0 = input.p0;
				subInput.p1 = input.p1;
				subInput.fraction = fraction;

				float value = callback(this, id, subInput);

				if (value > 0.0f)
				{
					fraction = value;
				}
				else {
					return;
				}
			}
			else {
				stack.push(node->right);
				stack.push(node->left);
			}
		}
	}

	template<class _T, class _SimilarityTrait>
	BVHNode<_T>* BVHTree<_T, _SimilarityTrait>::getRoot() {
		if (root_ >= 0 && root_ < nodes_.size()) {
			return &nodes_[root_];
		}
		return nullptr;
	}

#include "BVH.inl"
}



#endif 