
#include "myMap.h"
#include "resource.h"

int gameMap::m_box_height = 35;
int gameMap::m_box_width = 35;
//构造函数1
gameMap::gameMap()
{
	memset(this->m_Map, 0, 50 * 50 * sizeof(char));//默认地图最大50*50
	this->m_height = 0;
	this->m_width = 0;
	this->m_nowWindowx = 0;
	this->m_nowWindowy = 0;
	this->m_dc = 0;
	this->m_nowmapIx = 0;
	this->m_nowmapIy = 0;
	Gdiplus::GdiplusStartupInput StartupInput;
	GdiplusStartup(&m_gdiplusToken, &StartupInput, NULL);
}

//构造函数 从文件中创建地图
gameMap::gameMap(string filename)
{
	gameMap();
	LoadMap(StrToWstr(filename));
}


void gameMap::drawIndc(HDC drawdc)
{
	unsigned int i, j;
	int ls = 0;

	//绘制背景图
	drawBmp2(drawdc, m_bjt, 0, 0, 1181, 615);//, get<0>(calcMapWidthHeight()), get<1>(calcMapWidthHeight()));

	for (j = 0; j < m_height; j++)
	{
		for (i = 0; i < m_width; i++)
		{
			int state = 0;
			int srcx, srcy;
			Gdiplus::Graphics graphics(drawdc);
			state = this->m_Map[j][i];
			auto p1 = new Gdiplus::Pen(Gdiplus::Color(255, 0, 0, 0));
			Gdiplus::SolidBrush* b1;

			srcx = m_box_width * i;
			srcy = j * m_box_height;
			switch (state)
			{
			case 1://障碍
				//创建gdi+对象来画矩形 漂亮
				b1 = new Gdiplus::SolidBrush(Color(100, 255, 0, 0));

				graphics.FillRectangle(b1, srcx, srcy, m_box_width, m_box_height);

				delete b1;
				break;
			case 2://起点
				drawBmp(drawdc, m_qid, this->m_nowWindowx, m_nowWindowy, m_box_width, m_box_height);
				break;
			case 3://绘制终点
				//设置终点坐标
				//m_Windowendx = srcx;
				//m_Windowendy = srcy;
				//m_ArrayendX = j;
				//m_ArrayendY = i;
				drawBmp(drawdc, m_zhd, srcx, srcy, m_box_width, m_box_height);
				break;

			}
			//绘制方框
			graphics.DrawRectangle(p1, 0 + m_box_width * i, 0 + j * m_box_height, m_box_width, m_box_height);

		}
	}
	//SelectObject(drawdc, oPen);

}

//计算地图的宽度高度
tuple<int, int> gameMap::calcMapWidthHeight()
{
	return make_tuple((m_width + 1) * m_box_width, (1 + m_height) * m_box_height);
}

//从程序资源中加载图片资源 
void gameMap::loadPic(HINSTANCE mHin)
{
	this->m_qid = LoadBitmap(mHin, MAKEINTRESOURCE(IDB_BITMAP4));//人物起点
	this->m_zhd = LoadBitmap(mHin, MAKEINTRESOURCE(IDB_BITMAP2));//终点图片
	this->m_bjt = LoadBitmap(mHin, MAKEINTRESOURCE(IDB_BITMAP3));//背景图
//	this->m_actiongif = new Image(L"D:\\dlq.gif");

	//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)loadgif, (LPVOID)this, 0, 0);

}
//在指定位置X,Y绘制W,H的位图 
//取原图的60*60大小 绘制到目标画板上
void gameMap::drawBmp(HDC drawdc, HBITMAP& hbitmap, int x, int y, int w, int h)
{
	HDC memDC = CreateCompatibleDC(drawdc);
	auto oldBit = (HBITMAP)SelectObject(memDC, hbitmap);
	//BitBlt(drawdc, srcx, srcy+2, srcx + 60, srcy + 60,memDC,0,0, SRCCOPY);
	StretchBlt(drawdc, x, y, w, h, memDC, 0, 0, 60, 60, SRCCOPY);
	SelectObject(memDC, oldBit);
	//DeleteObject(hbitmap);
}
//取原图的W,H部分绘制到目标画板上
void gameMap::drawBmp2(HDC drawdc, HBITMAP& hbitmap, int x, int y, int w, int h)
{
	HDC memDC = CreateCompatibleDC(drawdc);
	RECT rt;
	auto oldBit = (HBITMAP)SelectObject(memDC, hbitmap);
	//BitBlt(drawdc, x, y , x + w, y + h, memDC, 0, 0, SRCCOPY);
	GetWindowRect(m_hwnd, &rt);
	StretchBlt(drawdc, x, y, rt.right - rt.left, rt.bottom - rt.top, memDC, 0, 0, w, h, SRCCOPY);

	SelectObject(memDC, oldBit);
	//DeleteObject(hbitmap);
}

