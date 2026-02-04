#include "RandomizedPrim.h"
#include <cassert>
#include <random>
#include <chrono>
#include <iostream>

namespace Sun {


	void UnionFind::init(const std::vector<int>& data) {
		datas = data;
	}

	void UnionFind::clear() {
		for (int i = 0; i < datas.size(); ++i) {
			datas[i] = i;
		}
	}

	//并查集
	int UnionFind::parent(int i){
		if (datas[i] == i) {
			return i;
		}
		datas[i] = parent(datas[i]);
		return datas[i];
	}

	void UnionFind::merge(int a, int b) {
		int k = getParent(b);
		datas[k] = a;
		parent(b);
	}

	int UnionFind::getParent(int i) {
		if (datas[i] == i) {
			return i;
		}
		return getParent(datas[i]);
	}

	void Maze::init(int M, int N) {
		assert(M > 0 && N >0 );
		
		clear();
		m = M;
		n = N;

		unionFind.datas.resize(m * n);
		unionFind.clear();

		//初始所有墙壁都存在
		horizontals.resize((m - 1) * n,1);
		verticals.resize(m * (n - 1),1);
	}

	//设置迷宫起始点 (x为行号，y为列号,从0开始)
	void Maze::setStartPoint(int x, int y) {
		start_x = x;
		start_y = y;
	}

	//设置迷宫终止点
	void Maze::setEndPoint(int x, int y) {
		end_x = x;
		end_y = y;
	}

	void Maze::setGridSize(vec2 siz) {
		grid_siz = siz;
	}

	void Maze::clear() {
		
		

	}

	std::pair<int, int> Maze::calUnitNumber(int sn) {
		int h = (m - 1) * n;
		int v = m * (n - 1);
		int total = h + v; //总墙壁数
		std::pair<int, int> res;
		if (sn < h) {
			res.first = sn;
			res.second = sn + n;
		}
		else {
			sn = sn - h;
			int columnNo = sn / m;
			int rowNo = sn - columnNo * m;
			res.first = rowNo * n + columnNo;
			res.second = res.first + 1;
		}
		return res;
	}

	void Maze::randomPrim() {
		assert(m > 0 && n > 0);
		srand(std::chrono::high_resolution_clock::now().time_since_epoch().count());
		int h = (m - 1) * n;
		int v = m * (n - 1);
		int total = h + v;//总墙壁数

		int start = start_x * n + start_y;
		int end = end_x * n + end_y;
		while (true) {
			//随机
			int r = (float)rand() / RAND_MAX * total;
			std::pair<int, int> pr = calUnitNumber(r);
			//标记该墙壁是水平还是垂直
			bool isHorizontal = r < h;

			int p1 = unionFind.parent(pr.first);
			int p2 = unionFind.parent(pr.second);
			if (p1 != p2) {
				unionFind.merge(pr.first, pr.second);
				//删除墙壁
				if (isHorizontal) {
					horizontals[r] = 0;
				}
				else {
					int t = r - h;
					if (t >= verticals.size()) t = verticals.size() - 1;
					verticals[t] = 0;
				}

				if (unionFind.parent(start) == unionFind.parent(end)) {
					break;
				}
			}
			else {
				continue;
			}
		}

	}

	void Maze::print() {


	}

	//根据水平墙壁索引，计算水平墙壁左端点的坐标
	vec2 Maze::horizontalCoordinate(int sn) const {
		int row = sn / n;
		int col = sn - row * n;

		vec2 v;
		v.y = grid_siz.y * (row + 1);
		v.x = grid_siz.x * col;

		return v;
	}

	//根据垂直墙壁索引，计算垂直墙壁上端点的坐标
	vec2 Maze::verticalCoordinate(int sn) const {
		int col = sn / m;
		int row = sn - col * m;

		vec2 v;
		v.x = grid_siz.x * (col + 1);
		v.y = grid_siz.y * row;

		return v;
	}
}