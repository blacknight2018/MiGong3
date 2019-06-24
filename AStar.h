#pragma once
#include "MiGong3.h"
struct myPoint
{
	int m_x, m_y;
	myPoint(int x, int y) :m_x(x), m_y(y) {}
	myPoint()
	{
		m_x = m_y = -1;
	}
	bool operator==(myPoint& p)
	{
		return (this->m_x == p.m_x) && (this->m_y == p.m_y);
	}
	bool operator<(const myPoint& c)const
	{
		if (m_x != c.m_x)
			return m_x < c.m_x;
		if (m_y != c.m_y)
			return m_y < c.m_y;
		return false;
	}



};
struct Box
{
	myPoint zb;
	int F, G, H;
	myPoint parent;
	Box(myPoint t)
	{

		F = G = H = 0;
		zb = t;

	}
	Box(myPoint tz, myPoint parent)
	{
		F = G = H = 0;
		this->zb = tz;
		this->parent = parent;
	}
	Box()
	{
		F = G = H = 0;

	}
	bool operator==(Box& x)
	{
		return x.zb == this->zb;
	}
};

//A*寻路
class AStarFind
{
private:
	map<myPoint, int> g_Flag;
	map<myPoint, myPoint> g_ParentmyPoint; //记录某个坐标的父坐标
private:
	//int* m_pMap;
	int m_width;
	int m_height;
	int m_startx, m_starty;
	int m_endx, m_endy;
	vector<vector<int>> m_vecMap;
public:

	void SetMap(int* mapmyPoint, int mapWidth, int mapHeight);	 //设置要寻路的地图
	void SetmyPoint(int startX, int startY, int endX, int endY);  //设置起点终点坐标
	vector<vector<int>> StartFind();
private:
	vector<vector<int>> getResult(myPoint End);				 //获取结果
public:

	bool boxCanArrive(myPoint detmyPoint);
	vector<Box> findBoxsCanArrive(myPoint nowmyPoint);
	Box findFleast(vector<Box>& b);
	bool removeBox(vector<Box>& list, Box bx);
	bool findBox(vector<Box>& list, Box bx);
	int  CalcmyPointH(myPoint& a, myPoint& b);
	bool isEnd(vector<Box> all, myPoint End);

};