// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 SCRIPT_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// SCRIPT_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的

#ifndef __LEXER_H
#define __LEXER_H


#include <string>
#include <vector>
#include "export.h"
#include "Token.h"

namespace Sun {


	class SCRIPT_API Lexer {
	public:
		Lexer();
		Lexer(const std::string& input);
		void clear() {
			input.clear();
            prePosition = -1;
			position = 0;
			readPosition = 0;
			ch = 0;
		}
		void setInput(const std::string& input) {
			this->input = input;
			readChar();
		}
		void clearAndSetInput(const std::string& input) {
			clear();
			setInput(input);
		}
        void skipCurLine();

        //期待之后出现的字符是str,如果是返回true,否则返回false;不改变读取状态
        bool expect(const std::string& str) const;

        //跳转到str第一次出现的位置，并将读取位置移动到str后面第一个字符,如果str不存在，则跳到字符流尾部
        //TODO:利用KMP算法匹配
        void skipToNext(const std::string& str);

        //尝试读一个token, 如果有合法token,改变读取状态并返回该合法token,
        //否则，不改变读取状态并返回一个非法token
		SToken nextToken();

        //读取到类型为type的token为止
        SToken skipToNextToken(TokenType type);

        //提取字符串直到某个符号之前,position移动移动到那个位置之前
        std::string tokenStringBefore(char c);

		static bool isLetter(char ch);
		static bool isDigit(char ch);

        void rollback();

        char readChar();
        char peekChar() const;

        std::string readIdentifier();
        std::string readNumber(TokenType& type);

        void skipWhiteSpace();

        //将tokens的literal按顺序以split为分隔符合并为一个字符串输出
        static std::string tokenLiteralMerge(const std::vector<SToken>& tks, const std::string& split);
	protected:
		std::string input;
        //指向上一次读取的位置 TODO:增加撤销队列
        int prePosition;
		//指向input的位置，当前读取的
		int position;
		//下一个将读取的位置
		int readPosition;
		//当前字符
		char ch;
		//当前的行号
		int curRowNo = 1;
	};
	
}

#endif


