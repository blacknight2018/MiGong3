#include "myMap.h"
#include "RectChoose.h"
#include "AStar.h"
#include "MapEdit.h"
#include <mmsystem.h>
#include <comutil.h>  

#pragma comment(lib, "winmm.lib") 
#pragma comment(lib, "comsuppw.lib")

// 全局变量:
HWND g_mainHwnd = 0;
CMapEdit g_mapEd;
HINSTANCE g_hins=0;
// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass3(HINSTANCE hInstance);
BOOL                InitInstance3(HINSTANCE, int);
LRESULT CALLBACK    WndProc3(HWND, UINT, WPARAM, LPARAM);



int APIENTRY wWinMain3(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);

	g_hins = hInstance;

	//默认设置地图最大 30列20行
	g_mapEd.setMapSize(30, 20);
	g_mainHwnd = (HWND)hPrevInstance;


	// TODO: 在此处放置代码。


	// 初始化全局字符串
	MyRegisterClass3(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance3(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = 0;

	MSG msg;

	// 主消息循环:
	while (GetMessage(&msg, 0, 0, 0))
	{

		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}

	return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass3(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc3;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = L"Migong3";
	wcex.hIconSm = 0;

	return RegisterClassExW(&wcex);
}


//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance3(HINSTANCE hInstance, int nCmdShow)
{
 
	int mapWidth=0, mapHeight=0;

	//根据不同的地图大小 计算出需要的宽度
	//根据输入的二维数组地图的大小返回应该创建的窗口大小
	auto [windowWidth,windowHeight] = g_mapEd.calcMapWidthHeight();

 
	HWND hWnd = CreateWindowW(L"Migong3", L"地图编辑器", WS_OVERLAPPED | WS_SYSMENU,
		CW_USEDEFAULT, 0, windowWidth+10, windowHeight+10, 0, 0, hInstance, 0);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc3(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int ret = 0;
	
	switch (message)
	{
	case WM_CREATE://当主窗口创建完成
	{
		
		g_mapEd.loadPic(g_hins);

		break;
	}
	case WM_LBUTTONDOWN:
	{
		int xPos = LOWORD(lParam);
		int yPos = HIWORD(lParam);
		auto [iX,iY,wx,wy] = g_mapEd.getMapPointByWindowPoint(xPos, yPos);
		int val = g_mapEd.getMapRes(iX,iY);
		val = (1+val) % 4;
		g_mapEd.setMapNode(iX, iY, val);
		RECT rt = { 0 };
		rt.left = wx;
		rt.top = wy;
		rt.bottom = rt.top + gameMap::m_box_height;
		rt.right = rt.left + gameMap::m_box_height;
		InvalidateRect(hWnd, &rt, true);


		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		
		g_mapEd.drawMap(ps.hdc);

		//结束绘制
		EndPaint(hWnd, &ps);


		break;
	}
	case WM_CLOSE:
	{	
		ret = MessageBoxA(g_mainHwnd, "是否保存地图?", "询问你啊", MB_OKCANCEL);
		if (ret == 1)
		{
			g_mapEd.saveMapToFile();
			MessageBoxA(g_mainHwnd, "地图已经保存在当前目录下newmap.txt", "提示", MB_OK | MB_ICONMASK);
		}
		DestroyWindow(hWnd);
		break;
	}
	case WM_DESTROY:
		ShowWindow(g_mainHwnd, SW_SHOW);
		PostQuitMessage(0);
		
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


void CMapEdit::drawMap(HDC drawdc)
{
	unsigned int i, j;
	int ls = 0;

	//绘制背景图
	

	for (j = 0; j < this->m_height; j++)
	{
		for (i = 0; i < this->m_width; i++)
		{
			int state = 0;
			int srcx, srcy;

			Gdiplus::Graphics graphics(drawdc);
			state = this->m_Map[j][i];
			auto p1 = new Gdiplus::Pen(Gdiplus::Color(255, 0, 0, 0));
			Gdiplus::SolidBrush* b1;
			srcx = m_box_width * i;
			srcy = j * m_box_height;

			tuple<int,int> winXY;
			switch (state)
			{
			case 1://障碍
				//创建gdi+对象来画矩形 漂亮
				b1 = new Gdiplus::SolidBrush(Color(100, 255, 0, 0));

				graphics.FillRectangle(b1, srcx, srcy, m_box_width, m_box_height);

				delete b1;
				break;
			case 2://起点
				winXY = this->getWindowPointByMapPoint(i, j);
				drawBmp(drawdc, m_qid, get<0>(winXY), get<1>(winXY), m_box_width, m_box_height);
				break;
			case 3://绘制终点

				//设置终点坐标
				winXY = this->getWindowPointByMapPoint(i, j);
				drawBmp(drawdc, m_zhd, get<0>(winXY), get<1>(winXY), m_box_width, m_box_height);
				break;

			}
			//绘制方框
			graphics.DrawRectangle(p1, 0 + m_box_width * i, 0 + j * m_box_height, m_box_width, m_box_height);

		}
	}
}

void CMapEdit::setMapSize(int w, int h)
{
	this->m_width = w;
	this->m_height = h;
}
void CMapEdit::setMapNode(int x, int y,int val)
{

	this->m_Map[x][y] = val;
}
void CMapEdit::saveMapToFile()
{
	int i=0, j=0;
	ofstream ofmap;
	ofmap.open("newmap.txt", ios::out);
	if (ofmap.is_open())
	{
		for (i=0;i<this->m_height;i++)
		{
			for (j = 0; j < this->m_width; j++)
			{
				ofmap << m_Map[i][j];
				if (j != this->m_width - 1)
				{
					ofmap << ",";
				}
			}
			ofmap << endl;
		}
		ofmap.close();
	}
	
}