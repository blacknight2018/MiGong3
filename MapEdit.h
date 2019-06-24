#pragma once
#include "MiGong3.h"
#include "myMap.h"

//µÿÕº±‡º≠¿‡
class CMapEdit :public gameMap
{
public:
	void drawMap(HDC dc);
	void setMapSize(int w, int h);
	void setMapNode(int x,int y,int val);
	void saveMapToFile();
};