void gameMap::setHwnd(HWND m_hwnd)
{
	this->m_hwnd = m_hwnd;
}

tuple<int, int> gameMap::getWindowPointByMapPoint(int x, int y)
{
	return make_tuple(x * m_box_width, y * m_box_height);
}

void gameMap::setDc(HDC hdc)
{
	this->m_dc = hdc;
}

//从二维数组地图中计算出对应的应该在屏幕窗口中的坐标值
tuple<int, int, int, int> gameMap::getMapPointByWindowPoint(int x, int y)
{
	unsigned int i, j;
	for (j = 0; j < m_height; j++)
	{
		for (i = 0; i < m_width; i++)
		{
			int srcx, srcy;
			srcx = i * m_box_width;
			srcy = j * m_box_height;
			if (srcx <= x && srcy <= y && srcx + m_box_width >= x && srcy + m_box_height >= y)
			{
				return make_tuple(j, i, i * m_box_width, j * m_box_height);

			}
		}
	}
	return make_tuple(-1, -1, -1, -1);
}


//设置当前任务的坐标 x y是对应的屏幕坐标值 ix iy对应的是在地图中的坐标索引
void gameMap::setNewPos(int x, int y, int ix, int iy)
{
	this->m_nowWindowx = x;
	this->m_nowWindowy = y;
	this->m_nowmapIx = ix;
	this->m_nowmapIy = iy;

	if (y == m_Windowendx && x == m_Windowendy)
	{
		MessageBoxA(NULL, "鸡你太美！", "", MB_OK);
		playstartMusic();
		SendMessage(m_hwnd, WM_CLOSE, 0, 0);
		return;
	}

}

//返回当前人物在二维数组地图中的坐标索引
tuple<int, int> gameMap::getMapIndexPos()
{
	return make_tuple(m_nowmapIx, m_nowmapIy);
}

//返回指定二维数组坐标某个位置的数值
int gameMap::getMapRes(int ix, int iy)
{
	return m_Map[ix][iy];
}

tuple<int, int> gameMap::getMapWidthHeight()
{
	return make_tuple(m_width, m_height);
}


//返回人物的位图
HBITMAP& gameMap::GetStartBitmap()
{
	return m_qid;
}


/*
//从文件中加载地图
void gameMap::StartFromFile(wstring fileName)
{
	//加载需要的资源位图
	LoadMap(fileName);
	InvalidateRect(m_hwnd, 0, true);

}
*/
void gameMap::LoadMap(wstring fileName)
{
	int i, j;
	memset(this->m_Map, 0, 50 * 50 * sizeof(char));
	this->m_height = 0;
	this->m_width = 0;
	this->m_nowWindowx = 0;
	this->m_nowWindowy = 0;
	this->m_dc = 0;
	this->m_nowmapIx = 0;
	this->m_nowmapIy = 0;

	ifstream inmap(fileName, ios::in);
	if (inmap.is_open())
	{
		char ch = 0;
		int k = 0;
		inmap.get(ch);
		while (!inmap.eof())
		{
			//
			if ('\n' == ch)
			{
				this->m_height++;
				if (k != 0)
					this->m_width = k;
				k = 0;

			}
			else
			{
				if (ch != ',')
					m_Map[this->m_height][k++] = ch - '0';

			
			}
			inmap.get(ch);
		}
		//地图大小固定 
		this->m_height = 20;
		this->m_width = 30;


		//找起点和终点
		tuple<int, int> tmp;
		for (i = 0; i < m_height; i++)
		{
			for (j = 0; j < m_width; j++)
			{
				switch (m_Map[i][j])
				{
				case 2:
					m_nowmapIx = i;
					m_nowmapIy = j;
					tmp = this->getWindowPointByMapPoint(j, i);
					m_nowWindowx = get<0>(tmp);// m_nowmapIx* gameMap::m_box_width;
					m_nowWindowy = get<1>(tmp);// m_nowmapIy* gameMap::m_box_height;
					
					break;
			
				case 3:
					m_ArrayendX = i;
					m_ArrayendY = j;
					//m_Windowendx = m_ArrayendX * gameMap::m_box_width;
					//m_Windowendy = m_ArrayendY * gameMap::m_box_height;
					tmp = this->getWindowPointByMapPoint(i, j);
					m_Windowendx = get<0>(tmp);// m_nowmapIx* gameMap::m_box_width;
					m_Windowendy = get<1>(tmp);// m_nowmapIy* gameMap::m_box_height;
					break;
				default:
					break;
				}
			}

		}

	}
	else
	{
		MessageBoxA(NULL, "打开地图文件失败", "ERROR", MB_OK | MB_ICONWARNING);
	}
}


