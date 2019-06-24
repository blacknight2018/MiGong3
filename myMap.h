#pragma once
#include "MiGong3.h"

void playstartMusic();
class gameMap
{
protected:
	HWND m_hwnd;
	HDC m_dc;

	ULONG_PTR m_gdiplusToken;	              //GDI+令牌 初始化GDI+的时候返回的 之后没啥用了
	unsigned m_width, m_height;               //地图宽度高度 这里的宽度高度 是二维数组的长度高度
	HBITMAP m_qid, m_zhd, m_bjt;              //起点终点背景图的位图句柄
	int m_nowWindowx, m_nowWindowy;			  //人物当前屏幕坐标
	int m_nowmapIx, m_nowmapIy;		          //在地图中的索引
	int m_Map[50][50];						  //二维数组地图数据
	int m_Windowendx, m_Windowendy;			  //终点坐标(屏幕坐标)
	int m_ArrayendX, m_ArrayendY;			  //终点坐标(数组坐标)



public:
	static int m_box_width, m_box_height;	   //格子的大小
public:
	gameMap(string filename);
	gameMap();

	void loadPic(HINSTANCE mHin);
protected:
	void drawBmp(HDC drawdc, HBITMAP& hbitmap, int x, int y, int w, int h);
	void drawBmp2(HDC drawdc, HBITMAP& hbitmap, int x, int y, int w, int h);
public:
	void drawIndc(HDC drawdc);//绘画核心函数
	void setHwnd(HWND hd);	  //设置窗口句柄
	void setDc(HDC dc);		  //设置画板ID
private:
	void setNewPos(int x, int y, int ix, int iy);
public:
	//void StartFromFile(wstring fileName);
	void LoadMap(wstring fileName);			//从文件中加载地图
	void UpDateXY(int x, int y);			//更新坐标
private:
	void DrawBitmapToAPoint(int x, int y, HBITMAP& dw);



	HBITMAP& GetStartBitmap();
public:
	tuple<int, int>			  getWindowPointByMapPoint(int, int);
	tuple<int, int, int, int> getMapPointByWindowPoint(int, int);
	tuple<int, int>			  calcMapWidthHeight();			//计算地图大小 用在创建窗口的时候
	tuple<int, int>			  getMapIndexPos();				//获取当前人物在二维数组中的索引
	tuple<int, int>			  getMapWidthHeight();			//获取地图的宽度高度
	tuple<int, int, int, int> getEndPoint();
	int getMapRes(int ix, int iy);

	int* getMapPointer();							//返回地图的指针
};