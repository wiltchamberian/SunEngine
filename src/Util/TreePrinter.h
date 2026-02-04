#ifndef __TREE_PRINTER_H
#define __TREE_PRINTER_H

#include "TreePrinter.h"
#include <functional>
#include <vector>
#include <iostream>

namespace Sun {

	/*  demo
	void callback(_TreeNode* node){
		printf("%05d---", node->val);
	}
	*/

	template<class _Tree, class _TreeNode>
	class TreePrinter {
	public:
		TreePrinter(_Tree* tree) :tree_(tree) {}
		void print(_TreeNode* root, std::function<void(_TreeNode*)> callback,bool isFirstChild = true);

	protected:
		_Tree* tree_;
		std::vector<bool> g_stack;
	};

#define PRINT_HEIGHT 2
#define PLACE_HOLDER "--------";
#define CONNECTOR "|       "
#define WHITE_SPACE "        "

	template<class _Tree, class _TreeNode>
	void TreePrinter<_Tree,_TreeNode>::print(_TreeNode* root, std::function<void(_TreeNode*)> callback,bool isFristChild) {
		if (isFristChild) {
			callback(root);
		}
		else {
			for (int i = 0; i < g_stack.size() - 1; ++i) {
				if (g_stack[i]) {
					std::cout << CONNECTOR;
				}
				else {
					std::cout << WHITE_SPACE;
				}
			}
			std::cout << PLACE_HOLDER;
			callback(root);
		}

		//р╤вс╫з╣Ц
		if (root->isLeaf()) {
			std::cout << std::endl;
			for (int row = 1; row < PRINT_HEIGHT; ++row) {
				for (int i = 0; i < g_stack.size(); ++i) {
					if (g_stack[i]) {
						std::cout << CONNECTOR;
					}
					else {
						std::cout << WHITE_SPACE;
					}
				}
				std::cout << std::endl;
			}
		}

		std::vector<_TreeNode*> vec = root->getChildren(tree_);
		for (int i = 0; i <vec.size(); ++i) {
			if (i == vec.size() - 1) {
				g_stack.push_back(false);
			}
			else {
				g_stack.push_back(true);
			}
			print(vec[i], callback,i==0);
			g_stack.pop_back();
		}
	}

}

#endif