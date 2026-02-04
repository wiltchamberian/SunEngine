#include "Ast.h"
#include <cassert>
#include <memory>
#include "Eval.h"

namespace Sun {

	AstNode::~AstNode() {

	}

	Program::~Program() {
		for (int i = 0; i < statements.size(); ++i) {
			delete statements[i];
		}
	}

	std::string Program::printString() {
		std::string str;
		for (int i = 0; i < statements.size(); ++i) {
			str.append(statements[i]->printString());
		}
		return str;
	}

	std::vector<AstNode*> Program::getChildren() {
		return {};
	}

	std::shared_ptr<Object_t> Program::accept(EvalVisitor* visitor, Environment* ev) {
		return visitor->visit(this, ev);
	}

	std::string Program::tokenLiteral() {
		if (statements.size() > 0) {
			return statements[0]->tokenLiteral();
		}
		return "";
	}

	std::string Statement::tokenLiteral() {
		return tk.literal;
	}

	std::shared_ptr<Object_t> BooleanExp::accept(EvalVisitor* visitor, Environment* ev) {
		return visitor->visit(this, ev);
	}
	
	std::string Identifier::printString() {
		return value;
	}

	std::shared_ptr<Object_t> Identifier::accept(EvalVisitor* v, Environment* ev) {
		return v->visit(this, ev);
	}

	std::string DefStatement::printString() {
		std::string res;
		res.append(tokenLiteral() + " ");
		res.append(ident->printString());
		res.append(" = ");

		if (express != nullptr) {
			res.append(express->printString());
		}
		res.append(";");
		return res;
	}

	std::shared_ptr<Object_t> DefStatement::accept(EvalVisitor* visitor, Environment* ev) {
		return visitor->visit(this, ev);
	}

	std::vector<AstNode*> DefStatement::getChildren() {
		std::vector<AstNode*> vec;
		vec.push_back(ident);
		vec.push_back(express);
		return vec;
	}

	std::string ReturnStatement::printString() {
		std::string res;
		res.append(tokenLiteral());
		if (returnValue != nullptr) {
			res.append(returnValue->printString());
		}
		res.append(";");
		return res;
	}

	std::shared_ptr<Object_t> ReturnStatement::accept(EvalVisitor* v, Environment* ev) {
		return v->visit(this, ev);
	}

	std::string ExpressionStatement::printString() {
		if (expression != nullptr) {
			return expression->printString();
		}
		return std::string();
	}

	std::shared_ptr<Object_t> ExpressionStatement::accept(EvalVisitor* v, Environment* ev) {
		return v->visit(this, ev);
	}

	std::shared_ptr<Object_t> IntegerLiteral::accept(EvalVisitor* v, Environment* ev) {
		return v->visit(this, ev);
	}

	std::string PrefixExpression::printString() {
		std::string str;
		str.append("(");
		str.append(op);
		if (right)
			str.append(right->printString());
		str.append(")");
		return str;
	}

	std::shared_ptr<Object_t> PrefixExpression::accept(EvalVisitor* v, Environment* ev) {
		return v->visit(this, ev);
	}

	std::string InfixExpression::printString() {
		std::string res;
		res.append("(");
		if (left)
			res.append(left->printString());
		res.append(" "); res.append(op); res.append(" ");
		if (right)
			res.append(right->printString());
		res.append(")");
		return res;
	}

	std::shared_ptr<Object_t> InfixExpression::accept(EvalVisitor* v, Environment* ev) {
		return v->visit(this, ev);
	}

