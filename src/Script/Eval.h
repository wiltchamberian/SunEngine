#ifndef __EVAL_H
#define __EVAL_H

#include "Ast.h"
#include "ObjectSys.h"

namespace Sun {

	inline bool isRightOperator(const char* op);
	inline std::shared_ptr<Error> error_type_mis_match(int rowNo, const char* t1, const char* op, const char* t2);
	inline std::shared_ptr<Error> error_type_mis_match(int rowNo, const char* op, const char* r);
	inline std::shared_ptr<Error> error_unknown_operator(int rowNo, const char* t1, const char* op, const char* t2);
	inline bool isErrorObj(Object_t* obj);
	inline std::shared_ptr<Error> newError(const char* format, ...);

	class EvalVisitor {
	public:
		std::shared_ptr<Object_t> visit(Program* pro,Environment* ev);
		std::shared_ptr<Object_t> visit(DefStatement* state, Environment* ev);
		std::shared_ptr<Object_t> visit(ReturnStatement* state, Environment* ev);
		std::shared_ptr<Object_t> visit(ExpressionStatement* state, Environment* ev);
		std::shared_ptr<Object_t> visit(BlockStatement* state, Environment* ev);
		std::shared_ptr<Object_t> visit(BooleanExp* state, Environment* ev);
		std::shared_ptr<Object_t> visit(Identifier* ident, Environment* ev);
		std::shared_ptr<Object_t> visit(IfExpression* exp, Environment* ev);
		std::shared_ptr<Object_t> visit(FunctionLiteral* exp, Environment* ev);
		std::shared_ptr<Object_t> visit(IntegerLiteral* exp, Environment* ev);
		std::shared_ptr<Object_t> visit(PrefixExpression* exp, Environment* ev);
		std::shared_ptr<Object_t> visit(InfixExpression* exp, Environment* ev);
		std::shared_ptr<Object_t> visit(CallExpression* exp, Environment* ev);

	protected:
		std::vector<std::shared_ptr<Object_t>> evalExpressions(const std::vector<Expression*>& vec, Environment* ev);
	};

}

#endif