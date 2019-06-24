// MiGong3.cpp : 定义应用程序的入口点。
//


#include "MiGong3.h"
#include "RectChoose.h"
#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
HWND g_hWnd = 0;								// 主窗口窗口句柄


void dance();
void sang();
void rap();
void baskball();
void mapedit();

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。



    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MIGONG3, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }


    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, 0, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MIGONG3));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, WINDOWWIDTH, WINDOWHEIGH, nullptr, nullptr, hInstance, nullptr);

   //初始化GDI+工作环境
   ULONG_PTR m_gdiplusToken;
   Gdiplus::GdiplusStartupInput StartupInput;
   GdiplusStartup(&m_gdiplusToken, &StartupInput, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//将背景图画在窗口上
void DrawBackground(HWND hWnd,HDC dc)
{
	//获取背景图宽度高度
	BITMAP bmp;
	HBITMAP bk = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
	GetObject(bk, sizeof(BITMAP), &bmp);

	
	//获取窗口宽度高度
	RECT rect;
	int width, height;
	GetWindowRect(hWnd, &rect);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	//创建内存DC
	HDC hMdc = CreateCompatibleDC(dc);
	SelectObject(hMdc, (HGDIOBJ)bk);
	StretchBlt(dc, 0, 0, width, height, hMdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

	//DC对象用完释放
	ReleaseDC(hWnd,dc);
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
CRectCall g_rtCall;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

	//当窗口创建完毕后收到这个消息
	case WM_CREATE:
	{

		g_hWnd = hWnd;

		g_rtCall.SetHwnd(hWnd);

		
		
		//菜单一
		CRectChooseText * g_first= new CRectChooseText(WINDOWWIDTH - 500,0, 300,50,"跳(简单)",(pfunClick)dance);
		g_rtCall.RegisterRectChoose((CRectChoose*)g_first);
		
		//菜单二
		CRectChooseText* g_second = new CRectChooseText(WINDOWWIDTH - 500, 120, 300, 50, "唱(容易)", (pfunClick)sang);
		g_rtCall.RegisterRectChoose((CRectChoose*)g_second);

		//菜单三
		CRectChooseText* g_thrid = new CRectChooseText(WINDOWWIDTH - 500, 240, 300, 50, "RAP(难)", (pfunClick)rap);
		g_rtCall.RegisterRectChoose((CRectChoose*)g_thrid);

		//菜单四
		CRectChooseText* g_four = new CRectChooseText(WINDOWWIDTH - 500, 360, 300, 50, "篮球(很难)", (pfunClick)baskball);
		g_rtCall.RegisterRectChoose((CRectChoose*)g_four);

		//菜单五
		CRectChooseText* g_five = new CRectChooseText(WINDOWWIDTH - 500, 480, 300, 50, "地图编辑器", (pfunClick)mapedit);
		g_rtCall.RegisterRectChoose((CRectChoose*)g_five);

		break;
	}
	case WM_LBUTTONDOWN :
	{

		//通知其下所有对象收到鼠标消息
		int x, y;
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		g_rtCall.RcevieClick(x, y);



		break;
	}
    
		//当窗口需要刷新画面时候这里被执行
    case WM_PAINT:
        {
            PAINTSTRUCT ps;

            HDC hdc = BeginPaint(hWnd, &ps);

			DrawBackground(hWnd,hdc);

			HDC mydc = GetDC(hWnd);

			g_rtCall.RecviePaint(hWnd,mydc);


			ReleaseDC(hWnd,mydc);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

wstring StrToWstr(string str)
{
	wstring result;
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	TCHAR* buffer = new TCHAR[len + 1];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';             //添加字符串结尾  
	result.append(buffer);
	delete[] buffer;
	return result;

}

string  WstrToStr(wstring wstr)
{
	string result;
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	result.append(buffer);
	delete[] buffer;
	return result;
}

void dance()
{
	MessageBoxA(NULL, "跳", NULL, MB_OK);
	ShowWindow(g_hWnd, SW_HIDE);
	wWinMain2(hInst, (HINSTANCE)g_hWnd, (wchar_t*)L"map1.txt", SW_SHOW);
	
}

void sang()
{
	MessageBoxA(NULL, "唱", NULL, MB_OK);
	ShowWindow(g_hWnd, SW_HIDE);
	wWinMain2(hInst, (HINSTANCE)g_hWnd, (wchar_t*)L"map2.txt", SW_SHOW);
}
void rap()
{
	MessageBoxA(NULL, "RAP", NULL, MB_OK);
	ShowWindow(g_hWnd, SW_HIDE);
	wWinMain2(hInst, (HINSTANCE)g_hWnd, (wchar_t*)L"map3.txt", SW_SHOW);
}
void baskball()
{
	MessageBoxA(NULL, "篮球", NULL, MB_OK);
	ShowWindow(g_hWnd, SW_HIDE);
	wWinMain2(hInst, (HINSTANCE)g_hWnd, (wchar_t*)L"map4.txt", SW_SHOW);
}

//当点击地图编辑器时调用这个函数
void mapedit()
{

	wWinMain3(hInst, (HINSTANCE)g_hWnd, nullptr, SW_SHOW);

	ShowWindow(g_hWnd, SW_SHOW);

}