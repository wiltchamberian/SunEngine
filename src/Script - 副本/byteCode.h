/*****************************************************************************
* @brief : byteCode
* 将AST转为自定义的byteCode,这种byteCode结构类似汇编模式
* @author : acedtang
* @date : 2021/7/10
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __BYTE_CODE_H
#define __BYTE_CODE_H

#include "Ast.h"

namespace Sun {

	//加法指令
	enum OpCode {
		ADD,   //4字节整数 + 法
		ADD_FLOAT,      //4字节浮点数 + 法
		ADD_DOUBLE,      //4字节双精度+法
		SUB,  //4字节整数 - 法
		MUL,   //4字节整数乘法
		MUL_FLOAT ,  //4字节浮点数乘法
	};

	//字节码栈
	struct Stack {
		void pushInteger(int num);
		void pushFloat(float num);
		void pushDouble(double num);
		void pushOpCode(int op);
		void pop();
		//int esp; //栈顶指针 buffer的长度作为栈顶
		int ebp = 0; //栈底指针
		int eip; //调用指针,指向opCode的地址(索引)
		std::vector<int> buffer;
	};

	extern Stack astToByteCode(Program* root);

}



#endif