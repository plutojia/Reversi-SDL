#include "stdafx.h"
#include<iostream>
#include<stdio.h>
#include<vector>
#include<set>

#define xm 8
#define ym 8
using namespace std;
//class

class qp {
private:int q[xm][ym];
		int difficulty = 5;
		int flag = 1;
		int passsum = 0;
		char *message;
		int scorearray[xm][ym] =
		{ { 32,0,16, 8,8,16,0,32 },
		{ 0,0,8,1,1,8, 0,0 },
		{ 16, 8,4, 2,2, 4,8,16 },
		{ 8,1,2,0,0,2,1,8 },
		{ 8,1,2,0,0,2,1,8 },
		{ 16,8,4,2,2,4,8,16 },
		{ 1,1, 8,1,1, 8, 1,1 },
		{ 32,1,16,8,8,16, 1,32 }
		};

		struct point {
			int hash;
			int x;
			int y;
			int score = 0;
			int flag;
			point(int x, int y) {
				if (x<0)x = 0;
				if (x >= xm) x = xm - 1;
				if (y<0)y = 0;
				if (y >= ym) y = ym - 1;
				hash = x*xm + y;
				this->x = x;
				this->y = y;
				score = 0;
				flag = 0;
			}
			bool operator<(point p)const {
				return this->hash<p.hash;
			}
		};
		struct board {
			int q[xm][ym];
			int step = 0, xsum, osum, flag;
			set<point>available;
			board() { step = 0; xsum = 0; osum = 0; flag = 0; }
			board(int temp[xm][ym], set<point>available, int flag) {
				xsum = 0; osum = 0; this->flag = flag;
				for (int i = 0; i<xm; i++) {
					for (int j = 0; j<ym; j++)
					{
						q[i][j] = temp[i][j];
						if (temp[i][j] == 1)
						{
							xsum += 1; step++;
						}
						else if (temp[i][j] == -1) { osum += 1; step++; }
					}
				}
				this->available = available;
			}
		};
		vector<board>boardlink;
		set<point> available;
		bool cal(int x, int y);

public:
	qp() {
		for (int i = 0; i<xm; i++)
			for (int j = 0; j<ym; j++)
				q[i][j] = 0;
		q[xm / 2 - 1][ym / 2 - 1] = 1;
		q[xm / 2][ym / 2 - 1] = -1;
		q[xm / 2 - 1][ym / 2] = -1;
		q[xm / 2][ym / 2] = 1;
		xsum = 2; osum = 2;
		message = "";
		available.insert(point(xm / 2 - 2, ym / 2 - 2));
		available.insert(point(xm / 2 - 2, ym / 2 - 1));
		available.insert(point(xm / 2 - 2, ym / 2));
		available.insert(point(xm / 2 - 2, ym / 2 + 1));
		available.insert(point(xm / 2 - 1, ym / 2 - 2));
		available.insert(point(xm / 2 - 1, ym / 2 + 1));
		available.insert(point(xm / 2, ym / 2 - 2));
		available.insert(point(xm / 2, ym / 2 + 1));
		available.insert(point(xm / 2 + 1, ym / 2 - 2));
		available.insert(point(xm / 2 + 1, ym / 2 - 1));
		available.insert(point(xm / 2 + 1, ym / 2));
		available.insert(point(xm / 2 + 1, ym / 2 + 1));
		flag = 1;
		struct board temp(q, available, flag);
		boardlink.push_back(temp);
	}
	int step = 0;
	int xsum, osum;
	int input(int x, int y);
	point getnextstep(board b, int count = 1);
	int back(int b = 2);
	void print();
	void testprint();
	int pvc(int x, int y);
	int cvc();
	int get(int x, int y);
	int getnextboard(board &b, point p);
	int shengfu();
	void pass();
	int setdifficulty(int d = 1);
	void getarray(int a[8][8]);
};