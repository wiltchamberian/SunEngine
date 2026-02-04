#include "Eval.h"
#include <cassert>
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

	std::shared_ptr<Error> error_type_mis_match(int rowNo, const char* t1, const char* op, const char* t2) {
		return newError("line %d\ntype mismatch: %s %s %s\n", rowNo, t1, op, t2);
	}

	std::shared_ptr<Error> error_type_mis_match(int rowNo, const char* op, const char* r) {
		return newError("line %d\ntype mismatch: %s %s\n", rowNo, op, r);
	}

	std::shared_ptr<Error> error_unknown_operator(int rowNo, const char* t1, const char* op, const char* t2) {
		return newError("line %d\nunknown operator %s %s %s\n", rowNo, t1, op, t2);
	}

	bool isErrorObj(Object_t* obj) {
		if (obj != nullptr && obj->type() == ERROR_OBJ) {
			return true;
		}
		return false;
	}

	std::shared_ptr<Error> newError(const char* format, ...) {
		char buf[1024];
		va_list args;
		va_start(args, format);
		vsprintf(buf, format, args);
		std::shared_ptr<Error> er(new Error(buf));
		va_end(args);
		return er;
	}

	std::shared_ptr<Object_t> EvalVisitor::visit(Program* pro,Environment* ev) {
		std::shared_ptr<Object_t> res = nullptr;
		for (auto& statement : pro->statements) {
			res = statement->accept(this, ev);
			if (res && res->isReturn) {
				return res;
			}
			if (isErrorObj(res.get())) {
				return res;
			}
		}
		return res;

	}

	std::shared_ptr<Object_t> EvalVisitor::visit(DefStatement* stmt, Environment* ev) {
		if (stmt->express == nullptr) {
			return nullptr;
		}
		std::shared_ptr<Object_t> obj = stmt->express->accept(this, ev);
		if (isErrorObj(obj.get())) {
			return obj;
		}
		ev->set(stmt->ident->value, obj);
		return obj;

	}

	std::shared_ptr<Object_t> EvalVisitor::visit(ReturnStatement* stmt, Environment* ev) {
		if (stmt->returnValue) {
			std::shared_ptr<Object_t> obj = stmt->returnValue->accept(this, ev);
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

	std::shared_ptr<Object_t> EvalVisitor::visit(ExpressionStatement* stmt, Environment* ev) {
		std::shared_ptr<Object_t> obj = nullptr;
		if (stmt->expression) {
			obj = stmt->expression->accept(this,ev);
		}
		else {
			assert(false);
		}
		return obj;
	}

	std::shared_ptr<Object_t> EvalVisitor::visit(BlockStatement* stmt, Environment* ev) {
		std::shared_ptr<Object_t> res = nullptr;
		for (auto& statement : stmt->statements) {
			res = statement->accept(this, ev);
			if (res && res->isReturn) {
				return res;
			}
			if (res && res->type() == ERROR_OBJ) {
				return res;
			}
		}
		return res;
	}

	std::shared_ptr<Object_t> EvalVisitor::visit(BooleanExp* state, Environment* ev) {
		return std::shared_ptr<Boolean>(new Boolean(state->value));
	}

	std::shared_ptr<Object_t> EvalVisitor::visit(Identifier* ident, Environment* ev) {
		std::shared_ptr<Object_t> obj = ev->get(ident->value);
		if (obj == nullptr) {
			return newError("line:%d\nidentifier not found:", ident->tk.rowNo, ident->value.c_str());
		}
		return obj;
	}

	std::shared_ptr<Object_t> EvalVisitor::visit(IfExpression* exp, Environment* ev) {
		std::shared_ptr<Object_t> cond(exp->condition->accept(this, ev));
		if (isErrorObj(cond.get())) {
			return cond;
		}
		if (exp->isTruthy(cond.get())) {
			return exp->consequence->accept(this, ev);
		}
		else if (exp->alternative != nullptr) {
			return exp->alternative->accept(this, ev);
		}
		return nullptr;
	}

	std::shared_ptr<Object_t> EvalVisitor::visit(FunctionLiteral* exp, Environment* ev) {
		return std::shared_ptr<Function>(new Function(exp->parameters, exp->body, ev));
	}

	std::shared_ptr<Object_t> EvalVisitor::visit(IntegerLiteral* exp, Environment* ev) {
		return std::shared_ptr<Integer>(new Integer(exp->value));
	}

	std::shared_ptr<Object_t> EvalVisitor::visit(PrefixExpression* exp, Environment* ev) {
		if (exp->right == nullptr) {
			return newError("line:%d \n operator %s has no right oprand\n", exp->tk.rowNo, exp->op);
		}
		std::shared_ptr<Object_t> res = exp->right->accept(this, ev);
		if (isErrorObj(res.get())) {
			return res;
		}
		if (exp->op == T_BANG) {
			if (res->type() == BOOLEAN_OBJ) {
				res->setBoolValue(!res->boolValue());
				return res;
			}
			else if (res->type() == INTEGER_OBJ) {
				std::shared_ptr<Boolean> obj(new Boolean(!res->boolValue()));
				return obj;
			}
			else {
				ObjectType type = res->type();
				return error_type_mis_match(exp->tk.rowNo, exp->op, type);
			}
		}
		else if (exp->op == T_MINUS) {
			if (res == nullptr || res->type() != INTEGER_OBJ) {
				return newError("unknown operator: %s", exp->op);
			}
			res->setIntValue(-res->intValue());
			return res;
		}
		return newError("unknown operator :%s %s\n", exp->op, exp->right->nodeType());
	}

	std::shared_ptr<Object_t> EvalVisitor::visit(InfixExpression* exp, Environment* ev) {
		std::shared_ptr < Object_t> lobj(exp->left->accept(this, ev));
		if (isErrorObj(lobj.get())) {
			return lobj;
		}
		if (lobj == nullptr) {
			return newError("line:%d\noperator %s has no left operand\n", exp->tk.rowNo, exp->op);
		}
		std::shared_ptr<Object_t> robj(exp->right->accept(this, ev));
		if (isErrorObj(robj.get())) {
			return robj;
		}
		if (robj == nullptr) {
			return newError("line:%d\noperator %s has no right operand\n", exp->tk.rowNo, exp->op);
		}
		if (lobj->type() != robj->type()) {
			return error_type_mis_match(exp->tk.rowNo, lobj->type(), exp->op, robj->type());
		}
		bool ok = isRightOperator(exp->op);
		if (!ok) {
			return error_type_mis_match(exp->tk.rowNo, lobj->type(), exp->op, robj->type());
		}
		if (lobj->type() == INTEGER_OBJ && robj->type() == INTEGER_OBJ) {
			return exp->evalIntegerInfixExpression(exp->op, lobj.get(), robj.get());
		}
		if (lobj->type() == BOOLEAN_OBJ && robj->type() == BOOLEAN_OBJ) {
			if (exp->op == "==") {
				return std::shared_ptr<Boolean>(new Boolean(lobj->boolValue() == robj->boolValue()));
			}
			else if (exp->op == "!=") {
				return std::shared_ptr<Boolean>(new Boolean(lobj->boolValue() != robj->boolValue()));
			}
			return error_type_mis_match(exp->tk.rowNo, lobj->type(), exp->op, robj->type());
		}
		if (lobj->type() != robj->type()) {
			return error_type_mis_match(exp->tk.rowNo, lobj->type(), exp->op, robj->type());
		}

		return error_unknown_operator(exp->tk.rowNo, lobj->type(), exp->op, robj->type());
	}

	std::shared_ptr<Object_t> EvalVisitor::visit(CallExpression* exp, Environment* ev) {
		//std::shared_ptr<Object_t> fn = exp->function->eval(ev);
		std::shared_ptr<Object_t> fn = exp->function->accept(this, ev);
		if (isErrorObj(fn.get())) {
			return fn;
		}

		auto args = evalExpressions(exp->arguments, ev);

		if (args.size() == 1 && isErrorObj(args[0].get())) {
			return args[0];
		}

		//return applyFunction(fn, args);
		std::shared_ptr<Function> func = std::dynamic_pointer_cast<Function>(fn);
		if (!func) {
			return newError("not a function: %s\n", func->type());
		}
		Environment* extendEv = exp->extendFunctionEnv(func, args);
		auto evaluated = func->body->accept(this, extendEv);
		return exp->unwrapReturnValue(evaluated);
	}

	std::vector<std::shared_ptr<Object_t>> EvalVisitor::evalExpressions(const std::vector<Expression*>& arguments, Environment* ev) {
		std::vector<std::shared_ptr<Object_t>> args;
		for (auto& it : arguments) {
			auto evaluated = it->accept(this, ev);
			if (isErrorObj(evaluated.get())) {
				return { evaluated };
			}
			args.push_back(evaluated);
		}
		return args;
	}
}