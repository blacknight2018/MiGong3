#include "AStar.h"
//A*寻路类的定义
void AStarFind::SetMap(int* mapmyPoint, int mapWidth, int mapHeight)
{

	int temp[50][50] = { 0 };
	memcpy(temp, mapmyPoint, 50 * 50 * sizeof(int));

	//先判断指针合法性
	if (mapmyPoint > 0)
	{
		int i = 0, j = 0;
		m_width = mapWidth;
		m_height = mapHeight;
		for (i = 0; i < mapHeight; i++)
		{
			m_vecMap.push_back(vector<int>(mapWidth, 0));
		}
		for (i = 0; i < mapHeight; i++)
		{
			for (j = 0; j < mapWidth; j++)
			{

				//m_vecMap[i][j] = mapmyPoint[i * mapWidth + j];
				m_vecMap[i][j] = temp[i][j];
			}
			
		}

		
	}
}


bool AStarFind::boxCanArrive(myPoint detmyPoint)
{
	int x = detmyPoint.m_x;
	int y = detmyPoint.m_y;
	if (x < 0 || x>m_height - 1 || y<0 || y>m_width - 1)
		return false;
	if (m_vecMap[detmyPoint.m_x][detmyPoint.m_y] == 1)
		return false;
	return true;
}



//找出当前点可以到达的四周围的点
vector<Box> AStarFind::findBoxsCanArrive(myPoint nowmyPoint)
{
	vector<Box> ret;
	int x = nowmyPoint.m_x;
	int y = nowmyPoint.m_y;

	//上
	int nx = x - 1;
	int ny = y;
	if (true == boxCanArrive(myPoint(nx, ny)))
	{
		if (g_Flag[myPoint(nx, ny)] == 0)
			ret.push_back(Box(myPoint(nx, ny)));
	}
	//下
	nx = x + 1;
	ny = y;
	if (true == boxCanArrive(myPoint(nx, ny)))
	{
		if (g_Flag[myPoint(nx, ny)] == 0)
			ret.push_back(Box(myPoint(nx, ny)));
	}
	//左
	nx = x;
	ny = y - 1;
	if (true == boxCanArrive(myPoint(nx, ny)))
	{
		if (g_Flag[myPoint(nx, ny)] == 0)
			ret.push_back(Box(myPoint(nx, ny)));
	}
	//右
	nx = x;
	ny = y + 1;
	if (true == boxCanArrive(myPoint(nx, ny)))
	{
		if (g_Flag[myPoint(nx, ny)] == 0)
			ret.push_back(Box(myPoint(nx, ny)));
	}

	return ret;
}

//找出F值最小的方格
Box AStarFind::findFleast(vector<Box>& b)
{
	int Minval = 9999999;
	Box MinBox;
	for (auto c : b)
	{
		if (c.F < Minval)
		{
			Minval = c.F;
			MinBox = c;
		}
	}
	return MinBox;
}

//将box移除List
bool AStarFind::removeBox(vector<Box>& list, Box bx)
{
	auto st = list.begin();
	auto end = list.end();
	for (; st != end; st++)
	{
		if (*st == bx)
		{
			list.erase(st);
			return true;
		}
	}
	return false;
}
//列表中寻找是否存在某个Box
bool AStarFind::findBox(vector<Box>& list, Box bx)
{
	for (auto c : list)
	{
		if (c == bx)
			return true;
	}
	return false;
}

//计算曼哈顿距离
int AStarFind::CalcmyPointH(myPoint& a, myPoint& b)
{
	return abs(a.m_x - b.m_x) + abs(a.m_y - b.m_y);
}

bool AStarFind::isEnd(vector<Box> all, myPoint End)
{
	for (auto c : all)
	{
		if (c.zb == End)
			return true;
	}
	return false;
}


vector<vector<int>> AStarFind::getResult(myPoint End)
{
	vector<vector<int>> ret;
	myPoint last = End;
	while (true)
	{

		//已经没有坐标了 跳出循环
		if (last.m_x == -1 || last.m_y == -1)
		{
			break;
		}
		//加入到返回列表
		vector<int> vec = { last.m_x ,last.m_y };
		ret.push_back(vec);

		//查询父结点
		myPoint tmp = g_ParentmyPoint[last];
		last = tmp;

	}
	return ret;
}


void AStarFind::SetmyPoint(int startX, int startY, int endX, int endY)
{
	this->m_startx = startX;
	this->m_starty = startY;
	this->m_endx = endX;
	this->m_endy = endY;
}

vector<vector<int>> AStarFind::StartFind()
{
	vector<vector<int>> ret;

	myPoint startmyPoint(this->m_startx, this->m_starty);
	myPoint endmyPoint(this->m_endx, this->m_endy);
	Box startBox(startmyPoint);
	Box endBox(endmyPoint);

	vector<Box> OpenList, CloseList;
	int sumStep = 0;  //记录起点走到当前格子的距离
	OpenList.push_back(startBox);

	g_Flag[startmyPoint] = 1;

	//记录循环次数 超过一定次数认为死循环
	int cLoop = 0;
	while (true)
	{
		Box t;

		//是否有终点
		if (true == isEnd(OpenList, endBox.zb))
		{

			auto tmp = getResult(endBox.zb);

			//返回的是坐标顺序是从末尾到起点 把顺序倒过来
			reverse(tmp.begin(), tmp.end());

			return tmp;
			
		}

		//找到F值最小的方格
		t = findFleast(OpenList);

		//移除OpenList
		removeBox(OpenList, t);


		//加入到CloseList
		CloseList.push_back(t);

		//找出当前上下左右可以到达的格子
		auto tmpp = findBoxsCanArrive(t.zb);

		//往四个方向走一步 所以步数+1
		sumStep++;
		for (auto c : tmpp)
		{
			//看是否有格子存在OpenList中
			if (false == findBox(OpenList, c))
			{
				//如果不在 加入OpenList
				//计算F,G,H值
				//把当前格子作为这个要加入Openlist格子的父亲结点
				c.H = CalcmyPointH(c.zb, endmyPoint);
				c.G = sumStep;
				c.F = c.H + c.G;
				c.parent = t.zb;
				g_ParentmyPoint[c.zb] = t.zb;

				//加入到OpenList
				OpenList.push_back(c);

				myPoint tmp = c.zb;
				g_Flag[tmp] = 1;
			}

		}
		Box next;
		next.zb = t.zb;
		next.parent.m_x = startBox.zb.m_x;
		next.parent.m_y = startBox.zb.m_y;

		startBox = next;

		cLoop++;
		if (cLoop >= 10000)
			break;
	}
	return ret;
}