void gameMap::DrawBitmapToAPoint(int x, int y, HBITMAP& dw)
{
	HDC dc = GetDC(this->m_hwnd);
	Graphics graphics(dc);
	auto p1 = new Gdiplus::Pen(Gdiplus::Color(255, 0, 0, 0));


	this->drawBmp(dc, dw, x, y, gameMap::m_box_width, gameMap::m_box_height);
	graphics.DrawRectangle(p1, x, y, gameMap::m_box_width, gameMap::m_box_height);
	ReleaseDC(m_hwnd, dc);
}
void removeRect(HWND hWnd, int lastSrcX, int lastSrcY)
{
	RECT flushrect = { 0 };
	flushrect.left = lastSrcX;
	flushrect.right = gameMap::m_box_width + lastSrcX;
	flushrect.top = lastSrcY;
	flushrect.bottom = lastSrcY + gameMap::m_box_height;
	InvalidateRect(hWnd, &flushrect, true);
}

//人物移动核心函数
void gameMap::UpDateXY(int xPos, int yPos)
{

	auto [desindexx, desindexy, des_scren_x, des_scren_y] = this->getMapPointByWindowPoint(xPos, yPos);
	auto [now_index_x, now_index_y] = this->getMapIndexPos();

	int tmp_index_y;
	tmp_index_y = m_nowWindowy;

	if (desindexx == now_index_x && desindexy != now_index_y)
	{
		if (now_index_y <= desindexy)
			for (int i = now_index_y; i <= desindexy; i++)
			{
				if (this->getMapRes(desindexx, i) == 1)
				{
					break;
				}
				tmp_index_y = i;
			}
		else
		{
			for (int i = now_index_y; i >= desindexy; i--)
			{
				if (this->getMapRes(desindexx, i) == 1)
				{
					break;
				}
				tmp_index_y = i;
			}
		}
		//移动
		auto [y, x] = this->getWindowPointByMapPoint(now_index_x, now_index_y);
		auto [y2, x2] = this->getWindowPointByMapPoint(desindexx, tmp_index_y);
		DrawBitmapToAPoint(x2, y2, m_qid);
		removeRect(m_hwnd, x, y);
		this->setNewPos(x2, y2, desindexx, tmp_index_y);
	}
	int tmp_index_x;
	tmp_index_x = m_nowWindowx;
	if (desindexy == now_index_y && desindexx != now_index_x)
	{
		if (now_index_x <= desindexx)
			for (int i = now_index_x; i <= desindexx; i++)
			{
				if (this->getMapRes(i, desindexy) == 1)
				{
					break;
				}
				tmp_index_x = i;
			}
		else
		{
			for (int i = now_index_x; i >= desindexx; i--)
			{
				if (this->getMapRes(i, desindexy) == 1)
				{
					break;
				}
				tmp_index_x = i;
			}
		}
		auto [y, x] = this->getWindowPointByMapPoint(now_index_x, now_index_y);

		auto [y2, x2] = this->getWindowPointByMapPoint(tmp_index_x, desindexy);
		DrawBitmapToAPoint(x2, y2, m_qid);
		removeRect(m_hwnd, x, y);
		this->setNewPos(x2, y2, tmp_index_x, desindexy);
	}
	return;
}

int* gameMap::getMapPointer()
{
	return (int*)m_Map;
}

tuple<int, int,int,int> gameMap::getEndPoint()
{
	return tuple<int, int,int,int>(this->m_Windowendx, m_Windowendy,m_ArrayendX,m_ArrayendY);
}