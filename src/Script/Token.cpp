#include "Token.h"

namespace Sun {
#if 0
    SCRIPT_API std::string enum_to_string(TokenType tk) {
        if (tk == T_ILLEGAL) {
            return ENUM_TO_STRING(ILLEGAL);
        }
        else if (tk == T_TEOF) {
            return ENUM_TO_STRING(TEOF);
        }
        else if (tk == T_INTEGER) {
            return ENUM_TO_STRING(INTEGER);
        }
        else if (tk == T_FLOAT) {
            return ENUM_TO_STRING(FLOAT);
        }
        else if (tk == T_DOUBLE) {
            return ENUM_TO_STRING(DOUBLE);
        }
        else if (tk == T_IDENT) {
            return ENUM_TO_STRING(IDENT);
        }
        else if (tk == T_ASSIGN) {
            return ENUM_TO_STRING(ASSIGN);
        }
        else if (tk == T_PLUS) {
            return ENUM_TO_STRING(PLUS);
        }
        else if (tk == T_MINUS) {
            return ENUM_TO_STRING(MINUS);
        }
        else if (tk == T_MUL) {
            return ENUM_TO_STRING(MUL);
        }
        else if (tk == T_DIV) {
            return ENUM_TO_STRING(DIV);
        }
        else if (tk == T_BANG) {
            return ENUM_TO_STRING(BANG);
        }
        else if (tk == T_LT) {
            return ENUM_TO_STRING(LT);
        }
        else if (tk == T_GT) {
            return ENUM_TO_STRING(GT);
        }
        else if (tk == T_COMMA) {
            return ENUM_TO_STRING(COMMA);
        }
        else if (tk == T_SEMICOLON) {
            return ENUM_TO_STRING(SEMICOLON);
        }
        else if (tk == T_LPAREN) {
            return ENUM_TO_STRING(LPAREN);
        }
        else if (tk == T_RPAREN) {
            return ENUM_TO_STRING(RPAREN);
        }
        else if (tk == T_LBRACE) {
            return ENUM_TO_STRING(LBRACE);
        }
        else if (tk == T_RBRACE) {
            return ENUM_TO_STRING(RBRACE);
        }
        else if (tk == T_FUNCTION) {
            return ENUM_TO_STRING(FUNCTION);
        }
        else if (tk == T_LET) {
            return ENUM_TO_STRING(LET);
        }
        else if (tk == T_VAR) {
            return ENUM_TO_STRING(VAR);
        }
        else if (tk == T_MESH) {
            return ENUM_TO_STRING(MESH);
        }
        else if (tk == T_TRUE) {
            return ENUM_TO_STRING(TRUE);
        }
        else if (tk == T_FALSE) {
            return ENUM_TO_STRING(FALSE);
        }
        else if (tk == T_ELSE) {
            return ENUM_TO_STRING(ELSE);
        }
        else if (tk == T_RETURN) {
            return ENUM_TO_STRING(RETURN);
        }
        else if (tk == T_EQ) {
            return ENUM_TO_STRING(EQ);
        }
        else if (tk == T_NOT_EQ) {
            return ENUM_TO_STRING(NOT_EQ);
        }
        return "";
    }
#endif

    bool SToken::isDef(TokenType type) {
        return type == T_INT;
    }
}