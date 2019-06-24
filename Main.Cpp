#include "myMap.h"
#include "RectChoose.h"
#include "AStar.h"
#include <mmsystem.h>
#include <comutil.h>  

#pragma comment(lib, "winmm.lib") 
#pragma comment(lib, "comsuppw.lib")

// 全局变量:
gameMap *g_map=nullptr;
HINSTANCE g_hInst;                                // 当前实例
HWND g_parentHwnd = 0;							  //主窗口的窗口句柄
HWND g_myhWnd = 0;								  //当前窗口的窗口句柄
CRectCall g_call;								  //负责管理窗口矩形鼠标单击的对象
int mapWidth=0, mapHeight=0;					  //地图在窗口上的宽度和高度
CRectChooseText *g_findPath=nullptr;			  //【自动寻路】对象

int g_Click = 0;								 //鼠标点击次数
bool g_ExitFlag = false;						 //当前窗口退出标记
// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass2(HINSTANCE hInstance);
BOOL                InitInstance2(HINSTANCE, int);
LRESULT CALLBACK    WndProc2(HWND, UINT, WPARAM, LPARAM);



int APIENTRY wWinMain2(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	
	g_Click = 0;
	mapWidth = mapHeight = 0;
	g_myhWnd = 0;
	g_parentHwnd = 0;

	g_parentHwnd = (HWND)hPrevInstance;			//保存主窗口窗口句柄 因为游戏结束后需要显示出来
	wstring tmp(lpCmdLine);						//临时的wstring对象 
	g_map = new gameMap(WstrToStr(tmp).c_str());//wstring类型转为string类型
	 

	// TODO: 在此处放置代码。


	// 初始化全局字符串
	MyRegisterClass2(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance2(hInstance, nCmdShow))
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
ATOM MyRegisterClass2(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc2;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = L"Migong2";
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
BOOL InitInstance2(HINSTANCE hInstance, int nCmdShow)
{
	g_hInst = hInstance; // 将实例句柄存储在全局变量中

	

	//根据不同的地图大小 计算出需要的宽度
	auto wh = g_map->calcMapWidthHeight();
	mapWidth = get<0>(wh);
	mapHeight = get<1>(wh);

	//在地图大小的基础上加上一个偏移大小 留出空间来显示文字选项
	HWND hWnd = CreateWindowW(L"Migong2", L"坤坤找篮球", WS_OVERLAPPED | WS_SYSMENU,
		CW_USEDEFAULT, 0, mapWidth + 250, mapHeight + 50, 0, 0, hInstance, 0);


	//窗口创建完毕后
	g_findPath = new CRectChooseText(mapWidth , 20, 220, 40, "我是蔡徐坤,开始寻路", findPath);
	g_findPath->SetText("我是蔡徐坤,开始寻路");
	g_call.RegisterRectChoose(g_findPath);

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
LRESULT CALLBACK WndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE://当主窗口创建完成
	{
		g_myhWnd = hWnd;

		g_call.SetHwnd(hWnd);

		//当窗口创建完毕 加载图片资源
		g_map->loadPic(g_hInst);


		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WatchThread, NULL, NULL, NULL);

		break;
	}
	case WM_LBUTTONDOWN:
	{
		int xPos = LOWORD(lParam);
		int yPos = HIWORD(lParam);
		//更新人物坐标
		g_map->UpDateXY(xPos, yPos);

		//通知有鼠标点击消息
		g_call.RcevieClick(xPos, yPos);

		g_Click ++ ;

		//当窗口刚刚创建的时候设置为true
		g_ExitFlag = true;
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;

		RECT rect;
		//开始画质 返回一块屏幕画板句柄
		HDC hdc = BeginPaint(hWnd, &ps);


		//获取窗口大小
		GetWindowRect(hWnd, &rect);
		//创建一块和窗口画布以用大的内存画布
		auto bitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
		auto mdc = CreateCompatibleDC(hdc);
		//把我们要画的画布选进内存画板里
		SelectObject(mdc, bitmap);

		//地图 人物贴图都画在这块内存画板里
		g_map->setHwnd(hWnd);
		g_map->setDc(mdc);
		g_map->drawIndc(mdc);

		//把这块内存画板贴到我们的屏幕画板上
		BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, mdc, 0, 0, SRCCOPY);
		ReleaseDC(hWnd, mdc);


		//通知绘制我们的文字
		g_call.RecviePaint(hWnd, hdc);

		//结束绘制
		EndPaint(hWnd, &ps);


		break;
	}

	case WM_DESTROY:
		//窗口退出设置为false
		g_ExitFlag = false;
		PostQuitMessage(0);
		ShowWindow(g_parentHwnd, SW_SHOW);

		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//播放音乐 文件路径是程序运行目录下的文件jntm.mp3
void playstartMusic()
{
	mciSendStringA("open jntm.mp3 alias media", NULL, NULL, NULL);
	mciSendStringA("play media", NULL, NULL, NULL);
}

//从当前人物坐标 开始寻路
void findPath()
{
	//首先获取人物在二维数组中的坐标索引
	//当前人物坐标 起点
	auto [x,y] = g_map->getMapIndexPos();
	//地图终点
	auto [winEndx,winEndY,arryEndx, arryEndy] = g_map->getEndPoint();
	//地图指针
	int* pMap = g_map->getMapPointer();
	//地图宽度高度
	auto [w,h] = g_map->getMapWidthHeight();
	

	//创建一个A*寻路对象
	AStarFind* pAStart = new AStarFind;
	pAStart->SetMap(pMap, w, h);	//设置地图
	pAStart->SetmyPoint(x, y, arryEndx, arryEndy);	//设置起点终点
	auto result=pAStart->StartFind();
	
	DrawPath(result);
}

//绘制路径 输入是各个坐标绘制上方块
void DrawPath(vector<vector<int>> Path)
{
	HDC dc;
	//获取一个画布句柄
	dc = GetDC(g_myhWnd);
	//创建一个GDI+对象 
	Gdiplus::Graphics gp(dc);

 
	Gdiplus::SolidBrush* hBrush = new SolidBrush(Color(125,0,125,0)); //透明度125  蓝色
	for (auto z:Path)
	{
		int arryx = z[0];
		int arryy = z[1];
		

		auto [winy,winx]=g_map->getWindowPointByMapPoint(arryx, arryy);

		//gp.DrawRectangle(hPen, winx, winy, gameMap::m_box_width, gameMap::m_box_height);
		gp.FillRectangle(hBrush,winx,winy, gameMap::m_box_width, gameMap::m_box_height);
	}

	//释放
	ReleaseDC(g_myhWnd,dc);

	delete hBrush;
}

//用于记录点击数 和 显示时间
void WatchThread()
{
	int iTime = 0;
	char buf[30] = { 0 };
	HDC dc = GetDC(g_myhWnd);

	//设置使用背景色 并且使用红色
	SetBkMode(dc, OPAQUE);
	SetBkColor(dc, RGB(255, 0, 0));

	HFONT ft = CreateFontA(-20/*高度*/, -7/*宽度*/, 0/*不用管*/, 0/*不用管*/, 400 /*一般这个值设为400*/, FALSE/*不带斜体*/, FALSE/*不带下划线*/, FALSE/*不带删除线*/, DEFAULT_CHARSET,      OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,                 DEFAULT_QUALITY,                 FF_DONTCARE,      "微软雅黑"              );
		
	SelectObject(dc, ft);

	while (true)
	{
		//HWND g_myhWnd = 0;								  //当前窗口的窗口句柄
	
		sprintf_s(buf, "Time:%05d(s)", iTime);

		TextOutA(dc, mapWidth, 100,buf, strlen(buf));


		sprintf_s(buf, "单击次数:%02d", g_Click);

		TextOutA(dc, mapWidth, 300, buf, strlen(buf));

		Sleep(1000);
		
		iTime++;

		//如果当前窗口已经关闭了 则退出线程
		if (g_ExitFlag == true)
		{
			//ExitThread(0);
		}

		
		
	}

	ReleaseDC(g_myhWnd, dc);
	

}

/*
296:参考https://blog.csdn.net/softn/article/details/51718347
*/