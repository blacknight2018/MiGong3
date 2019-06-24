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

const int WINDOWWIDTH = 1500;//常量 窗口宽度
const int WINDOWHEIGH = 800;//常量 窗口高度


//游戏画面的窗口的入口函数 因为需要调用 所以在这里声明
int APIENTRY wWinMain2(_In_ HINSTANCE hInstance,
	 HINSTANCE hPrevInstance,
	 LPWSTR    lpCmdLine,
	 int       nCmdShow);

//地图编辑程序的入口函数
int APIENTRY wWinMain3(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow);

//播放音乐的函数 用于游戏胜利后播放
void playstartMusic();

//A*寻路函数
void findPath();

//绘制路径 用于A*找到路径后绘在屏幕
void DrawPath(vector<vector<int>> Path);

void WatchThread();