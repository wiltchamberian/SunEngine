// Script.cpp : 定义 DLL 的导出函数。
//

#include "Lexer.h"
#include <vector>
#include <string>
#include <cassert>
#include "Token.h"

namespace Sun {

    Lexer::Lexer()
        :position(0)
        ,prePosition(-1)
        , readPosition(0)
        , ch(0) 
    {

    }

    Lexer::Lexer(const std::string& input)
        :position(0)
        ,prePosition(-1)
        , readPosition(0)
        , ch(0)
    {
        setInput(input);
    }

    void Lexer::skipCurLine() {
        while (true) {
            readChar();
            if (ch != '\n' && ch != 0) {
                ;
            }
            else {
                break;
            }
        }
    }

    bool Lexer::expect(const std::string& str) const {
        auto read = readPosition;
        for (int i = 0; i < str.size(); ++i) {
            if (read < input.size() && input[read]==str[i]) {
                ++read;
                continue;
            }
            else {
                return false;
            }
        }
        return true;
    }
    
    void Lexer::skipToNext(const std::string& str) {
        int k = 0;
        while (k < str.size()) {
            readChar();
            if (ch == 0) {
                return;
            }
            if (ch == str[k]) {
                ++k;
                continue;
            }
            else {
                k = 0;
                rollback();
            }
        }
        return;
    }

    //kernal function
    SToken Lexer::nextToken() {
        SToken token;

        skipWhiteSpace();
        readChar();
        token.rowNo = curRowNo;
        switch (ch) {
            case '=': 
            {
                if (peekChar() == '=') {
                    char tch = ch;
                    readChar();
                    token.type = T_EQ; token.literal = std::string(1,tch) + std::string(1,ch);
                }
                else {
                    token.type = T_ASSIGN; token.literal = ch;
                }            
            }
            break;
            case ';': 
            {
                token.type = T_SEMICOLON; token.literal = ch;
            }
            break;
            case '(':
            {
                token.type = T_LPAREN; token.literal = ch;
            }
            break;
            case ')':
            {
                token.type = T_RPAREN; token.literal = ch;
            }
            break;
            case '{':
            {
                token.type = T_LBRACE; token.literal = ch;
            }
            break;
            case '}':
            {
                token.type = T_RBRACE; token.literal = ch;
            }
            break;
            case ',':
            {
                token.type = T_COMMA; token.literal = ch;
            }
            break;
            case '+':
            {
                token.type = T_PLUS; token.literal = ch;
            }
            break;
            case '-':
            {
                token.type = T_MINUS; token.literal = ch;
            }
            break;
            case '*':
            {
                token.type = T_MUL; token.literal = ch;
            }
            break;
            case '/':
            {
                if (peekChar() == '/') {
                    rollback();
                    return SToken(T_ILLEGAL);
                }
                token.type = T_DIV; token.literal = ch;
            }
            break;
            case '!':
            {
                if (peekChar() == '=') {
                    char tch = ch;
                    readChar();
                    token.type = T_NOT_EQ; 
                    token.literal = std::string(1, tch) + std::string(1, ch);
                }
                else {
                    token.type = T_BANG; token.literal = ch;
                }
                
            }
            break;
            case '<':
            {
                token.type = T_LT; token.literal = ch;
            }
            break;
            case '>':
            {
                token.type = T_GT; token.literal = ch;
            }
            break;
            case 0:
            {
                token.type = T_EOF; token.literal = "";
            }
            break;
            default:
            {
                if (isLetter(ch)) {
                    token.literal = readIdentifier();
                    token.type = lookup(token.literal);
                    return token;
                }
                else if (isDigit(ch)) {
                    token.literal = readNumber(token.type);
                    return token;
                }
                else {
                    token.type = T_ILLEGAL;
                    rollback();
                    return token;
                    //assert(false);
                }
            }
            break;
        }
        //readChar();

        return token;
    }

    std::string Lexer::tokenStringBefore(char c) {
        std::string res;
        while (true) {
            readChar();
            if (ch != c) {
                res.push_back(ch);
                continue;
            }
            else {
                break;
            }
        }
        rollback();
        return res;
    }

    bool Lexer::isLetter(char ch) {
        return 'a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z' || ch == '_';
    }

    bool Lexer::isDigit(char ch) {
        return '0' <= ch && ch <= '9';
    }

    std::string Lexer::tokenLiteralMerge(const std::vector<SToken>& tks, const std::string& split) {
        std::string res;
        for (int i = 0; i < tks.size()-1; ++i) {
            res.append(tks[i].literal);
            res.append(split);
        }
        if (tks.size() > 0) {
            res.append(split);
        }
        return res;
    }

    std::string Lexer::readIdentifier() {
        int pos = position;
        while (isLetter(ch)) {
            readChar();
        }
        return std::string(input.begin() + pos, input.begin() + position);
    }

    std::string Lexer::readNumber(TokenType& type) {
        int pos = position;
        type = T_INTEGER;
        while (isDigit(ch)) {
            readChar();
        }
        //增加"."判定以支持浮点数
        if (ch == '.') {
            type = T_FLOAT;
            readChar();
            while (isDigit(ch)) {
                readChar();
            }
        }

        return std::string(input.begin() + pos, input.begin() + position);
    }

    char Lexer::readChar() {
        if (readPosition >= input.size()) {
            ch = 0;
        }
        else {
            ch = input[readPosition];
        }
        prePosition = position;
        position = readPosition;  //TODO
        readPosition++;
        return ch;
    }

    char Lexer::peekChar() const {
        if (readPosition >= input.size()) {
            return 0;
        }
        else {
            return input[readPosition];
        }
        return 0;
    }

    void Lexer::rollback() {
        assert(prePosition != -1);
        readPosition = position;
        position = prePosition;
        prePosition = -1;
    }

    void Lexer::skipWhiteSpace() {
        readChar();
        while(ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
            if (ch == '\n') {
                curRowNo += 1;
            }
            readChar();
        }
        rollback();
    }

    

}

