#pragma once

#include "resource.h"
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <tuple>
#include <vector>
#include <gdiplus.h>
#include <map>
#include <algorithm>
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
using namespace Gdiplus;
using namespace std;
#pragma comment(lib,"gdiplus.lib")

wstring StrToWstr(string str);
string  WstrToStr(wstring wstr);

const int WINDOWWIDTH = 1500;//���� ���ڿ��
const int WINDOWHEIGH = 800;//���� ���ڸ߶�


//��Ϸ����Ĵ��ڵ���ں��� ��Ϊ��Ҫ���� ��������������
int APIENTRY wWinMain2(_In_ HINSTANCE hInstance,
	 HINSTANCE hPrevInstance,
	 LPWSTR    lpCmdLine,
	 int       nCmdShow);

//��ͼ�༭�������ں���
int APIENTRY wWinMain3(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow);

//�������ֵĺ��� ������Ϸʤ���󲥷�
void playstartMusic();

//A*Ѱ·����
void findPath();

//����·�� ����A*�ҵ�·���������Ļ
void DrawPath(vector<vector<int>> Path);

void WatchThread();