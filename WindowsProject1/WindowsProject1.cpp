#ifndef MAIN_INCLUDED
#define MAIN_INCLUDED
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <algorithm>
#include <string.h>
#include <vector>

#define set0(x) memset(x,0,sizeof(x))
#define isin(xl,xr,yu,yd) (latestX>=(xl)&&latestX<=(xr)&&latestY>=(yu)&&latestY<=(yd))
#define waitUntilMouseInput\
while (GetMessage(&msg, NULL, 0, 0)){\
	TranslateMessage(&msg);\
	DispatchMessage(&msg);\
	if(isMouseTouched)break;\
}


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HWND hwnd;
int outbreakTrack=0;
int infectRate=0;
int numOfResearch = 0;
int cardNumByRate[7] = { 2,2,2,3,3,4,4 };
int status;
bool isMouseTouched; int latestX, latestY;
bool flag;

typedef struct _Color {
	const wchar_t* chineseName;
	wchar_t englishName;
	int num;
	int cureStatus;//0 uncured,1 cured,2 eraditated
	int virusRemain;//24 in the beginning, -1 to lose, cure to recover
}Color;
Color colors[4];
typedef struct _HandCard {
	int nCityNum;//is special event num when cardtype==1
	int cardType;//0 when normal; 1 when special event; 2 when pandemic,-1 when empty
	_HandCard(int x = -1, int y = -1) :nCityNum(y), cardType(x) {}
	bool operator== (_HandCard cd) { return nCityNum == cd.nCityNum && cardType == cd.cardType; }
}HandCard;
HandCard emptyCard;
std::vector<HandCard> toUseHandCards;
std::vector<HandCard> usedHandCards;
void showUsedHC(); void showUsedVC();
typedef struct _VirusCard {
	int nCitynum;
	_VirusCard(int x=0):nCitynum(x){}
}VirusCard;
std::vector<VirusCard> toUseVirusCards;
std::vector<VirusCard> usedVirusCards;
class City{
private:
	int neighbor[10];
	bool operator==(City c);
	bool hasVirus[3];
	int colVirus[3];
	bool hasOutbreak;
	const wchar_t* englishName;
	int cx, cy;
	mutable int nowin;
	const int num;
	int getNextCity(int nowCity)const;//iterate all the city neighboring,return its num,start from self
public:
	City();
	const wchar_t* chineseName;
	bool hasResearch;
	int color;
	bool isPeopleHere[4];
	int outbreak(int col);//>3 outbreak
	void redraw();
	bool isNeighbor(const City& c)const;
	friend bool checkRemove(int col);//check if virus of color color doesn't exist in the city
	void addVirus(int col=-1);
	bool subVirus();
	bool isClickNear(int clickX, int clickY);
	static void init();
	static void clearOutbreakStatus();
	void showInformation();
	bool confirmSelection(const wchar_t* thingToDo);
};
City cities[49];
void epidemic();
class Player {
protected:
	int cardToCure;
	void discardCard(HandCard& h);
	void addCard(HandCard h);
private:
	char playerType;
public:
	int num;
	int nowCity;
	Player();
	HandCard handCards[7];
	int remainMove;
	void drive(int c);//correctnesses of all the functions here is protected by outside. These are all noexcept.
	void directFlight(HandCard& h);//go to h directed
	void charterFlight(HandCard& h,int to);
	void shuttleFlight(int cWithResearch);
	virtual void buildResearch(HandCard& h);
	void removeResearch();
	void discoverCure(HandCard* hs);
	virtual void treatDisease();
	virtual void /*a.k.a.share knowledge*/ deliverCard(HandCard h, Player& p,bool isGive);
	virtual void skill() = 0;
	void touchCards();
	void drawCards();
	void infectVirus();
};
Player* players[4];
class SkilllessPlayer :public Player {
	virtual void skill() {}//this is a testing skillless player
};
class Actor :public Player {
	virtual void skill() {/*actually overload buildResearch();here to concrete it,below same*/}
	virtual void buildResearch(HandCard& h);
};
class Dispatcher :public Player {
	virtual void skill() {
		//here really need to write something, but I have no idea now
	}
};
class Medic :public Player {
	virtual void skill(){}
	virtual void treatDisease();
};
class Researcher :public Player {
	virtual void skill(){}
	virtual void deliverCard(HandCard& h, Player& p, bool isGive);
};
class Scientist :public Player {
	virtual void skill(){}
	Scientist();//only need to override constructor to change cardToCure to 4
};

