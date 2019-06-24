#pragma once
#include "MiGong3.h"
typedef   void(*pfunClick)();
typedef   void(*pfunPaint)(HDC);

//��ʾһ�����򱻵�������
class CRectChoose
{
protected:
	HDC m_dc;
	RECT m_rect;					//����������ľ������ڵ�λ�ú͸߶ȿ��
	pfunClick m_pFuncClickpointer;  //��굥������ָ���ַ
	pfunPaint m_pFuncPaintpointer;  //����ͼ�κ���ָ���ַ
public:
	virtual void click()	  =0;			//��������������α�����������������
	virtual void paint(HWND,HDC)   =0;		//�����������������Ҫ���Ƶ�ʱ������������
	RECT getRect();
	CRectChoose();
};



//��CRectChoose��Ļ���������˻���������ʾ
class CRectChooseText :public CRectChoose
{
private:
	string m_drawStr;				 //��Ҫ���Ƶ�����
private:
	void CalcStrRect(int fSize,int &w,int &h); //������Ƶ����ֵĸ߶ȿ�ȱ�������
public:
	void SetText(string str);		 //������Ҫ���Ƶ�����
	 
	void click();					 //��д���ຯ��
	void paint(HWND,HDC);			 //��д���ຯ��

	

	CRectChooseText(int x,int y, int w,int h,string str, pfunClick p);
private:
	void DrawRectangle(HWND, HDC);		 //����һ�����α߿�
	void DrawOutText(HWND, HDC);
};






class CRectCall
{
private:
	HWND m_hWnd;
public:
	vector< CRectChoose*> m_allObject;			//���������еľ���
	void RegisterRectChoose(CRectChoose *obj);	//ע��һ��RectChoose����
	void RcevieClick(int X,int Y);				//�������յ���������������Ϣ�����������
	void RecviePaint(HWND hWnd ,HDC dc);		//�������յ���Ҫ���»�ͼ����Ϣʱ����������� �����ǻ�����
	void SetHwnd(HWND hwnd);					//������Ҫ����Ĵ��ھ��(ID)

};