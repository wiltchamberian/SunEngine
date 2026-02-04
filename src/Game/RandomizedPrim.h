/*
* 随机prim迷宫生成算法，迷宫用一个m行,n列的矩阵来表示
*/

#ifndef __RANDOMIZED_PRIM_H
#define __RANDOMIZED_PRIM_H

#include <vector>
#include "Math/Vector2D.h"

namespace Sun {

	//并查集
	class UnionFind {
	public:
		void init(const std::vector<int>& data);
		void clear();
		
		int parent(int i);

		void merge(int a, int b);
		std::vector<int> datas;
	protected:
		int getParent(int i);

	};

	/***
	迷宫从上往下看是一个2维矩形grid,
	坐标系为x-z,x指向右侧，z指向下侧
	缺省状态下迷宫矩形的左上角位于原点处，矩形的周边不计为墙壁，行数为m,列数为n个grid组成
	***/
	class Maze {
	public:
		Maze() { grid_siz = vec2(1, 1); }
		void init(int m, int n);
		//设置迷宫起始点
		void setStartPoint(int x, int y);
		//设置迷宫终止点
		void setEndPoint(int x, int y);
		void setGridSize(vec2 siz);
		void clear();
		void randomPrim();

		void print();

		//根据水平墙壁索引，计算水平墙壁左端点的坐标
		vec2 horizontalCoordinate(int sn) const;

		//根据垂直墙壁索引，计算垂直墙壁下端点的坐标
		vec2 verticalCoordinate(int sn) const;
	protected:
		//根据墙壁序号计算两边的单元格序号
		inline std::pair<int, int> calUnitNumber(int sn);

		int m = 1;
		int n = 1;

		int start_x = 0;
		int start_y = 0;
		int end_x = 0;
		int end_y = 0;
		vec2 grid_siz;

		UnionFind unionFind;

	public:
		//所有横向的墙壁（不包含边界)长度为(m-1)*n,按照行主序排列
		std::vector<int> horizontals; //(m-1)*n
		//所有纵向的墙壁（不包含边界)长度为m*(n-1),按照列主序排列
		std::vector<int> verticals; //m*(n-1)
	};


}



#endif