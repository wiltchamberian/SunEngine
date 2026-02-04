#include "Parser.h"
#include <iostream>
#include <cassert>
#include "Lexer.h"
#include "Token.h"

namespace Sun {

	Parser::Parser(Lexer* lex) :lexer(lex) {
		// Read two tokens, so curToken and peekToken are both set
		nextToken();
		nextToken();

		registerPrefix(T_IDENT, std::bind(&Parser::parseIdentifier, this));
		registerPrefix(T_INTEGER, std::bind(&Parser::parseIntegerLiteral, this));
		registerPrefix(T_BANG, std::bind(&Parser::parsePrefixExpression, this));
		registerPrefix(T_MINUS, std::bind(&Parser::parsePrefixExpression, this));
		registerPrefix(T_TRUE, std::bind(&Parser::parseBoolean, this));
		registerPrefix(T_FALSE, std::bind(&Parser::parseBoolean, this));
		registerPrefix(T_LPAREN, std::bind(&Parser::parseGroupedExpression, this));
		//registerPrefix(T_RPAREN, std::bind(&Parser::parseGroupedExpression, this));
		registerPrefix(T_IF, std::bind(&Parser::parseIfExpression, this));
		registerPrefix(T_FUNCTION, std::bind(&Parser::parseFunctionLiteral, this));
		registerPrefix(T_STRUCT, std::bind(&Parser::parseStruct, this));

		registerInfix(T_PLUS, std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1));
		registerInfix(T_MINUS, std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1));
		registerInfix(T_DIV, std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1));
		registerInfix(T_MUL, std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1));
		registerInfix(T_EQ, std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1));
		registerInfix(T_NOT_EQ, std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1));
		registerInfix(T_LT, std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1));
		registerInfix(T_GT, std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1));
		registerInfix(T_LPAREN, std::bind(&Parser::parseCallExpression, this, std::placeholders::_1));
		//acedtang
		registerInfix(T_ASSIGN, std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1));

		precedences = { 
		{ T_EQ, Order::EQUALS },
		{ T_NOT_EQ ,Order::EQUALS },
		{ T_LT, Order::LESS_GREATER },
		{ T_GT, Order::LESS_GREATER },
		{ T_PLUS, Order::SUM },
		{ T_MINUS, Order::SUM },
		{ T_DIV, Order::PRODUCT },
		{ T_MUL, Order::PRODUCT },
		{ T_LPAREN, Order::CALL },
		//acedtang add
		{ T_ASSIGN, Order::ASSIGN},
		};
	}

	void Parser::nextToken() {
		curToken = peekToken;
		peekToken = lexer->nextToken();
	}

	Order Parser::peekPrecedence() {
		if (precedences.find(peekToken.type) != precedences.end()) {
			return precedences[peekToken.type];
		}
		return LOWEST;
	}

	Order Parser::curPrecedence() {
		if (precedences.find(curToken.type) != precedences.end()) {
			return precedences[curToken.type];
		}
		return LOWEST;
	}

	void Parser::clearAndSetInput(const std::string& input){
		assert(lexer != nullptr);

		lexer->clearAndSetInput(input);
		errors.clear();

		nextToken();
		nextToken();
	}

	Program* Parser::parseProgram() {
		Program* pro = new Program();
		
		while(curToken.type != T_EOF){
			Statement* stmt = parseStatement();
			if (stmt != nullptr) {
				pro->statements.push_back(stmt);
			}
			nextToken();
		}

		return pro;
	}

	std::vector<std::string> Parser::outputErrors() const{
		return errors;
	}

	void Parser::registerPrefix(TokenType tkType, PrefixParseFn fn) {
		prefixParseFns[tkType] = fn;
	}

	void Parser::registerInfix(TokenType tkType, InfixParseFn fn) {
		infixParseFns[tkType] = fn;
	}

	void Parser::peekError(TokenType type) {
		char buf[256];
		sprintf(buf, "expected next token to be %s, got %s instead",
			type, peekToken.type);
		errors.push_back(std::string(buf));
	}

	Statement* Parser::parseStatement() {
		if (SToken::isDef(curToken.type)) {
			return parseDefStatement();
		}
		else if (curToken.type == T_RETURN) {
			return parseReturnStatement();
		}
		return parseExpressionStatement();
	}

	DefStatement* Parser::parseDefStatement() {
		DefStatement* stmt = new DefStatement(curToken);
		if (!expectPeek(T_IDENT)) {
			return nullptr;
		}
		stmt->ident = new Identifier(curToken, curToken.literal);

		if (!expectPeek(T_ASSIGN)) {
			return nullptr;
		}
#if 0
		// TODO: We're skipping the expressions until we encounter a semicolon
		//一直到是";"的token
		while(!curTokenIs(T_SEMICOLON)) {
			nextToken();
		}
		return stmt;
#endif 
		nextToken();
		stmt->express = parseExpression(LOWEST);
		if (peekTokenIs(T_SEMICOLON)) {
			nextToken();
		}
		return stmt;
	}

	ReturnStatement* Parser::parseReturnStatement() {
		ReturnStatement* stmt = new ReturnStatement(curToken);
		nextToken();
#if 0
		// TODO: We're skipping the expressions until we encounter a semicolon
		while (!curTokenIs(T_SEMICOLON)) {
			nextToken();
		}
#endif 

		stmt->returnValue = parseExpression(LOWEST);
		if (peekTokenIs(T_SEMICOLON)) {
			nextToken();
		}
		return stmt;
	}

	ExpressionStatement* Parser::parseExpressionStatement() {
		ExpressionStatement* es = new ExpressionStatement(curToken);
		es->expression = parseExpression(LOWEST);
		if(peekTokenIs(T_SEMICOLON)) {
			nextToken();
		}
		return es;
	}

	BlockStatement* Parser::parseBlockStatement() {
		BlockStatement* block = new BlockStatement(curToken);
		nextToken();
		while (!curTokenIs(T_RBRACE) && !curTokenIs(T_EOF)) {
			Statement* stmt = parseStatement();
			if (stmt != nullptr) {
				block->statements.push_back(stmt);
			}
			nextToken();
		}
		
		return block;
	}

	std::vector<Identifier*> Parser::parseFunctionParameters() {
		std::vector<Identifier*> identifiers;
		if (peekTokenIs(T_RPAREN)) {
			nextToken();
			return identifiers;
		}
		nextToken();

		Identifier* ident = new Identifier(curToken, curToken.literal);
		identifiers.push_back(ident);

		while (peekTokenIs(T_COMMA)) {
			nextToken();
			nextToken();
			Identifier* ident = new Identifier(curToken, curToken.literal);
			identifiers.push_back(ident);
		}

		if (!expectPeek(T_RPAREN)) {
			return {};
		}

		return identifiers;
	}

	Expression* Parser::parseExpression(Order precedence) {
		auto iter = prefixParseFns.find(curToken.type);
		if (iter == prefixParseFns.end()) {
			noPrefixParseFnError(curToken.type);
			return nullptr;
		}
		PrefixParseFn prefix = iter->second;

		Expression*  leftExp = prefix();

		while (!peekTokenIs(T_SEMICOLON) && precedence < peekPrecedence()) {
			InfixParseFn infix = infixParseFns[peekToken.type];
			if (infix == nullptr){
				return leftExp;
			}
			nextToken();
			leftExp = infix(leftExp);
		}

		return leftExp;
	}

	Expression* Parser::parseIdentifier() {
		return new Identifier(curToken, curToken.literal);
	}

	Expression* Parser::parseBoolean() {
		return new BooleanExp(curToken, curTokenIs(T_TRUE));
	}

	Expression* Parser::parseGroupedExpression() {
		nextToken();
		Expression* exp = parseExpression(LOWEST);
		if (!expectPeek(T_RPAREN)) {
			return nullptr;
		}
		return exp;
	}

	Expression* Parser::parseIfExpression() {
		IfExpression* expression = new IfExpression(curToken);
		if (!expectPeek(T_LPAREN)) {
			return nullptr;
		}
		nextToken();
		expression->condition = parseExpression(LOWEST);
		if (!expectPeek(T_RPAREN)) {
			return nullptr;
		}
		if (!expectPeek(T_LBRACE)) {
			return nullptr;
		}
		expression->consequence = parseBlockStatement();

		if (peekTokenIs(T_ELSE)) {
			nextToken();
			if (!expectPeek(T_LBRACE)) {
				return nullptr;
			}
			expression->alternative = parseBlockStatement();
		}

		return expression;
	}

	Expression* Parser::parseFunctionLiteral() {
		FunctionLiteral* lit = new FunctionLiteral(curToken);
		if (!expectPeek(T_LPAREN)) {
			return nullptr;
		}
		lit->parameters = parseFunctionParameters();
		if (!expectPeek(T_LBRACE)) {
			return nullptr;
		}
		lit->body = parseBlockStatement();
		return lit;
	}

	Expression* Parser::parseStruct() {
		StructLiteral* stru = new StructLiteral(curToken);
		if (!expectPeek(T_IDENT)) {
			return nullptr;
		}
		if (!expectPeek(T_LBRACE)) {
			return nullptr;
		}

		BlockStatement* stmt = parseBlockStatement();
		//stru->blockStatement = stmt;

		return stru;
	}

	Expression* Parser::parseIntegerLiteral() {
		IntegerLiteral* lit = new IntegerLiteral(curToken);
		//TODO 检查curToken.literal 是否是 整数
		std::int64_t value = 0;
		try {
			value = std::stoll(curToken.literal);
		}
		catch (...) {
			char buf[255];
			sprintf(buf, "could not parse %s as integer", curToken.literal.c_str());
			errors.push_back(std::string(buf));
			return nullptr;
		}

		lit->value = value;
		return lit;
	}

	Expression* Parser::parsePrefixExpression() {
		PrefixExpression* expression = new PrefixExpression{ curToken,curToken.type };
		nextToken();
		expression->right = parseExpression(PREFIX);
		return expression;
	}

	Expression* Parser::parseInfixExpression(Expression* left) {
		InfixExpression* exp = new InfixExpression(curToken, curToken.type);
		exp->left = left;
		Order precedence = curPrecedence();
		nextToken();
		exp->right = parseExpression(precedence);
		return exp;
	}

	Expression* Parser::parseCallExpression(Expression* func) {
		CallExpression* exp = new CallExpression(curToken);
		exp->function = func;
		exp->arguments = parseCallArguments();
		return exp;
	}

	std::vector<Expression*> Parser::parseCallArguments() {
		std::vector<Expression*> args;
		if (peekTokenIs(T_RPAREN)) {
			nextToken();
			return args;
		}
		nextToken();
		args.push_back(parseExpression(LOWEST));

		while (peekTokenIs(T_COMMA)) {
			nextToken();
			nextToken();
			args.push_back(parseExpression(LOWEST));
		}

		if (!expectPeek(T_RPAREN)) {
			return {};
		}

		return args;
	}

	void Parser::noPrefixParseFnError(TokenType type) {
		char buf[255];
		sprintf(buf,"no prefix parse function for %s found",type);
		errors.push_back(buf);
	}

	bool Parser::curTokenIs(TokenType type) const {
		return curToken.type == type;
	}

	bool Parser::peekTokenIs(TokenType type) const {
		return peekToken.type == type;
	}

	bool Parser::expectPeek(TokenType type){
		if (peekTokenIs(type)) {
			nextToken();
			return true;
		}
		else {
			peekError(type);
			return false;
		}
	}
}