void drawBitmap(const wchar_t* fileName, int width, int height, int xUp, int yUp);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	srand(time(0));
#include "colorinit.txt"
	City::init();
	players[0] = new SkilllessPlayer(); players[0]->num = 0;
	for (int i = 1; i <= 48; i++)
		toUseVirusCards.emplace_back(i),
		toUseHandCards.emplace_back(0, i);
	for (int i = 0; i < 5; i++)
		toUseHandCards.emplace_back(1, i);
	std::random_shuffle(toUseVirusCards.begin(), toUseVirusCards.end());
	std::random_shuffle(toUseHandCards.begin(), toUseHandCards.end());
	int difficulty = 4;
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
	int sz = toUseHandCards.size();
	for (int i = 0; i < difficulty; i++) {
		int t = rand() % (sz / difficulty) + (sz / difficulty + 1) * (i);
		toUseHandCards.insert(toUseHandCards.begin()+t, _HandCard(2, 0));
	}
	while (GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (isMouseTouched && latestX > 1200 && latestY > 700)
			isMouseTouched = 0,players[0]->handCards[0].nCityNum=latestY%40, InvalidateRect(hwnd,NULL, false);
		if (isMouseTouched) {
			isMouseTouched = false;
			if (cities[1].isClickNear(latestX, latestY)) {
				usedHandCards.push_back(*toUseHandCards.rbegin());
				toUseHandCards.pop_back();
			}
			if (cities[2].isClickNear(latestX, latestY)) {
				usedVirusCards.push_back(*toUseVirusCards.rbegin());
				toUseVirusCards.pop_back();
			}
			if (isin(879,1007,535,867)) {
				showUsedHC();
			}
			if (isin(870, 1070, 22, 132)) {
				showUsedVC();
			}
			for (int i = 1; i <= 48; i++) {
				break;
				if (cities[i].isClickNear(latestX, latestY)) {
					if (cities[i].confirmSelection(L"坐车"))
						MessageBox(hwnd, L"1", L"1", 0);
					else
						MessageBox(hwnd, L"0", L"0", 0);
					break;
				}
			}
		}
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
		wchar_t c[40];
		drawBitmap(L"Pictures/main_background.bmp", 1200, 700, 0, 0);
		drawBitmap(L"Pictures/hback.bmp", 128, 152, 715, 535);
		system("Pictures/cities/city4/info.txt");
		if (usedHandCards.size()) {
			wsprintf(c, L"Pictures/cities/city%d/h.bmp",(*usedHandCards.rbegin()).nCityNum);
			drawBitmap(c, 128, 152, 879, 535);
		}
		drawBitmap(L"Pictures/vback.bmp", 200, 110, 684, 22);
		if (usedVirusCards.size()) {
			wsprintf(c, L"Pictures/cities/city%d/v.bmp", (*usedVirusCards.rbegin()).nCitynum);
			drawBitmap(c, 200, 110, 870, 22);
		}
		if (outbreakTrack == 0)
			drawBitmap(L"Pictures/O0.bmp", 44, 40, outbreakTrackX[outbreakTrack], outbreakTrackY[outbreakTrack]);
		else if(outbreakTrack%2)
			drawBitmap(L"Pictures/O1.bmp", 44, 40, outbreakTrackX[outbreakTrack], outbreakTrackY[outbreakTrack]);
		else
			drawBitmap(L"Pictures/O2.bmp", 44, 40, outbreakTrackX[outbreakTrack], outbreakTrackY[outbreakTrack]);
		drawBitmap(L"Pictures/infRate.bmp", 42, 37, infectRateX[infectRate], infectRateY[infectRate]);
		players[0]->drawCards();
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
		isMouseTouched = 1; latestX = mouse_x; latestY = mouse_y;
		wchar_t a[200]; 
		if (mouse_y > 700)outbreakTrack++;
		if (mouse_x > 1200)infectRate++;
		if (wParam & (WM_LBUTTONDOWN))
			swprintf_s(a, L"pos:%d %d", mouse_x, mouse_y), MessageBox(hwnd,a, L"abc", 0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

City::City() :num(this-cities){
	int t = this - cities, u = 0, i = 0;
	set0(hasVirus); set0(isPeopleHere);
	hasOutbreak = hasResearch = false;
	hasVirus[0] = hasVirus[1] = hasVirus[2] =false; 
	isPeopleHere[0] = isPeopleHere[1] = isPeopleHere[2] = isPeopleHere[3] = false;
	hasResearch = false;
}
void City::init() {
#include "cityinit.txt"
}
#define setPenAndBrush(i,r,g,b) pen[i] = CreatePen(PS_SOLID, 1, RGB(r,g,b)); brush[i] = CreateSolidBrush(RGB(r,g,b));
void City::redraw() {
	wchar_t str[20];
	PAINTSTRUCT ps;
	HDC hdc = GetDC(hwnd);
	HPEN pen[4]; HBRUSH brush[4];
	//colVirus[0] = colVirus[1] = colVirus[2] = color;
	swprintf_s(str, L"%d", this->num);
	//TextOut(hdc, cx, cy, str, wcslen(str));
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
Player::Player() {
	cardToCure = 5; this->nowCity = 2; playerType = 'n';
}
const wchar_t* event[] = { L"airmove",L"boom",L"foresee",L"fund",L"silent" };
const wchar_t* eventC[] = { L"空运",L"人口快速恢复",L"预测",L"政府拨款",L"寂静的一夜" };
void Player::drawCards() {
	wchar_t nm[100];
	for (int i = 0; i < 7; i++) {
		if (handCards[i].cardType == 0) {
			wsprintf(nm, L"Pictures/cities/city%d/h.bmp", handCards[i].nCityNum); //MessageBox(hwnd, nm, L"anc", 0);
			drawBitmap(nm, 120, 150, 1200 + (i % 2) * 120, (i / 2) * 150);
		}
		else if (handCards[i].cardType == 1) {
			wsprintf(nm, L"Pictures/Event/%s.bmp", event[handCards[i].nCityNum]);// MessageBox(hwnd, nm, L"anc", 0);
			drawBitmap(nm, 120, 150, 1200 + (i % 2) * 120, (i / 2) * 150);
		}
		else return;
	}
	return;
}
bool City::operator== (City c) {
	return num == c.num;
}
int City::getNextCity(int nowCity)const{
	if (nowCity == num) {
		nowin = 0;
		return neighbor[0];
	}
	if (nowCity == 0) {
		nowin = 0; return 0;
	}
	++nowin;
	return neighbor[nowin];
}
bool City::isNeighbor(const City& c)const {
	int t = c.num;
	while (t = c.getNextCity(t)) {
		if (cities[t] == *this)return true;
	}
	return false;
}
bool checkRemove(int col) {
	for (int i = 1; i <= 48; i++)
		for (int j = 0; j < 3; j++)
			if (cities[i].hasVirus[j] && cities[i].colVirus[j] == col)
				return false;
	return true;
}
void City::addVirus(int col) {
	if (col = -1)col = color;
	if (colors[col].cureStatus == 2)return;
	for (int i = 0; i < 3; i++) {
		if (!hasVirus[i]) {
			hasVirus[i] = true;
			colVirus[i] = col;
			return;
		}
	}
	hasOutbreak = true;
	outbreakTrack += outbreak(col);
	if (outbreakTrack > 8)
		outbreakTrack = 8;
}
int City::outbreak(int col) {
	if (hasOutbreak)return 0;
	int t = this->num;
	while (t = getNextCity(t)) {
		cities[t].addVirus(t);
	}
	return 1;
}
bool City::subVirus() {
	if (colors[color].cureStatus == 1) {
		set0(hasVirus); set0(colVirus); return true;
	}
	for(int i=2;i>-1;i--)
		if (hasVirus[i]) {
			hasVirus[i] = 0; return true;
		}
	return false;
}
void City::clearOutbreakStatus() {
	for (int i = 1; i <= 48; i++)
		cities[i].hasOutbreak = false;
}
bool City::isClickNear(int clickX, int clickY) {
	//wchar_t info[20]; wsprintf(info, "%d %d %d %d", clickX, clickY, cx, cy);MessageBox(hwnd, info, "abc", 0);
	return clickX <= cx + 20 && clickY <= cy + 20 && clickX >= cx - 20 && clickY >= cy - 20;
}
void City::showInformation() {
	wchar_t info[200], title[20]; int nvirus = 0+hasVirus[0] + hasVirus[1] + hasVirus[2],
		nplayer=0+isPeopleHere[0]+isPeopleHere[1]+isPeopleHere[2]+isPeopleHere[3];
	wsprintf(info, L"%s是个%s色城市，%s研究所\n有%d个病毒，%d人在此", chineseName, colors[color].chineseName, hasResearch ? L"有" : L"无",nvirus, nplayer);
	wsprintf(title, L"%s信息", chineseName);
	MessageBox(hwnd, info, title, 0);
}
bool City::confirmSelection(const wchar_t* thingToDo) {
	wchar_t info[200], title[20];
	wsprintf(info, L"你确定要对%s执行%s操作吗？", chineseName, thingToDo);
	wsprintf(title, L"%s操作确认", thingToDo);
	return MessageBox(hwnd, info, title, MB_YESNO | MB_ICONQUESTION) == IDYES;
}
void showUsedHC() {
	wchar_t info[1000] = L"已经弃掉的手牌有：\n"; int t = 0;
	for (auto it : usedHandCards) {
		if (it.cardType == 0)
			wcscat(info, cities[it.nCityNum].chineseName);
		else if (it.cardType == 1)
			wcscat(info, eventC[it.nCityNum]);
		else
			wcscat(info,L"蔓延");
		if (++t % 4 == 0)wcscat(info, L"\n");
		else wcscat(info, L" ");
	}
	MessageBox(hwnd, info, L"已经弃掉的手牌信息", 0);
}
void showUsedVC() {
	wchar_t info[1000] = L"已经弃掉的病毒牌有：\n"; int t = 0;
	for (auto it : usedVirusCards) {
		wcscat(info, cities[it.nCitynum].chineseName);
		if (++t % 4 == 0)wcscat(info, L"\n");
		else wcscat(info, L" ");
	}
	MessageBox(hwnd, info, L"已经弃掉的病毒牌信息", 0);
}
void Player::discardCard(HandCard& h) {
	for (int i = 0; i < 7; i++) {
		if (handCards[i] == h) {
			for (int j = 6; j > i; j--)
				handCards[j] = handCards[j - 1];
			handCards[6].cardType = -1;
			break;
		}
	}
}
void Player::buildResearch(HandCard& h){
	cities[nowCity].hasResearch = true;
	numOfResearch++;
	discardCard(h);
	remainMove--;
}
void Player::drive(int c) {
	cities[nowCity].isPeopleHere[num] = false;
	nowCity = c;
	cities[nowCity].isPeopleHere[num] = true;
	remainMove--;
}
void Player::directFlight(HandCard& h) {
	drive(h.nCityNum);
	discardCard(h);
}
void Player::charterFlight(HandCard& h, int to) {
	drive(to);
	discardCard(h);
}
void Player::shuttleFlight(int cWithResearch) {
	drive(cWithResearch);
}
void Player::treatDisease(){
	remainMove-=cities[nowCity].subVirus();
}
void Player::addCard(HandCard h) {
	if (~handCards[6].cardType) {
		//MessageBox
		//wait for mouse input
		//delete one
	}
	for (int i = 0; i < 7; i++)
		if (~handCards[i].cardType) {
			handCards[i] = h;
			return;
		}
}
void Player::deliverCard(HandCard h, Player& p, bool isGive){
	if (isGive) {
		discardCard(h);
		p.addCard(h);
	}
	else {
		p.discardCard(h);
		addCard(h);
	}
}
void Player::removeResearch() {
	cities[nowCity].hasResearch = false;
}
void Player::infectVirus() {
	wchar_t info[200]=L"在";
	for (int i = 0; i < infectRate; i++) {
		auto t = *toUseVirusCards.rbegin();
		cities[t.nCitynum].addVirus();
		wcscat(info, cities[t.nCitynum].chineseName);
		wcscat(info, L" ");
		usedVirusCards.push_back(t);
		toUseVirusCards.pop_back();
	}
	wcscat(info, L"发生了病毒传播");
	MessageBox(hwnd, info, L"传播", 0);
}
void Player::touchCards() {
	for (int i = 0; i < 2; i++) {
		auto t = *toUseHandCards.rbegin();
		if (t.cardType == 2)epidemic();
		else {
			addCard(t);
			usedHandCards.push_back(t);
			usedVirusCards.pop_back();
		}
	}
}
void Player::discoverCure(HandCard* h) {
	int co = cities[h->nCityNum].color;
	for (int i = 0; i < cardToCure; i++) {
		discardCard(h[i]);
	}
	colors[co].cureStatus = 1;
	remainMove--;
}
#endif