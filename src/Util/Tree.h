/*****************************************************************************
* @brief : tree
* 通用多叉树结构，通过首孩子指针，父指针，和兄弟指针描述
* @author : acedtang
* @date : 2021/3/26
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/

#ifndef __TREE_H
#define __TREE_H

#include <functional>
#include <deque>

namespace Sun {

#if 0
    template<class _T>
    struct TreeNode {
        std::vector<int> childrenIndex;
        int parent;
        //自身索引
        int index;
    };

    template<class _T>
    class Tree {
    public:
        //add child to ptr
        TreeNode<_T>* addChild(TreeNode<_T>* ptr, const _T& data) {
            TreeNode<_T> node;
            node.data = data;
            nodes_.push_back(node);
            node.index = nodes_.size() - 1;
            if (ptr != nullptr) {
                ptr->childrenIndex.push_back(node.index);
                node.parent = ptr->index;
            }
            else {
                node.parent = -1;
            }
            return &nodes_.back();
        }
        TreeNode<_T>* getRoot() {
            return nodes_.empty() ? nullptr : nodes_[0];
        }
    private:
        std::vector<TreeNode<_T>> nodes_;
    };
#endif

    template<class _DerivedPointer>
    class TreeNodeBase {
    public:
        TreeNodeBase()
        :firstChild_(nullptr)
        ,parent_(nullptr)
        ,prevSlibing_(nullptr)
        ,nextSlibing_(nullptr){

        }
        _DerivedPointer firstChild_;
        _DerivedPointer parent_;
        _DerivedPointer prevSlibing_;
        _DerivedPointer nextSlibing_; 

        bool isRoot() {
            return parent_ == nullptr;
        }
    };

    template<class _TreeNodePointer>
    void addChild(_TreeNodePointer parent, _TreeNodePointer child) {
        if (!child) return;
        _TreeNodePointer s = parent->firstChild_;
        parent->firstChild_ = child;
        child->parent_ = parent;
        child->nextSlibing_ = s;
        if(s)
            s->prevSlibing_ = child;
    }

    template<class _TreeNodePointer>
    void detachFrom(_TreeNodePointer self) {
        if (!self) return;
        //去除其它rotor对自身的引用
        if (self->parent_ && self->parent_->firstChild_ == self) {
            self->parent_->firstChild_ = self->nextSlibing_;
            if (self->nextSlibing_) {
                self->nextSlibing_->prevSlibing_ = nullptr;
            }
        }
        else if (self->parent_) {
            if (self->prevSlibing_)
                self->prevSlibing_->nextSlibing_ = self->nextSlibing_;
            if (self->nextSlibing_)
                self->nextSlibing_->prevSlibing_ = self->prevSlibing_;
        }
        //去除自身对其它节点的引用
        self->nextSlibing_ = nullptr;
        self->prevSlibing_ = nullptr;
        self->parent_ = nullptr;
    }

    //广度优先遍历子树
    template<class _TreeNodePointer>
    void travel(_TreeNodePointer self, std::function<void(_TreeNodePointer)> func) {
#if 1
        if (!self) return;
        func(self);
        //this is a optimize for no child nodes;
        if (!self->firstChild_) {
            return;
        }
        std::deque<_TreeNodePointer> deq;
        deq.push_back(self);
        while (!deq.empty()) {
            _TreeNodePointer pt = deq.front();
            deq.pop_front();
            func(pt);
            _TreeNodePointer s = pt->firstChild_;
            while (s) {
                deq.push_back(s);
                s = s->nextSlibing_;
            }
        } 
        return;
#endif 

#if 0
        func(self);
        return;
#endif
    }
}

#endif


