#pragma once
/*
#include <stdio.h>
#include <windows.h>
extern HWND hwnd;
#define set0(x) memset(x,0,sizeof(x))
#ifndef MAIN_INCLUDED
typedef struct _Color {
	wchar_t chineseName[3];
	int num;
	bool isCured;
	bool isEradicated;
	int virusRemain;
	void drawCureStatus();
}Color;
#endif
extern Color colors[4];
#ifndef MAIN_INCLUDED
class City {
private:
	int num;
	int neighbor[10];
	int cx, cy;
	wchar_t englishName[10];
	wchar_t chineseName[50];
	bool operator==(City c);
	int nVirus;
	bool hasVirus[3];
	Color colVirus[3];
	bool hasResearch;
	bool hasOutbreak;
	void drawShapes(int type, int centerX, int centerY);
public:
	City();
	Color color;
	bool isPeopleHere[4];
	City getNextCity(int nowCity);
	City& cityByName(const wchar_t* name);
	void outbreak(int color);
	void redraw();
	bool isNeighbor(City& c);
	friend bool checkRemove(int color);
	void addVirus(Color col);
};
#endif
extern City cities[49];*/