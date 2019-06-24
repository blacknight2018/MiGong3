#pragma once
#include "MiGong3.h"
typedef   void(*pfunClick)();
typedef   void(*pfunPaint)(HDC);

//表示一个区域被单击的类
class CRectChoose
{
protected:
	HDC m_dc;
	RECT m_rect;					//这个被单击的矩形所在的位置和高度宽度
	pfunClick m_pFuncClickpointer;  //鼠标单击函数指针地址
	pfunPaint m_pFuncPaintpointer;  //绘制图形函数指针地址
public:
	virtual void click()	  =0;			//当窗口上这个矩形被单击后调用这个函数
	virtual void paint(HWND,HDC)   =0;		//当窗口在这个矩形需要绘制的时候调用这个函数
	RECT getRect();
	CRectChoose();
};



//在CRectChoose类的基础上添加了绘制文字显示
class CRectChooseText :public CRectChoose
{
private:
	string m_drawStr;				 //需要绘制的文字
private:
	void CalcStrRect(int fSize,int &w,int &h); //计算绘制的文字的高度宽度保存起来
public:
	void SetText(string str);		 //设置需要绘制的文字
	 
	void click();					 //重写基类函数
	void paint(HWND,HDC);			 //重写基类函数

	

	CRectChooseText(int x,int y, int w,int h,string str, pfunClick p);
private:
	void DrawRectangle(HWND, HDC);		 //绘制一个矩形边框
	void DrawOutText(HWND, HDC);
};






class CRectCall
{
private:
	HWND m_hWnd;
public:
	vector< CRectChoose*> m_allObject;			//窗口上所有的矩形
	void RegisterRectChoose(CRectChoose *obj);	//注册一个RectChoose对象
	void RcevieClick(int X,int Y);				//当窗口收到鼠标左键单击的消息调用这个函数
	void RecviePaint(HWND hWnd ,HDC dc);		//当窗口收到需要重新绘图的消息时调用这个函数 参数是画板句柄
	void SetHwnd(HWND hwnd);					//设置所要管理的窗口句柄(ID)

};