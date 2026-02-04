#ifndef __AST_H
#define __AST_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_set>
#include "Token.h"
#include "environment.h"

/*
Expressin:
BooleanExp
Identifier
IntegerLiteral
PrefixExpression
InfixExpression
IfExpression
FunctionLiteral
StructLiteral
CallExpression

*/

namespace Sun {

#define ANT(abc) #abc

	class EvalVisitor;

	//global ref for readonly null
	//extern Null g_null;


	class AstNode {
	public:
		virtual ~AstNode();
		virtual std::string tokenLiteral() = 0;
		virtual std::string printString() { return std::string(); }
		virtual const char* nodeType() {
			return ANT(AstNode);
		}
		virtual std::vector<AstNode*> getChildren() { return {}; }
	};

	class Statement : public AstNode {
	public:
		Statement(const SToken& token) :tk(token) {}
		SToken tk;
		virtual std::string tokenLiteral() override;
		virtual void statementNode() = 0;
		virtual std::shared_ptr<Object_t> accept(EvalVisitor* visitor, Environment* ev) = 0;
		virtual const char* nodeType() {
			return ANT(Statement);
		}
	};

	struct Program : public AstNode {
		~Program();
		std::vector<Statement*> statements;
		std::string tokenLiteral() override;
		std::string printString() override;
		virtual std::shared_ptr<Object_t> accept(EvalVisitor*, Environment* ev);
		std::vector<AstNode*> getChildren() override;
		virtual const char* nodeType() {
			return ANT(Program);
		}
	};

	class Expression : public AstNode {
	public:
		Expression(const SToken& token) :tk(token) {}
		SToken tk;
		virtual std::string tokenLiteral() override { return tk.literal; }
		virtual void expressionNode() = 0;
		virtual const char* nodeType() = 0;
		virtual std::shared_ptr<Object_t> accept(EvalVisitor*, Environment* ev) = 0;
	};


	struct BooleanExp : public Expression {
		BooleanExp(const SToken& token, bool val) :Expression(token), value(val) {}
		bool value;
		void expressionNode() {}
		std::string printString() { return tk.literal; }
		std::shared_ptr<Object_t> accept(EvalVisitor*, Environment* ev) override;
		virtual const char* nodeType() {
			return ANT(BooleanExp);
		}
	};

	struct Identifier : public Expression {
		Identifier(const SToken& token, const std::string& val) :Expression(token), value(val) {}
		std::string value;
		void expressionNode() override {}
		std::string printString(); //must return value!
		std::shared_ptr<Object_t> accept(EvalVisitor*, Environment* ev) override;
		virtual const char* nodeType() {
			return ANT(Identifier);
		}
	};

	//泛化的指定义语句
	struct DefStatement : public Statement {
		DefStatement(const SToken& token) :Statement(token), ident(nullptr), express(nullptr) {}
		Identifier* ident;
		Expression* express;
		void statementNode()override {}
		std::string printString();
		std::shared_ptr<Object_t> accept(EvalVisitor* visitor, Environment* ev) override;
		std::vector<AstNode*> getChildren() override;
		virtual const char* nodeType() {
			return ANT(DefStatement);
		}
	};

	struct ReturnStatement : public Statement {
		Expression* returnValue;
		ReturnStatement(const SToken& token) :Statement(token),returnValue(nullptr) {}
		void statementNode() {}
		std::string printString();
		std::shared_ptr<Object_t> accept(EvalVisitor*, Environment* ev) override;
		virtual const char* nodeType() {
			return ANT(ReturnStatement);
		}
	};

	struct ExpressionStatement :public Statement {
		ExpressionStatement(const SToken& token) :Statement(token), expression(nullptr) {}
		Expression* expression;
		void statementNode() {}
		std::string printString();
		std::shared_ptr<Object_t> accept(EvalVisitor* v, Environment* ev) override;
		virtual const char* nodeType() {
			return ANT(ExpressionStatement);
		}
	};

	struct IntegerLiteral :public Expression {
		IntegerLiteral(SToken token) :Expression(token),value(0) {}
		std::int64_t value;
		void expressionNode() {}
		std::string printString() { return tk.literal; }
		std::shared_ptr<Object_t> accept(EvalVisitor* v, Environment* ev) override;
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
		std::shared_ptr<Object_t> accept(EvalVisitor* v, Environment* ev) override;	
		virtual const char* nodeType() {
			return ANT(PrefixExpression);
		}
	protected:
		//std::shared_ptr<Object_t> evalBangOperatorExpression();
	};

	struct InfixExpression : public Expression {
		friend class EvalVisitor;
		InfixExpression(const SToken& token, const char* Op)
			:Expression(token), op(Op), left(nullptr), right(nullptr) {}
		const char* op;
		Expression* left;
		Expression* right;
		void expressionNode() {}
		std::string printString();
		std::shared_ptr<Object_t> accept(EvalVisitor* v, Environment* ev) override;
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
		std::shared_ptr<Object_t> accept(EvalVisitor* v,Environment* ev) override;
		virtual const char* nodeType() {
			return ANT(BlockStatement);
		}
	};

	struct IfExpression : public Expression {
		friend class EvalVisiotr;
		IfExpression(const SToken& token) :Expression(token), condition(nullptr)
			, consequence(nullptr), alternative(nullptr) {}
		Expression* condition;
		BlockStatement* consequence;
		BlockStatement* alternative;

		void expressionNode() {}
		std::string printString();
		std::shared_ptr<Object_t> accept(EvalVisitor* v, Environment* ev) override;
		virtual const char* nodeType() {
			return ANT(IfExpression);
		}

		bool isTruthy(Object_t* obj) const;
	};

	struct FunctionLiteral :public Expression {
		FunctionLiteral(const SToken& token) :Expression(token), body(nullptr) {}
		std::vector<Identifier*> parameters;
		BlockStatement* body;
		void expressionNode() {}
		std::string printString();
		std::shared_ptr<Object_t> accept(EvalVisitor* v, Environment* ev) override;
		virtual const char* nodeType() {
			return ANT(FunctionLiteral);
		}
	};

	struct StructLiteral : public Expression {
		StructLiteral(const SToken& token) : Expression(token) {}
		void expressionNode() {}
		virtual const char* nodeType() { return ANT(StructLiteral); }
		std::string printString() { return std::string(); }
		std::shared_ptr<Object_t> accept(EvalVisitor* v, Environment* ev) override;
		
	};

	struct CallExpression :public Expression {
		friend class EvalVisitor;
		CallExpression(const SToken& token) :Expression(token), function(nullptr) {}
		Expression* function;
		std::vector<Expression*> arguments;
		void expressionNode() {}
		std::string printString();
		std::shared_ptr<Object_t> accept(EvalVisitor* v, Environment* ev) override;
		virtual const char* nodeType() {
			return ANT(CallExpression);
		}
	protected:
		Environment* extendFunctionEnv(const std::shared_ptr<Function>& fn, const std::vector < std::shared_ptr<Object_t>>& args);
		std::shared_ptr<Object_t> unwrapReturnValue(const std::shared_ptr<Object_t>& obj);
	};
}



#endif