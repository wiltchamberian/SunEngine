#include "Ast.h"
#include <cassert>
#include <memory>
#include <cstdarg>

namespace Sun {

	//Null g_null;
	bool isRightOperator(const char* op) {
		static std::unordered_set<const char*> opSet = {
			"+",
			"-",
			"*",
			"/",
			"<",
			">",
			"==",
			"!=",
			//acedtang 
			"=",
		};
		return opSet.find(op) != opSet.end();
	}

	std::shared_ptr<Error> error_type_mis_match(int rowNo,const char* t1, const char* op, const char* t2) {
		return newError("line %d\ntype mismatch: %s %s %s\n",rowNo, t1, op, t2);
	}

	std::shared_ptr<Error> error_type_mis_match(int rowNo,const char* op, const char* r) {
		return newError("line %d\ntype mismatch: %s %s\n",rowNo, op, r);
	}

	std::shared_ptr<Error> error_unknown_operator(int rowNo, const char* t1, const char* op, const char* t2) {
		return newError("line %d\nunknown operator %s %s %s\n",rowNo, t1,op,t2);
	}

	bool isErrorObj(Object_t* obj) {
		if (obj != nullptr && obj->type() == ERROR_OBJ) {
			return true;
		}
		return false;
	}

	AstNode::~AstNode() {

	}

