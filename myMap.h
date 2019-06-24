#pragma once
#include "MiGong3.h"

void playstartMusic();
class gameMap
{
protected:
	HWND m_hwnd;
	HDC m_dc;

	ULONG_PTR m_gdiplusToken;	              //GDI+���� ��ʼ��GDI+��ʱ�򷵻ص� ֮��ûɶ����
	unsigned m_width, m_height;               //��ͼ��ȸ߶� ����Ŀ�ȸ߶� �Ƕ�ά����ĳ��ȸ߶�
	HBITMAP m_qid, m_zhd, m_bjt;              //����յ㱳��ͼ��λͼ���
	int m_nowWindowx, m_nowWindowy;			  //���ﵱǰ��Ļ����
	int m_nowmapIx, m_nowmapIy;		          //�ڵ�ͼ�е�����
	int m_Map[50][50];						  //��ά�����ͼ����
	int m_Windowendx, m_Windowendy;			  //�յ�����(��Ļ����)
	int m_ArrayendX, m_ArrayendY;			  //�յ�����(��������)



public:
	static int m_box_width, m_box_height;	   //���ӵĴ�С
public:
	gameMap(string filename);
	gameMap();

	void loadPic(HINSTANCE mHin);
protected:
	void drawBmp(HDC drawdc, HBITMAP& hbitmap, int x, int y, int w, int h);
	void drawBmp2(HDC drawdc, HBITMAP& hbitmap, int x, int y, int w, int h);
public:
	void drawIndc(HDC drawdc);//�滭���ĺ���
	void setHwnd(HWND hd);	  //���ô��ھ��
	void setDc(HDC dc);		  //���û���ID
private:
	void setNewPos(int x, int y, int ix, int iy);
public:
	//void StartFromFile(wstring fileName);
	void LoadMap(wstring fileName);			//���ļ��м��ص�ͼ
	void UpDateXY(int x, int y);			//��������
private:
	void DrawBitmapToAPoint(int x, int y, HBITMAP& dw);



	HBITMAP& GetStartBitmap();
public:
	tuple<int, int>			  getWindowPointByMapPoint(int, int);
	tuple<int, int, int, int> getMapPointByWindowPoint(int, int);
	tuple<int, int>			  calcMapWidthHeight();			//�����ͼ��С ���ڴ������ڵ�ʱ��
	tuple<int, int>			  getMapIndexPos();				//��ȡ��ǰ�����ڶ�ά�����е�����
	tuple<int, int>			  getMapWidthHeight();			//��ȡ��ͼ�Ŀ�ȸ߶�
	tuple<int, int, int, int> getEndPoint();
	int getMapRes(int ix, int iy);

	int* getMapPointer();							//���ص�ͼ��ָ��
};