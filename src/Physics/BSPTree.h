#ifndef __BSP_TREE_H
#define __BSP_TREE_H

#include "Math/Plane.h"
#include "boundingVolume.h"
#include <unordered_set>
#include <stack>
#include "Physics/polyhedron.h"
#include "Util/MemoryPool.h"
#include "Physics.h"

namespace Sun
{

    struct CollideInfo
    {
        int side = 0; //from Plane
        int splitSize = 0;
    };

    template<class _Object>
    class BSPTreeNode
    {
    public:
        Plane _plane;
        BSPTreeNode* _frontNode = nullptr;
        BSPTreeNode* _backNode = nullptr;
        std::vector<_Object*> _objs;
        void build_r(std::vector<_Object*>& objs);
        int classify(const Polyhedron& poly, const Plane& plane);
        void testCollideInfo(const Polyhedron& poly, const Plane& plane, CollideInfo& outInfo);
        bool bestSplitPlane(const std::vector<_Object*>& objs, Plane& plane);
        bool isLeaf() {
            return _frontNode == nullptr && _backNode == nullptr;
        }
        std::unordered_set<std::pair<_Object*, _Object*>> outputCollidingPairs();
        //估值函数
        inline int valuation(int frontSize, int backSize, int splitSize) {
            return -5 * splitSize - abs(frontSize - backSize);
        }
        void collidingObjects(const BoundingBox& box, std::vector<_Object*>& _ouptut);
    };

    template<class _Object>
    bool BSPTreeNode<_Object>::bestSplitPlane(const std::vector<_Object*>& objs, Plane& bestPlane)
    {
        //遍历所有objs的所有平面，选出最优分割平面
        int bestValue = INT_MIN;
        bool found = false;
        int bestSplitSize;
        for (int i = 0; i < objs.size(); ++i) {
            Polyhedron& poly = objs[i]->getPolyhedronRef();
            for (int j = 0; j < poly.sides.size(); ++i) {
                Side& s = poly.sides[i];
                int frontSize = 0;
                int backSize = 0;
                int splitSize = 0;
                int value = 0;
                //对该平面，遍历所有obj
                for (int k = 0; k < objs.size(); ++k) {
                    //计算该平面和obj的碰撞信息;
                    CollideInfo info;
                    Polyhedron& po = objs[k]->getPolyhedronRef();
                    testCollideInfo(po, s.plane, info);
                    splitSize += info.splitSize;
                    if (info.side & FRONT)
                        frontSize += 1;
                    if (info.side & BACK)
                        backSize += 1;
                }
                //这种划分不可取（说明全部分到一侧，或者是所有obj都被穿越
                if (frontSize == objs.size() || backSize == objs.size()) {
                    continue;
                }
                //估值函数，可调整
                value = valuation(frontSize, backSize, splitSize);
                if (bestValue < value) {
                    bestValue = value;
                    bestPlane = s.plane;
                    found = true;
                }
            }
        }
        return found;
    }

    template<class _Object>
    void BSPTreeNode<_Object>::testCollideInfo(const Polyhedron& poly, const Plane& plane, CollideInfo& outInfo)
    {
        int side = classify(poly, plane);
        outInfo.side = side;
        //not clide;
        if ((side & BOTH) != BOTH) {
            outInfo.side = side;
            return;
        }
        for (int i = 0; i < poly.sides.size(); ++i) {
            int s = poly.sideCross(i, plane);
            if (isSideCross(s)) {
                outInfo.splitSize++;
            }
        }

        return;
    }

    template<class _Object>
    int BSPTreeNode<_Object>::classify(const Polyhedron& poly, const Plane& plane)
    {
        int side = 0;
        //遍历所有obj所有顶点
        for (int i = 0; i < poly.pts.size(); ++i) {
            side |= plane.pointSide(poly.pts[i]);
            if ((side & BOTH) == BOTH) {
                break;
            }
        }
        return side;
    }

    template<class _Object>
    void BSPTreeNode<_Object>::build_r(std::vector<_Object*>& objs)
    {
        Plane plane;
        bool found = bestSplitPlane(objs, plane);
        //没有合适的分割平面，终止
        if (!found) {
            return;
        }
        this->_plane = plane;
        //找到最优分割超平面，将objs分化到平面对应侧的节点下
        _frontNode = new BSPTreeNode<_Object>();// MemoryPool::malloc<BSPTreeNode<_Object>>();
        _backNode = new BSPTreeNode<_Object>();// MemoryPool::malloc<BSPTreeNode<_Object>>();
        std::vector<_Object*> frontVec;
        std::vector<_Object*> backVec;
        //重新计算一遍位于哪一侧
        //对该平面，遍历所有obj
        for (int k = 0; k < objs.size(); ++k) {
            //计算该平面和obj的碰撞信息;
            CollideInfo info;
            Polyhedron& po = objs[k]->getPolyhedronRef();
            testCollideInfo(po, plane, info);
            if (info.side == BOTH) {
                //分配到两个节点的方案
                frontVec.push_back(objs[k]);
                backVec.push_back(objs[k]);
            }
            else if (info.side == FRONT) {
                frontVec.push_back(objs[k]);
            }
            else if (info.side == BACK) {
                backVec.push_back(objs[k]);
            }
        }
        _frontNode->build_r(frontVec);
        _backNode->build_r(backVec);
    }

    template<class _Object>
    void BSPTreeNode<_Object>::collidingObjects(const BoundingBox& box, std::vector<_Object*>& output)
    {
        if (isLeaf()) {
            output.insert(output.end(), _objs);
            return;
        }
        bool bfront = isAllInFront(box, _plane);
        if (bfront && _frontNode) {
            _frontNode->collidingObjects(box, output);
            return;
        }
        bool bback = isAllInBack(box, _plane);
        if (bback && _backNode) {
            _backNode->collidingObjects(box, output);
            return;
        }
        if (_frontNode) {
            _frontNode->collidingObjects(box, output);
        }
        if (_backNode) {
            _backNode->collidingObjects(box, output);
        }
    }

    template<class _Object>
    std::unordered_set<std::pair<_Object*, _Object*>> BSPTreeNode<_Object>::outputCollidingPairs()
    {
        return {};
#if 0
        std::unordered_set<std::pair<_Object*, _Object*>> res;
        //travel all the trees, find the leafs,int each leaves ,output all the paris
        std::stack<BSPTree<_Object>> st;
        st.push(_root);

        while (!st.empty()) {
            BSPTree<_Object> node = st.top();
            st.pop();
            if (node.isLeaf()) {
                for (int i = 0; i < _objs.size(); ++i) {
                    for (int j = i + 1; j < _objs.size(); ++j) {
                        res.insert(make_pair(_objs[i], _objs[j]));
                    }
                }
            }
        }

        return res;
#endif
    }

    /**************************BSPTree*************************/
    template<class _Object>
    class BSPTree
    {
    public:
        BSPTreeNode<_Object> _root;

        std::unordered_set<std::pair<_Object*, _Object*>> outputCollidingPairs();

        void build(std::vector<_Object*>& objs) {
            _root.build_r(objs);
        }
        // 计算与_Object可能接触的对象
        void collidingObjects(_Object*, std::vector<_Object*>& _ouptut);
    };

    template<class _Object>
    void BSPTree<_Object>::collidingObjects(_Object* obj, std::vector<_Object*>& _output)
    {
        if (_root)
            _root->collidingObjects(obj, _output);
    }
}

#endif