	std::shared_ptr<Error> newError(const char* format, ...) {
		char buf[1024];
		va_list args;
		va_start(args, format);
		vsprintf(buf,format, args);
		std::shared_ptr<Error> er(new Error(buf));
		va_end(args);
		return er;
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

	std::shared_ptr<Object_t> Program::eval(Environment* ev) {
		std::shared_ptr<Object_t> res = nullptr;
		for (auto& statement : statements) {
			res = statement->eval(ev);
			if (res && res->isReturn) {
				return res;
			}
			if (isErrorObj(res.get())) {
				return res;
			}
		}
		return res;
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

	std::string Expression::tokenLiteral() {
		return tk.literal;
	}

	std::shared_ptr<Object_t> BooleanExp::eval(Environment* ev) {
		return std::shared_ptr<Boolean>(new Boolean(value));
	}
	
	std::string Identifier::printString() {
		return value;
	}

	std::shared_ptr<Object_t> Identifier::eval(Environment* ev) {
		std::shared_ptr<Object_t> obj = ev->get(value);
		if (obj == nullptr) {
			return newError("line:%d\nidentifier not found:",tk.rowNo, value.c_str());
		}
		return obj;
	}

	std::string LetStatement::printString() {
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

	std::shared_ptr<Object_t> LetStatement::eval(Environment* ev) {
		if (express == nullptr) {
			return nullptr;
		}
		std::shared_ptr<Object_t> obj = express->eval(ev);
		if (isErrorObj(obj.get())) {
			return obj;
		}
		ev->set(ident->value, obj);
		return obj;
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

	std::shared_ptr<Object_t> ReturnStatement::eval(Environment* ev) {
		if (returnValue) {
			std::shared_ptr<Object_t> obj  = returnValue->eval(ev);
			if (isErrorObj(obj.get())) {
				return obj;
			}
			if (obj) {
				obj->isReturn = true;
				return obj;
			}		
		}
			
		return nullptr;
	}

	std::string ExpressionStatement::printString() {
		if (expression != nullptr) {
			return expression->printString();
		}
		return std::string();
	}

	std::shared_ptr<Object_t> ExpressionStatement::eval(Environment* ev) {
		std::shared_ptr<Object_t> obj = nullptr;
		if (expression) {
			obj = expression->eval(ev);
		}
		else {
			assert(false);
		}
		return obj;
	}

	std::shared_ptr<Object_t> IntegerLiteral::eval(Environment* ev) {
		std::shared_ptr<Integer> obj(new Integer(value));
		return obj;
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

	std::shared_ptr<Object_t> PrefixExpression::eval(Environment* ev) {
		if (right == nullptr) {
			return newError("line:%d \n operator %s has no right oprand\n", tk.rowNo, op);
		}
		std::shared_ptr<Object_t> res = right->eval(ev);
		if (isErrorObj(res.get())) {
			return res;
		}
		if (op == T_BANG) {		
			if (res->type() == BOOLEAN_OBJ) {
				res->setBoolValue(!res->boolValue());
				return res;
			}
			else if (res->type() == INTEGER_OBJ){
				std::shared_ptr<Boolean> obj(new Boolean(!res->boolValue()));
				return obj;
			}
			else {
				ObjectType type = res->type();
				return error_type_mis_match(tk.rowNo, op, type);
			}
		}
		else if (op == T_MINUS) {
			if (res == nullptr || res->type() != INTEGER_OBJ) {
				return newError("unknown operator: %s", op);
			}
			res->setIntValue(-res->intValue());
			return res;
		}
		return newError("unknown operator :%s %s\n", op, right->nodeType());
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

	std::shared_ptr<Object_t> InfixExpression::eval(Environment* ev) {
		std::shared_ptr < Object_t> lobj(left->eval(ev));
		if (isErrorObj(lobj.get())) {
			return lobj;
		}
		if (lobj == nullptr) {
			return newError("line:%d\noperator %s has no left operand\n",tk.rowNo, op);
		}
		std::shared_ptr<Object_t> robj(right->eval(ev));
		if (isErrorObj(robj.get())) {
			return robj;
		}
		if (robj == nullptr) {
			return newError("line:%d\noperator %s has no right operand\n", tk.rowNo, op);
		}
		if (lobj->type() != robj->type()) {
			return error_type_mis_match(tk.rowNo,lobj->type(), op, robj->type());
		}
		bool ok = isRightOperator(op);
		if (!ok) {
			return error_type_mis_match(tk.rowNo, lobj->type(), op, robj->type());
		}
		if (lobj->type() == INTEGER_OBJ && robj->type() == INTEGER_OBJ) {
			return evalIntegerInfixExpression(op, lobj.get(), robj.get());
		}
		if (lobj->type() == BOOLEAN_OBJ && robj->type() == BOOLEAN_OBJ) {
			if (op == "==") {
				return std::shared_ptr<Boolean>(new Boolean(lobj->boolValue() == robj->boolValue()));
			}
			else if (op == "!=") {
				return std::shared_ptr<Boolean>(new Boolean(lobj->boolValue() != robj->boolValue()));
			}
			return error_type_mis_match(tk.rowNo, lobj->type(), op, robj->type());
		}
		if (lobj->type() != robj->type()) {
			return error_type_mis_match(tk.rowNo, lobj->type(), op, robj->type());
		}

		return error_unknown_operator(tk.rowNo, lobj->type(), op, robj->type());
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

	std::shared_ptr<Object_t> BlockStatement::eval(Environment* ev) {
		std::shared_ptr<Object_t> res = nullptr;
		for (auto& statement : statements) {
			res = statement->eval(ev);
			if (res && res->isReturn) {
				return res;
			}
			if (res && res->type() == ERROR_OBJ) {
				return res;
			}
		}
		return res;
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

	std::shared_ptr<Object_t> IfExpression::eval(Environment* ev) {
		std::shared_ptr<Object_t> cond(condition->eval(ev));
		if (isErrorObj(cond.get())) {
			return cond;
		}
		if (isTruthy(cond.get())) {
			return consequence->eval(ev);
		}
		else if (alternative != nullptr) {
			return alternative->eval(ev);
		}
		return nullptr;
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

	std::shared_ptr<Object_t> FunctionLiteral::eval(Environment* ev) {
		return std::shared_ptr<Function>(new Function(parameters,body,ev));
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

	std::shared_ptr<Object_t> CallExpression::eval(Environment* ev) {
		std::shared_ptr<Object_t> fn = function->eval(ev);
		if (isErrorObj(fn.get())) {
			return fn;
		}
		auto args = evalExpressions(ev);
		if (args.size() == 1 && isErrorObj(args[0].get())) {
			return args[0];
		}
		return applyFunction(fn, args);
	}

	std::vector<std::shared_ptr<Object_t>> CallExpression::evalExpressions(Environment* ev) {
		std::vector<std::shared_ptr<Object_t>> res;

		for (auto& it : arguments) {
			auto evaluated = it->eval(ev);
			if (isErrorObj(evaluated.get())) {
				return { evaluated };
			}
			res.push_back(evaluated);
		}

		return res;
	}

	std::shared_ptr<Object_t> CallExpression::applyFunction(const std::shared_ptr<Object_t>& func, const std::vector<std::shared_ptr<Object_t>>& args) {
		std::shared_ptr<Function> fn = std::dynamic_pointer_cast<Function>(func);
		if (!fn) {
			return newError("not a function: %s\n", fn->type());
		}
		Environment* extendEv = extendFunctionEnv(fn, args);
		auto evaluated = fn->body->eval(extendEv);
		return unwrapReturnValue(evaluated);
	}

	Environment* CallExpression::extendFunctionEnv(const std::shared_ptr<Function>& fn, const std::vector < std::shared_ptr<Object_t>>& args) {
		Environment* env = newEnclosedEnvironment(fn->ev);
		assert(fn->parameters.size() == args.size());
		for (int i = 0; i < fn->parameters.size(); ++i) {
			env->set(fn->parameters[i]->value, args[i]);
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