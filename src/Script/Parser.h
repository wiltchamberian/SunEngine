#ifndef __PARSER_H
#define __PARSER_H

#include "Lexer.h"
#include <cstdarg>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <functional>
#include "Ast.h"

namespace Sun {
	//表示优先级，顺序必须按照优先级从低到高排列
	enum Order {
		LOWEST, 
		ASSIGN, 
		EQUALS,   //==
		LESS_GREATER,     // < or >
		SUM,     //+
		PRODUCT, //*
		PREFIX,  //-X or !X
		CALL     //myFunction(X)
	};

	inline void errorf(char const* const _Format,...) {
		va_list args;
		va_start(args, _Format);
		vprintf(_Format, args);
		va_end(args);
	}
	using PrefixParseFn = std::function<Expression*()>;
	using InfixParseFn = std::function<Expression* (Expression*)>;

	class SCRIPT_API Parser {
	public:
		Parser(Lexer* lex);
		void clearAndSetInput(const std::string& input);
		//将程序代码转为AST,返回AST的根节点
		Program* parseProgram();
		std::vector<std::string> outputErrors() const;		
	protected:
		void registerPrefix(TokenType tkType, PrefixParseFn fn);
		void registerInfix(TokenType tkType, InfixParseFn fn);
		void peekError(TokenType type);
		Statement* parseStatement();
		DefStatement* parseDefStatement();
		ReturnStatement* parseReturnStatement();		
		ExpressionStatement* parseExpressionStatement();
		BlockStatement* parseBlockStatement();
		std::vector<Identifier*> parseFunctionParameters();
		Expression* parseExpression(Order order); //最精髓的代码片段之一
		Expression* parseIdentifier();
		Expression* parseBoolean();
		Expression* parseGroupedExpression();
		Expression* parseIfExpression();
		Expression* parseFunctionLiteral();
		Expression* parseStruct();
		Expression* parseIntegerLiteral();
		Expression* parsePrefixExpression();
		Expression* parseInfixExpression(Expression* left);
		Expression* parseCallExpression(Expression* left);
		std::vector<Expression*> parseCallArguments();
		void noPrefixParseFnError(TokenType type);
		bool curTokenIs(TokenType type) const;
		bool peekTokenIs(TokenType type) const;
		bool expectPeek(TokenType type);
		void nextToken();
		Order peekPrecedence();
		Order curPrecedence();

		std::map<TokenType, PrefixParseFn> prefixParseFns;
		std::map<TokenType,InfixParseFn> infixParseFns;
		Lexer* lexer;
		SToken curToken;
		SToken peekToken;
		std::vector<std::string> errors;
		std::map<TokenType, Order> precedences;
	};



}






#endif