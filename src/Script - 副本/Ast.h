#ifndef __AST_H
#define __AST_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_set>
#include "Token.h"
#include "environment.h"

namespace Sun {

#define ANT(abc) #abc

	//global ref for readonly null
	//extern Null g_null;

	inline bool isRightOperator(const char* op);
	inline std::shared_ptr<Error> error_type_mis_match(int rowNo,const char* t1, const char* op, const char* t2);
	inline std::shared_ptr<Error> error_type_mis_match(int rowNo,const char* op, const char* r);
	inline std::shared_ptr<Error> error_unknown_operator(int rowNo,const char* t1, const char* op, const char* t2);
	inline bool isErrorObj(Object_t* obj);

	class AstNode {
	public:
		virtual ~AstNode();
		virtual std::string tokenLiteral() = 0;
		virtual std::shared_ptr<Object_t> eval(Environment* ev) { return nullptr; }
		virtual std::string printString() { return std::string(); }
		virtual const char* nodeType() {
			return ANT(AstNode);
		}
	};

	class Statement : public AstNode {
	public:
		Statement(const SToken& token) :tk(token) {}
		SToken tk;
		std::string tokenLiteral() override;
		virtual void statementNode() = 0;
		virtual const char* nodeType() {
			return ANT(Statement);
		}
	};

	inline std::shared_ptr<Error> newError(const char* format, ...);

	struct Program : public AstNode {
		~Program();
		std::vector<Statement*> statements;
		std::string tokenLiteral() override;
		std::string printString() override;
		std::shared_ptr<Object_t> eval(Environment* ev);
		virtual const char* nodeType() {
			return ANT(Program);
		}
	};

	class Expression : public AstNode {
	public:
		Expression(const SToken& token) :tk(token) {}
		SToken tk;
		std::string tokenLiteral() override;
		virtual void expressionNode() = 0;
		virtual const char* nodeType() {
			return ANT(Expression);
		}
	};


	struct BooleanExp : public Expression {
		BooleanExp(const SToken& token, bool val) :Expression(token), value(val) {}
		bool value;
		void expressionNode() {}
		std::string printString() { return tk.literal; }
		std::shared_ptr<Object_t> eval(Environment* ev) override;
		virtual const char* nodeType() {
			return ANT(BooleanExp);
		}
	};

	struct Identifier : public Expression {
		Identifier(const SToken& token, const std::string& val) :Expression(token), value(val) {}
		std::string value;
		void expressionNode() override {}
		std::string printString();
		std::shared_ptr<Object_t> eval(Environment* ev) override;
		virtual const char* nodeType() {
			return ANT(Identifier);
		}
	};

	//泛化的指定义语句
	struct LetStatement : public Statement {
		LetStatement(const SToken& token) :Statement(token), ident(nullptr), express(nullptr) {}
		Identifier* ident;
		Expression* express;
		void statementNode()override {}
		std::string printString();
		std::shared_ptr<Object_t> eval(Environment* ev) override;
		virtual const char* nodeType() {
			return ANT(LetStatement);
		}
	};

	struct ReturnStatement : public Statement {
		Expression* returnValue;
		ReturnStatement(const SToken& token) :Statement(token),returnValue(nullptr) {}
		void statementNode() {}
		std::string printString();
		std::shared_ptr<Object_t> eval(Environment* ev) override;
		virtual const char* nodeType() {
			return ANT(ReturnStatement);
		}
	};

	struct ExpressionStatement :public Statement {
		ExpressionStatement(const SToken& token) :Statement(token), expression(nullptr) {}
		Expression* expression;
		void statementNode() {}
		std::string printString();
		std::shared_ptr<Object_t> eval(Environment* ev) override;
		virtual const char* nodeType() {
			return ANT(ExpressionStatement);
		}
	};

	struct IntegerLiteral :public Expression {
		IntegerLiteral(SToken token) :Expression(token),value(0) {}
		std::int64_t value;
		void expressionNode() {}
		std::string printString() { return tk.literal; }
		std::shared_ptr<Object_t> eval(Environment* ev) override;
		virtual const char* nodeType() {
			return ANT(IntegerLiteral);
		}
	};

	struct PrefixExpression : public Expression {
		PrefixExpression(const SToken& token, const char* Op)
			:Expression(token), op(Op),right(nullptr) {}
		const char* op;
		Expression* right;
		void expressionNode() {}
		std::string printString();
		std::shared_ptr<Object_t> eval(Environment* ev) override;	
		virtual const char* nodeType() {
			return ANT(PrefixExpression);
		}
	protected:
		//std::shared_ptr<Object_t> evalBangOperatorExpression();
	};

	struct InfixExpression : public Expression {
		InfixExpression(const SToken& token, const char* Op)
			:Expression(token), op(Op), left(nullptr), right(nullptr) {}
		const char* op;
		Expression* left;
		Expression* right;
		void expressionNode() {}
		std::string printString();
		std::shared_ptr<Object_t> eval(Environment* ev) override;
		virtual const char* nodeType() {
			return ANT(InfixExpression);
		}
	protected:
		std::shared_ptr<Object_t> evalIntegerInfixExpression(const std::string& op, Object_t* left, Object_t* right);
	};

	struct BlockStatement : public Statement {
		BlockStatement(const SToken& token) :Statement(token) {}
		std::vector<Statement*> statements;
		void statementNode() {}
		std::string printString();
		std::shared_ptr<Object_t> eval(Environment* ev) override;
		virtual const char* nodeType() {
			return ANT(BlockStatement);
		}
	};

	struct IfExpression : public Expression {
		IfExpression(const SToken& token) :Expression(token), condition(nullptr)
			, consequence(nullptr), alternative(nullptr) {}
		Expression* condition;
		BlockStatement* consequence;
		BlockStatement* alternative;

		void expressionNode() {}
		std::string printString();
		std::shared_ptr<Object_t> eval(Environment* ev) override;
		virtual const char* nodeType() {
			return ANT(IfExpression);
		}
	protected:
		bool isTruthy(Object_t* obj) const;
	};

	struct FunctionLiteral :public Expression {
		FunctionLiteral(const SToken& token) :Expression(token), body(nullptr) {}
		std::vector<Identifier*> parameters;
		BlockStatement* body;
		void expressionNode() {}
		std::string printString();
		std::shared_ptr<Object_t> eval(Environment* ev) override;
		virtual const char* nodeType() {
			return ANT(FunctionLiteral);
		}
	};

	struct StructLiteral : public Expression {
		StructLiteral(const SToken& token) : Expression(token) {}
		void expressionNode() {}
		std::string printString() { return std::string(); }
		BlockStatement* blockStatement;
	};

	struct CallExpression :public Expression {
		CallExpression(const SToken& token) :Expression(token), function(nullptr) {}
		Expression* function;
		std::vector<Expression*> arguments;
		void expressionNode() {}
		std::string printString();
		std::shared_ptr<Object_t> eval(Environment* ev) override;
		virtual const char* nodeType() {
			return ANT(CallExpression);
		}
	protected:
		std::vector<std::shared_ptr<Object_t>> evalExpressions(Environment* ev);
		std::shared_ptr<Object_t> applyFunction(const std::shared_ptr<Object_t>& fn, const std::vector<std::shared_ptr<Object_t>>& args);
		Environment* extendFunctionEnv(const std::shared_ptr<Function>& fn, const std::vector < std::shared_ptr<Object_t>>& args);
		std::shared_ptr<Object_t> unwrapReturnValue(const std::shared_ptr<Object_t>& obj);
	};
}



#endif