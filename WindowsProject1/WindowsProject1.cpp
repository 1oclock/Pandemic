#include <windows.h>
#include <stdio.h>
#include <algorithm>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HWND hwnd;
int outbreakTrack;
int infectRate;
int status;
typedef struct _Color {
	wchar_t chineseName[3];
	int num;
	bool isCured;
	bool isEradicated;
	int virusRemain;
}Color;
typedef struct _HandCard {
	int nCityNum;
	int cardType;
}HandCard;
HandCard handCards[100];
HandCard usedHandCards[100];
typedef struct _VirusCard {
	int nCitynum;
}VirusCard;
VirusCard virusCards[100];
VirusCard usedVirusCards[100];
class City{
private:
	int num;
	int neighbor[10];
	wchar_t englishName[10];
	wchar_t chineseName[10];
	bool operator==(City c);
	int nVirus;
	bool hasVirus[3];
	Color colVirus[3];
	bool hasResearch;
	bool hasOutbreak;
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
void epidemic();
class Player {
private:
	int cardToCure;
	City nowCity;
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
	WNDCLASS wndclass;
	MSG msg;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = L"瘟疫危机";
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wndclass);
	hwnd = CreateWindow(L"瘟疫危机", L"瘟疫危机", WS_OVERLAPPEDWINDOW, 50/*initial x*/
		, 50/*initial y*/, 1300/*width*/, 800/*height*/,NULL, NULL, hInstance, NULL);
	DWORD   dwStyle = GetWindowLong(hwnd, GWL_STYLE);
	dwStyle &= ~(WS_SIZEBOX);
	SetWindowLong(hwnd, GWL_STYLE, dwStyle);//*/
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
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
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	switch (message){
	case WM_CREATE:
		return 0;
	case WM_PAINT:
		drawBitmap(L"2.bmp", 1000, 500, 0, 0);
		drawBitmap(L"1.bmp", 100, 200, 500, 100);
		drawBitmap(L"11.bmp", 200, 146, 0, 0);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_LBUTTONDOWN:
		int mouse_x = (int)LOWORD(lParam);
		int mouse_y = (int)HIWORD(lParam);
		wchar_t a[200];
		if (wParam & (WM_LBUTTONDOWN))
			swprintf_s(a, L"pos:%d %d", mouse_x, mouse_y), MessageBox(hwnd,a, L"abc", 0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

#include "cityImp.cpp"
