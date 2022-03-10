#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#pragma warning(disable:4996)

//----- console color -----
#define BLACK	0x0
#define BLUE	0x1
#define GREEN	0x2
#define CYAN	0x3
#define RED		0x4
#define MAGENTA	0x5
#define BROWN	0x6
#define WHITE	0x7
//----- console color -----

int base_display(HWND hWind, HDC hDC);
int ConsoleColorTest(HANDLE hConsole);
int gotoxy(int x, int y);
int gotoxySprint(int x, int y, char* chText);
int gotoxySprintColor(int x, int y, char* chText, WORD wColor);

int main(void)
{
	//no change
	HWND hWind;
	HDC hDC;
	HANDLE hConsole;
	char chText[1024];

	int x, y;
	char chComm;

	hWind = GetForegroundWindow();
	hDC = GetDC(hWind);
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	do {
		printf("---command:");

		scanf(" %c", &chComm);
		switch (chComm) {
		case 'a':
			base_display(hWind, hDC);
			break;
		case 'b': // 콘솔 색상 테스트(배경,전경)
			ConsoleColorTest(hConsole);
			break;
		case 'c': // 콘솔 지우기
			system("cls");
			break;
		case 'd': // 콘솔 색상 변경(배경:밝은힌색, 전경:어두운검은색)
			system("color f0");
			break;
		case 'e': // 콘솔창 크기 변경
			system("mode con:cols=120 lines=40");
			break;
		case 'f': // x,y cursor move and print
			x = 15; y = 12;
			gotoxy(x, y);
			printf("---cursor move test---");
			break;
		case 'g': // x,y cursor move and print
			x = 15; y = 12;
			sprintf(chText, "---cursor move and string---");
			gotoxySprint(x, y, chText);
			break;
		case 'h': // x,y cursor move and print, color
			x = 15; y = 12;
			sprintf(chText, "---cursor move and string, color---");
			//gotoxySprintColor(x, y, chText, ((BROWN << 4) + RED));
			//gotoxySprintColor(x, y, chText, ((CYAN << 4) + RED));
			gotoxySprintColor(x, y, chText, ((WHITE << 4) + RED));
			break;
		default:
			break;
		}
	} while ('x' != chComm);

	ReleaseDC(hWind, hDC);
	CloseHandle(hConsole);

	return(0);
}

int base_display(HWND hWind, HDC hDC)
{
	int x, y;
	int i;

	x = 150; y = 170;
	MoveToEx(hDC, x, y, NULL);
	x = 150; y = 250;
	LineTo(hDC, x, y);

	Rectangle(hDC, 100, 100, 200, 150);

	Ellipse(hDC, 200, 200, 300, 300);

	x = 300; y = 70;
	for (i = 30; i < 250; i++) {
		SetPixel(hDC, (x + i), (y + i), RGB(255, 0, 0));
	}

	return(0);
}

int ConsoleColorTest(HANDLE hConsole)
{
	int i, m;

	for (i = 0; i < 16; i++) { // 배경색
		for (m = 0; m < 8; m++) { // 어두운 전경색
			SetConsoleTextAttribute(hConsole, ((i << 4) + m));
			printf("(test:%02x)", ((i << 4) + m));
		}
		printf("\n");
		for (m = 8; m < 16; m++) { // 밝은 전경색
			SetConsoleTextAttribute(hConsole, ((i << 4) + m));
			printf("(test:%02x)", ((i << 4) + m));
		}
		printf("\n");
	}

	return(0);
}

int gotoxy(int x, int y)
{

	COORD Pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);

	return(0);
}

int gotoxySprint(int x, int y, char* chText)
{
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo);

	COORD curPos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), curPos);
	printf("%s", chText);

	COORD prePos = { bufferInfo.dwCursorPosition.X, bufferInfo.dwCursorPosition.Y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), prePos);

	return(0);
}

int gotoxySprintColor(int x, int y, char* chText, WORD wColor)
{
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);

	COORD curPos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), curPos);
	printf("%s", chText);

	COORD prePos = { bufferInfo.dwCursorPosition.X, bufferInfo.dwCursorPosition.Y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), prePos);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), bufferInfo.wAttributes);

	return(0);
}

