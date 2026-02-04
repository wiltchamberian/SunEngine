// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 SCRIPT_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// SCRIPT_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的

#ifndef __LEXER_H
#define __LEXER_H


#include <string>
#include "export.h"
#include "Token.h"

namespace Sun {


	class SCRIPT_API Lexer {
	public:
		Lexer();
		Lexer(const std::string& input);
		void clear() {
			input.clear();
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
		SToken nextToken();
		static bool isLetter(char ch);
		static bool isDigit(char ch);
	protected:
		std::string readIdentifier();
		std::string readNumber(TokenType& type);
		void readChar();
		char peekChar() const;
		
		void skipWhiteSpace();
		std::string input;
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


