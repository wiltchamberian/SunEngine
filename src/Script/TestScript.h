#pragma once

#include "Lexer.h"
#include "Parser.h"
#include "ObjectSys.h"
#include <string>
#include <variant>

namespace Sun {

	using Variant = std::variant<std::int64_t,int, bool, void*, std::string>;

	extern SCRIPT_API void start_clever();

	extern SCRIPT_API void runAllTestCases();

	extern SCRIPT_API void testLexer(void);

	extern SCRIPT_API Program* prepareProgram(const std::string& str);

	extern SCRIPT_API void testDefStatements();
	bool testDefStatement(Statement* st,const std::string& name);

	extern SCRIPT_API void testReturnStatements();

	extern SCRIPT_API void testIdentifierExpression();

	extern SCRIPT_API void testBooleanExpression();

	extern SCRIPT_API void testIfExpression();

	extern SCRIPT_API void testIfElseExpression();

	extern SCRIPT_API void testIntegerExpression();

	extern SCRIPT_API void testParsingPrefixExpression();
	
	extern SCRIPT_API void testParsingInfixExpressions();

	extern SCRIPT_API void testOperatorPrecedenceParsing();
	
	extern SCRIPT_API void testFunctionLiteralParsing();

	extern SCRIPT_API void testFunctionParameterParsing();

	extern SCRIPT_API void testCallExpressionParsing();

	extern SCRIPT_API bool testIntegerLiteral(Expression* il, std::int64_t value);
	extern SCRIPT_API bool testBooleanLiteral(Expression* exp, bool bl);
	extern SCRIPT_API bool testIdentify(Expression* exp, const std::string& value);
	bool testLiteralExpression(Expression* exp, const Variant& var);
	bool testInfixExpression(Expression* exp, const Variant& left, const char* op, const Variant& right);

	//eval
	extern SCRIPT_API void testEvalIntegerExpression();
	std::shared_ptr<Object_t> testEval(const std::string& input);
	bool testIntegerObject(Object_t* obj, std::int64_t expected);

	extern SCRIPT_API void testEvalBooleanExpression();
	bool testBooleanObject(Object_t* obj, bool expected);

	extern SCRIPT_API void testBangOperator();

	extern SCRIPT_API void testEvalIfElseExpressions();
	bool testNullObject(Object_t* obj);

	extern SCRIPT_API void testEvalReturnStatements();

	extern SCRIPT_API void testErrorHandling();

	extern SCRIPT_API void testEvalDefStatements();

	extern SCRIPT_API void testEvalFunctionObject();
	extern SCRIPT_API void testFunctionApplication();
	extern SCRIPT_API void testClosures();

	extern SCRIPT_API void testMyMain();
}