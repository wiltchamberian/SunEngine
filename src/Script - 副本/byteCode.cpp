#include "byteCode.h"

namespace Sun {

	void Stack::pushInteger(int num) {
		buffer.push_back(num);
	}

	void Stack::pushFloat(float num) {
		buffer.emplace_back();
		memcpy(&buffer.back(), &num, 4);
	}

	void Stack::pushDouble(double num) {

	}
	void Stack::pushOpCode(int op) {
		buffer.push_back(op);
	}

	void Stack::pop() {
		buffer.pop_back();
	}

	Stack astToByteCode(Program* pro) {
		AstNode* root = dynamic_cast<AstNode*>(pro);
		//采用后序遍历的方式遍历ast树，将每个节点解析出存入栈中
		Stack stack;



		return stack;
	}

}