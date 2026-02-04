#ifndef __TOKEN_H
#define __TOKEN_H

#include "export.h"
#include <string>

namespace Sun {

#define T_ILLEGAL   "illegal"
#define T_EOF       "eof"

#define T_INTEGER   "integer"
#define T_FLOAT     "float"
#define T_DOUBLE    "double"

#define T_IDENT     "identifier"

#define T_ASSIGN    "="
#define T_PLUS      "+"
#define T_MINUS     "-"
#define T_MUL       "*"
#define T_DIV       "/"
#define T_BANG      "!"
#define T_LT        "<"
#define T_GT        ">"
#define T_EQ        "=="
#define T_NOT_EQ    "!="

#define T_COMMA     ","
#define T_SEMICOLON ";"
#define T_LPAREN    "("
#define T_RPAREN    ")"
#define T_LBRACE      "{"
#define T_RBRACE      "}"

	//keywords
#define T_FUNCTION  "fn"
#define T_STRUCT    "struct"
#define T_CLASS     "class"
#define T_ENUM      "enum"
#define T_INT       "int"
#define T_BOOL      "bool"
#define T_UINT32    "uint32_t"
#define T_UINT64    "uint64_t"

#define T_VAR       "var"
#define T_MESH      "mesh"
#define T_TRUE      "true"
#define T_FALSE     "false"
#define T_IF        "if"
#define T_ELSE      "else"
#define T_RETURN    "return"

	using TokenType = const char*;

	//extern SCRIPT_API std::string enum_to_string(TokenType tk);

	inline TokenType lookup(const std::string& ident) {
		if (ident == "int") {
			return T_INT;
		}
		else if (ident == "fn") {
			return T_FUNCTION;
		}
		else if (ident == "true") {
			return T_TRUE;
		}
		else if (ident == "false") {
			return T_FALSE;
		}
		else if (ident == "if") {
			return T_IF;
		}
		else if (ident == "else") {
			return T_ELSE;
		}
		else if (ident == "return") {
			return T_RETURN;
		}
		return T_IDENT;
	}

	struct SToken {
		SToken(TokenType Type) :type(Type) {

		}
		SToken(TokenType Type, const std::string& str) :type(Type), literal(str) {}
		SToken() :type(T_ILLEGAL) {}
		static bool isDef(TokenType type);
		TokenType type;
		std::string literal;
		int rowNo = 0;
	};

}

#endif