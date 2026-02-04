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

	void RegisterStack::pushFloat(float num) {
		buffer.emplace_back();
		memcpy(&buffer.back(), &num, sizeof(int));
	}

	void RegisterStack::pushInt(int num) {
		buffer.push_back(num);
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="ConverterVisitor"></param>
	/// <returns></returns>
	void ConverterVisitor::astToByteCode(Program* pro) {
		AstNode* root = dynamic_cast<AstNode*>(pro);
		//采用后序遍历的方式遍历ast树，将每个节点解析出存入栈中

		//root->accept(this);

		return ;
	}

	void ConverterVisitor::visit(Program* pro) {
		for (auto& it : pro->statements) {
			//visit(it);
		}
	}

	void ConverterVisitor::visit(InfixExpression* exp) {
		//exp->left->accept(this);

		//exp->right->accept(this);

		//stack.pushOpCode(exp->op);
	}
}