	std::shared_ptr<Object_t> InfixExpression::evalIntegerInfixExpression(const std::string& op, Object_t* left, Object_t* right) {
		if (op == "+") {
			return std::shared_ptr<Integer>(new Integer(left->intValue() + right->intValue()));
		}
		else if (op == "-") {
			return std::shared_ptr<Integer>(new Integer(left->intValue() - right->intValue()));
		}
		else if (op == "*") {
			return std::shared_ptr<Integer>(new Integer(left->intValue() * right->intValue()));
		}
		else if (op == "/") {
			return std::shared_ptr<Integer>(new Integer(left->intValue() / right->intValue()));
		}
		else if (op == "<") {
			return std::shared_ptr<Boolean>(new Boolean(left->intValue() < right->intValue()));
		}
		else if (op == ">") {
			return std::shared_ptr<Boolean>(new Boolean(left->intValue() > right->intValue()));
		}
		else if (op == "==") {
			return std::shared_ptr<Boolean>(new Boolean(left->intValue() == right->intValue()));
		}
		else if (op == "!=") {
			return std::shared_ptr<Boolean>(new Boolean(left->intValue() != right->intValue()));
		}
		//acedtang 
		else if (op == "=") {
			return std::shared_ptr<Integer>(new Integer(right->intValue()));
		}
		return error_unknown_operator(tk.rowNo, left->type(),op.c_str(),right->type());
	}

	std::string BlockStatement::printString() {
		std::string str;
		for (auto& it : statements) {
			str.append(it->printString());
		}
		return str;
	}

	std::shared_ptr<Object_t> BlockStatement::accept(EvalVisitor* v, Environment* ev) {
		return v->visit(this, ev);
	}

	std::string IfExpression::printString() {
		std::string str;

		if (condition != nullptr) {
			str.append("if");
			str.append(condition->printString());
			str.append(" ");
			str.append(consequence->printString());
		}

		if (alternative != nullptr) {
			str.append("else ");
			str.append(alternative->printString());
		}

		return str;
	}

	std::shared_ptr<Object_t> IfExpression::accept(EvalVisitor* v, Environment* ev) {
		return v->visit(this, ev);
	}

	bool IfExpression::isTruthy(Object_t* obj) const {
		if (obj == nullptr) {
			return false;
		}
		if (obj->type() == BOOLEAN_OBJ && obj->boolValue() == false) {
			return false;
		}

		return true;
	}

	std::string FunctionLiteral::printString() {
		std::string buf;
		buf.append(tokenLiteral());
		buf.append("(");
		std::vector<std::string> params;
		for (int i = 0; i < parameters.size(); ++i) {
			if (i != 0)
				buf.append(", ");
			buf.append(parameters[i]->printString());
		}
		buf.append(")");
		buf.append(body->printString());
		return buf;
	}

	std::shared_ptr<Object_t> FunctionLiteral::accept(EvalVisitor* v, Environment* ev) {
		return v->visit(this, ev);
	}

	std::shared_ptr<Object_t> StructLiteral::accept(EvalVisitor* v, Environment* ev) {
		//TODO
		return nullptr;
	}

	std::string CallExpression::printString() {
		std::string buf;
		if (function == nullptr) return buf;
		buf.append(function->printString());
		buf.append("(");
		for (int i = 0; i < arguments.size(); ++i) {
			if (i != 0) {
				buf.append(", ");
			}
			buf.append(arguments[i]->printString());
		}
		buf.append(")");
		return buf;
	}

	std::shared_ptr<Object_t> CallExpression::accept(EvalVisitor* v, Environment* ev) {
		return v->visit(this, ev);
	}

	Environment* CallExpression::extendFunctionEnv(const std::shared_ptr<Function>& fn, const std::vector < std::shared_ptr<Object_t>>& args) {
		Environment* env = newEnclosedEnvironment(fn->ev);
		assert(fn->parameters.size() == args.size());
		for (int i = 0; i < fn->parameters.size(); ++i) {
			env->set(fn->parameters[i]->printString(), args[i]);
		}
		return env;
	}

	std::shared_ptr<Object_t> CallExpression::unwrapReturnValue(const std::shared_ptr<Object_t>& obj) {
		if (obj->isReturn) {
			return obj;
		}
		return obj;
	}
}