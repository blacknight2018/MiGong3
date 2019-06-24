#include "RectChoose.h"
//CRectCall类
void CRectCall::RcevieClick(int x, int y)
{
	//遍历所有的对象
	for (auto c:m_allObject)
	{
		RECT rt;
	
		rt = c->getRect();
		//判断是否在范围内
		if (x > rt.left && x<rt.right && y>rt.top && y < rt.bottom)
		{
			c->click();//符合条件调用click函数通知其
		}
	}
}

void CRectCall::RegisterRectChoose(CRectChoose *obj)
{
	this->m_allObject.push_back(obj);
}

void CRectCall::SetHwnd(HWND hwnd)
{
	this->m_hWnd = hwnd;
}
void CRectCall::RecviePaint(HWND hWnd,HDC dc)
{
	for (auto c : m_allObject)
	{
		c->paint(hWnd,dc);
	}
}
//





//CRectChoose类
CRectChoose::CRectChoose()
{
	this->m_rect = { 0 };
	this->m_dc = 0;
	this->m_pFuncClickpointer = nullptr;
	this->m_pFuncPaintpointer = nullptr;
}
RECT CRectChoose::getRect()
{
	return this->m_rect;
	
}

//






//CRectChooseText类
void CRectChooseText::SetText(string str)
{

	this->m_drawStr = str;
	 
	return;
}
void CRectChooseText::CalcStrRect(int fontSize,int& w, int& h)
{
	 
	Graphics gp(m_dc);
	wstring tmp = StrToWstr(m_drawStr);
	Font* ft = new Font(L"微软雅黑",(REAL)fontSize);
	RectF boundRect;
	gp.MeasureString(tmp.c_str(), tmp.length(), ft, PointF(0, 0), &boundRect);
	w = (int)boundRect.Width;
	h = (int)boundRect.Height;
	delete ft;
}
void CRectChooseText::click()
{
	if(m_pFuncClickpointer>0)
		this->m_pFuncClickpointer();
}
void CRectChooseText::paint(HWND hWnd,HDC dc)
{
	m_dc = dc;

	//绘制出矩形
	this->DrawRectangle(hWnd,dc);

	//绘制出文字
	this->DrawOutText(hWnd,dc);

	//调用这个注册的回调函数
	if (m_pFuncPaintpointer > 0)
		this->m_pFuncPaintpointer(dc);
}

void CRectChooseText::DrawOutText(HWND hwnd, HDC dc)
{
	Gdiplus::Graphics gp(dc);
	//gp.DrawString()
	wstring tmp = StrToWstr(m_drawStr);					//多字节转为宽字节版本
	Font* ft = new Gdiplus::Font(L"微软雅黑",16);		//程序默认全都使用微软雅黑字体 这里使用16号大小
	SolidBrush* sb = new SolidBrush(Color(100,125,0,255));//使用蓝色字体 不透明
	int w=0, h=0;
	CalcStrRect(16, w, h);								//获取字体宽度和高度
	int x = m_rect.left;
	int y = m_rect.top;
	int rectWidth = m_rect.right - m_rect.left;
	int rectHeight = m_rect.bottom - m_rect.top;
	gp.DrawString(tmp.c_str(), tmp.length(), ft,PointF(REAL( x+ rectWidth/2-(w/2)), REAL(y+rectHeight/2-(h/2))), sb);
	delete ft;
	delete sb;
}
void CRectChooseText::DrawRectangle(HWND hWnd,HDC dc)
{
	//创建一个GDI+图形对象
	Gdiplus::Graphics gp(dc);
	//创建一只画笔
	Gdiplus::Pen hPen(Color(50,255,0,0),5.0f);

	//Gdiplus::SolidBrush hBrush(Color(100, 255, 0, 0));
	Gdiplus::Rect hRect(m_rect.left,  m_rect.top,  m_rect.right-m_rect.left,  m_rect.bottom-m_rect.top);
	//gp.DrawArc((const Gdiplus::Pen*)&hPen,hRect,100,360.0f);
	gp.DrawRectangle((const Gdiplus::Pen*) & hPen, hRect);

}
CRectChooseText::CRectChooseText(int x, int y, int w, int h, string str, pfunClick p)
{
	//设置文本起始坐标 然后计算宽度高度
	this->m_rect.left = x;
	this->m_rect.top = y;
	this->m_rect.right = x + w;
	this->m_rect.bottom = y + h;
	this->m_drawStr = str;
	

	//设置当鼠标单击的时候要调用的那个函数的函数地址
	this->m_pFuncClickpointer = p;

}

//