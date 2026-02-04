//尝试用最少的代码实现一个解释器

#pragma once

#include <string>
#include <vector>

namespace Sun {

	inline int plus(int a, int b) {
		return a + b;
	}

	inline int minus(int a, int b) {
		return a - b;
	}

	inline int Main() {
		int x = 3; // mov dword ptr [x],3
		int y = x; // eax, dword ptr [x]   ; dword ptr [y],eax

		int k = plus(x, y); //mov edx,dword ptr [x] ; mov ecx,dword ptr[x]; call Sun::plus ;mov dword ptr [k],eax

		if (k < 0) //cmp dword ptr[k],0 jge Sun::Main
		{
			return minus(x, y); //mov edx,dword ptr [y] ;mov ecx,dword ptr [x]; call Sun::minus
			//jup Sun::Main+7Fh
		}
		else {
			;
		}
		
		x = (x + y) * 3 + (x * plus(x, y));//

		//for (int i = 0; i < 3; ++i) {
		//	x += plus(x,y);
		//}

		return x;
	}
	/*
	
	
	
	*/

	class Compiler {
	public:
		
	};
}