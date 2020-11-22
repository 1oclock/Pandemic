#ifndef MAIN_INCLUDED
#define MAIN_INCLUDED
#include <windows.h>
#include <stdio.h>
#include <algorithm>
#include <string.h>

#define set0(x) memset(x,0,sizeof(x))

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HWND hwnd;
int outbreakTrack=0; void drawOutbreakTrack();
int infectRate=0; void drawInfectRate();
int cardNumByRate[7] = { 2,2,2,3,3,4,4 };
int status;
bool isMouseTouched; int mousex, mousey;
bool flag;

typedef struct _Color {
	const wchar_t* chineseName;
	wchar_t englishName;
	int num;
	int cureStatus;
	int virusRemain;
}Color;
Color colors[4];
typedef struct _HandCard {
	int nCityNum;
	int cardType;
	static void drawHandCardArea();
}HandCard;
HandCard handCards[100];
HandCard usedHandCards[100];
typedef struct _VirusCard {
	int nCitynum;
	static void drawVirusCardArea();
}VirusCard;
VirusCard virusCards[100];
VirusCard usedVirusCards[100];
class City{
private:
	int num;
	int neighbor[10];
	bool operator==(City c);
	int nVirus;
	bool hasVirus[3];
	int colVirus[3];
	bool hasResearch;
	bool hasOutbreak;
	void drawShapes(int type, int centerX, int centerY);
public:
	const wchar_t* englishName;
	const wchar_t* chineseName;
	int cx, cy;
	City();
	int color;
	bool isPeopleHere[4];
	City getNextCity(int nowCity);
	City& cityByName(const wchar_t* name);
	void outbreak(int color);
	void redraw();
	bool isNeighbor(City& c);
	friend bool checkRemove(int color);
	void addVirus(Color col);
};
City cities[49];
void epidemic();
class Player {
private:
	int cardToCure;
	City nowCity;
	int people;
public:
	HandCard handCards[7];
	int remainMove;
	void drive(City c);
	void directFlight(HandCard& h);
	void charterFlight(HandCard& h);
	void shuttleFlight(City cWithResearch);
	void buildResearch(HandCard& h);
	void removeResearch();
	void discoverCure(HandCard* hs);
	virtual void treatDisease();
	virtual void /*a.k.a.share knowledge*/ deliverCard(HandCard& h, Player& p,bool isGive);
	virtual void skill() = 0;
	void drawCards();
	void infectVirus();
};
void drawBitmap(const wchar_t* fileName, int width, int height, int xUp, int yUp);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
#include "colorinit.txt"
#include "cityinit.txt"
	flag = 0;
	WNDCLASS wndclass;
	MSG msg;
	wndclass.cbClsExtra = 0;wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = L"瘟疫危机";
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wndclass);
	hwnd = CreateWindow(L"瘟疫危机", L"瘟疫危机", WS_OVERLAPPEDWINDOW, 0/*initial x*/
		, 0/*initial y*/, 1500/*width*/, 800/*height*/,NULL, NULL, hInstance, NULL);
	DWORD   dwStyle = GetWindowLong(hwnd, GWL_STYLE);
	dwStyle &= ~(WS_SIZEBOX);
	SetWindowLong(hwnd, GWL_STYLE, dwStyle);//*/
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		UpdateWindow(hwnd);
	}
	return msg.wParam;
}
void drawBitmap(const wchar_t* fileName, int width, int height, int xUp, int yUp) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps), dc = GetDC(hwnd),H1;
	RECT rect; GetClientRect(hwnd, &rect);
	HBITMAP map = (HBITMAP)LoadImage(NULL, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	H1 = CreateCompatibleDC(dc);
	SelectObject(H1, map);
	BitBlt(dc, xUp, yUp, width, height, H1, 0, 0, SRCCOPY);
	EndPaint(hwnd, &ps);
}
const int outbreakTrackX[9] = { 57,96,57,96,57,96,57,96,57 };
const int outbreakTrackY[9] = { 407,438,467,498,530,560,590,620,650 };
const int infectRateX[8] = { 709,757,805,857,903,956,1005 };
const int infectRateY[8] = { 140,155,160,165,159,154,141 };
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	switch (message){
	case WM_CREATE:
		return 0;
	case WM_PAINT:
		drawBitmap(L"Pictures/main_background.bmp", 1200, 700, 0, 0);
		drawBitmap(L"Pictures/hback.bmp", 128, 152, 715, 535);
		system("Pictures/cities/city4/info.txt");
		drawBitmap(L"Pictures/cities/city2/h.bmp", 128, 152, 879, 535);
		drawBitmap(L"Pictures/vback.bmp", 200, 110, 684, 22);
		drawBitmap(L"Pictures/cities/city3/v.bmp", 200, 110, 870, 22);
		if (outbreakTrack == 0)
			drawBitmap(L"Pictures/O0.bmp", 44, 40, outbreakTrackX[outbreakTrack], outbreakTrackY[outbreakTrack]);
		else if(outbreakTrack%2)
			drawBitmap(L"Pictures/O1.bmp", 44, 40, outbreakTrackX[outbreakTrack], outbreakTrackY[outbreakTrack]);
		else
			drawBitmap(L"Pictures/O2.bmp", 44, 40, outbreakTrackX[outbreakTrack], outbreakTrackY[outbreakTrack]);
		drawBitmap(L"Pictures/infRate.bmp", 42, 37, infectRateX[infectRate], infectRateY[infectRate]);
		for (int i = 0; i < 4; i++) {
			wchar_t nm[30]; swprintf_s(nm, L"Pictures/cureEff/%c%d.bmp", colors[i].englishName, colors[i].cureStatus);
			//MessageBox(hwnd, nm, L"abc", 0);
			drawBitmap(nm, 40, 40, 355 + 85 * i, 642 - (colors[i].cureStatus != 0) * 42);
		}
		for (int i = 1; i <= 48; i++)
			cities[i].redraw();
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_LBUTTONDOWN:
		int mouse_x = (int)LOWORD(lParam);
		int mouse_y = (int)HIWORD(lParam);
		wchar_t a[200]; 
		if (mouse_y > 700)outbreakTrack++;
		if (mouse_x > 1200)infectRate++;
		if (wParam & (WM_LBUTTONDOWN))
			swprintf_s(a, L"pos:%d %d", mouse_x, mouse_y), MessageBox(hwnd,a, L"abc", 0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

City::City() {
	int t = this - cities, u = 0, i = 0;
	num = t;
	nVirus = 0;
	set0(hasVirus); set0(isPeopleHere);
	hasOutbreak = hasResearch = false;
	hasVirus[0] = hasVirus[1] = hasVirus[2] =false; 
	isPeopleHere[0] = isPeopleHere[1] = isPeopleHere[2] = isPeopleHere[3] = false;
	hasResearch = false;
}
#define setPenAndBrush(i,r,g,b) pen[i] = CreatePen(PS_SOLID, 1, RGB(r,g,b)); brush[i] = CreateSolidBrush(RGB(r,g,b));
void City::redraw() {
	wchar_t str[20];
	PAINTSTRUCT ps;
	HDC hdc = GetDC(hwnd);
	HPEN pen[4]; HBRUSH brush[4];
	colVirus[0] = colVirus[1] = colVirus[2] = color;
	swprintf_s(str, L"%d %d %d", colVirus[0], colVirus[1], colVirus[2]);
	//TextOut(hdc, cx, cy, str, 10);
	setPenAndBrush(0, 0, 0, 250); pen[1] = CreatePen(PS_SOLID, 1, RGB(128,128,128)); brush[1] = CreateSolidBrush(RGB(255,255,0));
	setPenAndBrush(2, 0, 0, 0); setPenAndBrush(3, 250, 0, 0);
	if (this->hasVirus[0]) {
		SelectObject(hdc,pen[colVirus[0]]);
		SelectObject(hdc, brush[colVirus[0]]);
		Rectangle(hdc, cx - 5, cy - 15, cx + 5, cy - 5);
	}
	if (this->hasVirus[1]) {
		SelectObject(hdc, pen[colVirus[1]]);
		SelectObject(hdc, brush[colVirus[1]]);
		Rectangle(hdc, cx - 5, cy + 5, cx + 5, cy + 15);
	}
	if (this->hasVirus[2]) {
		SelectObject(hdc, pen[colVirus[2]]);
		SelectObject(hdc, brush[colVirus[2]]);
		Rectangle(hdc, cx - 15, cy - 5, cx - 5, cy + 5);
	}
	setPenAndBrush(0, 255, 255, 255); setPenAndBrush(1, 0, 255, 0); setPenAndBrush(2, 255, 0, 255); setPenAndBrush(3, 0, 255, 255);
	if (this->isPeopleHere[0]) {
		SelectObject(hdc, pen[0]); SelectObject(hdc, brush[0]);
		Ellipse(hdc, cx - 15, cy - 15, cx - 5, cy - 5);
	}
	if (this->isPeopleHere[1]) {
		SelectObject(hdc, pen[1]); SelectObject(hdc, brush[1]);
		Ellipse(hdc, cx + 5, cy - 15, cx + 15, cy - 5);
	}
	if (this->isPeopleHere[2]) {
		SelectObject(hdc, pen[2]); SelectObject(hdc, brush[2]);
		Ellipse(hdc, cx - 15, cy + 5, cx - 5, cy + 15);
	}
	if (this->isPeopleHere[3]) {
		SelectObject(hdc, pen[3]); SelectObject(hdc, brush[3]);
		Ellipse(hdc, cx + 5, cy + 5, cx + 15, cy + 15);
	}
	setPenAndBrush(0, 255, 128, 0);
#define setxy(i,ix,iy) pentagon[i].x=cx + ix;pentagon[i].y=cy + iy;
	if (this->hasResearch) {
		SelectObject(hdc, pen[0]); SelectObject(hdc, brush[0]);
		POINT pentagon[5];
		setxy(0, 10, -5); setxy(1, 5, 0); setxy(2, 5, 5); setxy(3, 15, 5); setxy(4, 15, 0);
		Polygon(hdc, pentagon, 5);
	}
	DeleteObject(pen[0]); DeleteObject(brush[0]); DeleteObject(pen[1]); DeleteObject(brush[1]); 
	DeleteObject(pen[2]); DeleteObject(brush[2]); DeleteObject(pen[3]); DeleteObject(brush[3]);
	ReleaseDC(hwnd, hdc);
}//*/
#endif
#ifndef CITYIMP_INCLUDED
//# include "cityImp.h"
#define CITYIMP_INCLUDED
#endif
#include "drawImp.cpp"
#include "playerImp.cpp"