// 1412346.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1412346.h"
#include <WindowsX.h>
#include <commctrl.h>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <mmsystem.h>
#include <string>
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
int maxPoint = 0;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);
void OnDestroy(HWND hwnd);
BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
void btnStart_Onclick();
void GenerateQiz();
void UpdateInfor();
void StartAgain();
void LoadMaxPoint();
bool WriteMaxPoint(int currentPoint);

int timer = 1, timeRemain = 0;
int _level = 1;
int result, dummyNumber, delta = 1, answerID, numPassed = 0;
HWND txvQiz;
HWND txvTimeRemain;
HWND txvTime;
HWND txvFunny;
HWND txvNumPassed;
HWND btnStart;
HWND txvBest;
HWND btnAnswer[2];

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY1412346, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1412346));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY1412346);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON));


	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   
   ::SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE)&~WS_SIZEBOX&~WS_MAXIMIZEBOX);


   if (!hWnd)
   {
      return FALSE;
   }
   
   SetWindowPos(hWnd, NULL, 200, 100, 650, 500, 0);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WCHAR *buff = new WCHAR[50];
	// do something
	switch (message)
	{
		
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);


	case WM_TIMER:
		switch (wParam)
		{
		case ID_TIME:
			wsprintf(buff, L"Time Elapsed: %d", timer++);
			SetWindowText(txvTime, buff);
			delete buff;
			break;

		case ID_TIMEREMAIN:
			int deadTime = 5 - ++timeRemain;
			wsprintf(buff, L"%d", deadTime);

			SetWindowText(txvTimeRemain, buff);

			if (deadTime <= 0)
			{
				WCHAR *buffer = new WCHAR[100];
				wsprintf(buff, L"Time Elapsed: %d", timer++);
				SetWindowText(txvTime, buff);

				bool flag = WriteMaxPoint(numPassed);

				if (!flag)
					wsprintf(buffer, L"Game Over, Your Score: %d - Level: %d - Time: %d (seconds) - BEST SCORE: %d", numPassed, _level, timer - 1, maxPoint);
				else
					wsprintf(buffer, L"Game Over, Your Score: %d - Level: %d - Time: %d (seconds) - WOW! NEW RECORD!!!", numPassed, _level, timer - 1);

				KillTimer(hWnd, ID_TIME);
				KillTimer(hWnd, ID_TIMEREMAIN);
				MessageBox(hWnd, buffer, L"Game Over", 0);
				delete buffer;

				StartAgain();
			}
			delete buff;
			break;
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}



BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	LOGFONT logFont;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(logFont), &logFont);

	HFONT hFont = CreateFont(60, 0, 0, 0, FW_DONTCARE | FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, \
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, \
		DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

	//txvTitle LEFT TOP WIDTH HEIGHT
	HWND hwnd = CreateWindowEx(0, L"STATIC", L"Brain War", WS_CHILD | WS_VISIBLE | SS_LEFT, 220, 20, 220, 50, hWnd, NULL, hInst, NULL);
	SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

	hFont = CreateFont(40, 0, 0, 0, FW_DONTCARE | FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, \
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, \
		DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

	btnStart = CreateWindowEx(0, L"BUTTON", L"Start",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 220, 250, 210, 100, hWnd, (HMENU)IDC_STARTBUTTON, hInst, NULL);
	SendMessage(btnStart, WM_SETFONT, WPARAM(hFont), TRUE);

	hFont = CreateFont(20, 0, 0, 0, FW_DONTCARE | FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, \
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, \
		DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

	txvFunny = CreateWindowEx(0, L"STATIC", L"You don't know what you are getting into...",
		WS_CHILD | WS_VISIBLE | SS_LEFT, 170, 120, 350, 40, hWnd, NULL, hInst, NULL);
	SendMessage(txvFunny, WM_SETFONT, WPARAM(hFont), TRUE);

	hFont = CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, \
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, \
		DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

	txvQiz = CreateWindowEx(0, L"STATIC", L"4 + 5 = ?",
		WS_CHILD | WS_VISIBLE | SS_LEFT, 240, 120, 250, 40, hWnd, NULL, hInst, NULL);
	SendMessage(txvQiz, WM_SETFONT, WPARAM(hFont), TRUE);
	ShowWindow(txvQiz, SW_HIDE);

	hFont = CreateFont(20, 0, 0, 0, FW_DONTCARE | FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, \
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, \
		DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

	txvNumPassed = CreateWindowEx(0, L"STATIC", L"Score: 0 - Level: 1",
		WS_CHILD | WS_VISIBLE | SS_LEFT, 20, 50, 150, 40, hWnd, NULL, hInst, NULL);
	SendMessage(txvNumPassed, WM_SETFONT, WPARAM(hFont), TRUE);
	ShowWindow(txvNumPassed, SW_HIDE);

	txvTime = CreateWindowEx(0, L"STATIC", L"Time Elapsed: 0",
		WS_CHILD | WS_VISIBLE | SS_LEFT, 500, 50, 150, 40, hWnd, NULL, hInst, NULL);
	SendMessage(txvTime, WM_SETFONT, WPARAM(hFont), TRUE);
	ShowWindow(txvTime, SW_HIDE);

	txvBest = CreateWindowEx(0, L"STATIC", L"Best Score: ",
		WS_CHILD | WS_VISIBLE | SS_LEFT, 30, 90, 150, 40, hWnd, NULL, hInst, NULL);
	SendMessage(txvBest, WM_SETFONT, WPARAM(hFont), TRUE);
	ShowWindow(txvBest, SW_HIDE);


	hFont = CreateFont(64, 0, 0, 0, FW_DONTCARE | FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, \
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, \
		DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

	txvTimeRemain = CreateWindowEx(0, L"STATIC", L"",
		WS_CHILD | WS_VISIBLE | SS_LEFT, 290, 200, 60, 60, hWnd, NULL, hInst, NULL);
	SendMessage(txvTimeRemain, WM_SETFONT, WPARAM(hFont), TRUE);
	ShowWindow(txvTimeRemain, SW_HIDE);


	hFont = CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, \
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, \
		DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
	
	btnAnswer[0] = CreateWindowEx(0, L"BUTTON", L"Answer 1",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 50, 300, 250, 80, hWnd, (HMENU)ID_ANSWERBUTTON1, hInst, NULL);
	SendMessage(btnAnswer[0], WM_SETFONT, WPARAM(hFont), TRUE);
	ShowWindow(btnAnswer[0], SW_HIDE);

	btnAnswer[1] = CreateWindowEx(0, L"BUTTON", L"Answer 2",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 330, 300, 250, 80, hWnd, (HMENU)ID_ANSWERBUTTON2, hInst, NULL);
	SendMessage(btnAnswer[1], WM_SETFONT, WPARAM(hFont), TRUE);
	ShowWindow(btnAnswer[1], SW_HIDE);

	return TRUE;
}

int Random_ab(int a, int b)
{
	srand(time(0));
	return a + rand() % (b - a + 1);
}

void OnDestroy(HWND hwnd)
{
	PostQuitMessage(0);
}

void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{

	switch (id)
	{
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;

	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;

	case ID_ANSWERBUTTON1:
		if (answerID == 0)//True
		{
			//Sleep(500);
			timeRemain = 0;
			delta += 5;
			numPassed++;
			if (numPassed % 5 == 0)
				_level++;
			UpdateInfor();
			GenerateQiz();
			KillTimer(hWnd, ID_TIMEREMAIN);
			SetWindowText(txvTimeRemain, L"5");
			SetTimer(hWnd, ID_TIMEREMAIN, 1000, TIMERPROC(NULL));
		}
		else
		{
			KillTimer(hWnd, ID_TIME);
			KillTimer(hWnd, ID_TIMEREMAIN);
			WCHAR *buffer = new WCHAR[100];
			bool flag = WriteMaxPoint(numPassed);

			if (!flag)
				wsprintf(buffer, L"Game Over, Your Score: %d - Level: %d - Time: %d (seconds) - BEST SCORE: %d"
				, numPassed, _level, timer - 1, maxPoint);
			else
				wsprintf(buffer, L"Game Over, Your Score: %d - Level: %d - Time: %d (seconds) - WOW! NEW RECORD!!!"
				, numPassed, _level, timer - 1);

			MessageBox(hWnd, buffer, L"Have you got a Tha Thoo?", 0);
			delete buffer;
			StartAgain();
		}
		break;

	case ID_ANSWERBUTTON2:
		if (answerID == 1)
		{
			//Sleep(500);
			timeRemain = 0;
			delta += 5;
			numPassed++;
			if (numPassed % 5 == 0)
				_level++;
			UpdateInfor();
			GenerateQiz();
			KillTimer(hWnd, ID_TIMEREMAIN);
			SetWindowText(txvTimeRemain, L"5");
			SetTimer(hWnd, ID_TIMEREMAIN, 1000, TIMERPROC(NULL));
		}
		else
		{
			KillTimer(hWnd, ID_TIME);
			KillTimer(hWnd, ID_TIMEREMAIN);

			WCHAR *buffer = new WCHAR[100];

			bool flag = WriteMaxPoint(numPassed);

			if (!flag)
				wsprintf(buffer, L"Game Over, Your Score: %d - Level: %d - Time: %d (seconds) - BEST SCORE: %d"
				, numPassed, _level, timer - 1, maxPoint);
			else
				wsprintf(buffer, L"Game Over, Your Score: %d - Level: %d - Time: %d (seconds) - WOW! NEW RECORD!!!"
				, numPassed, _level, timer - 1);

			MessageBox(hWnd, buffer, L"Have you got a Tha Thoo?", 0);
			delete buffer;
			StartAgain();
		}
		break;

	case IDC_STARTBUTTON:
		btnStart_Onclick();
		GenerateQiz();
		int checkCreateTimer = SetTimer(hWnd, ID_TIME, 1000, TIMERPROC(NULL));
		SetTimer(hWnd, ID_TIMEREMAIN, 1000, TIMERPROC(NULL));
		SetWindowText(txvTime, L"Time Elapsed: 0");
		SetWindowText(txvTimeRemain, L"5");
		SetWindowText(txvNumPassed, L"Score: 0 - Level: 1");
		break;
	}
}

void btnStart_Onclick()
{
	PlaySound(TEXT("start.wav"), NULL, SND_ALIAS);
	ShowWindow(btnStart, SW_HIDE);
	ShowWindow(txvFunny, SW_HIDE);
	ShowWindow(txvNumPassed, SW_SHOW);
	ShowWindow(txvQiz, SW_SHOW);
	ShowWindow(txvTime, SW_SHOW);
	ShowWindow(txvTimeRemain, SW_SHOW);
	ShowWindow(btnAnswer[0], SW_SHOW);
	ShowWindow(btnAnswer[1], SW_SHOW);
	ShowWindow(txvBest, SW_SHOW);

	timer = 1;
	timeRemain = 0;
	numPassed = 0;
	_level = 1;
	delta = 1;
	LoadMaxPoint();

	WCHAR *buff = new WCHAR[100];

	wsprintf(buff, L"Best Score: %d", maxPoint);
	SetWindowText(txvBest, buff);

	delete buff;
}

void StartAgain()
{
	ShowWindow(btnStart, SW_SHOW);
	ShowWindow(txvFunny, SW_SHOW);
	ShowWindow(txvNumPassed, SW_HIDE);
	ShowWindow(txvQiz, SW_HIDE);
	ShowWindow(txvTime, SW_HIDE);
	ShowWindow(txvTimeRemain, SW_HIDE);
	ShowWindow(btnAnswer[0], SW_HIDE);
	ShowWindow(btnAnswer[1], SW_HIDE);
	ShowWindow(txvBest, SW_HIDE);
}

void GenerateQiz()
{
	int number1, number2;
	WCHAR* buffQuiz = new WCHAR[MAX_LOADSTRING];
	WCHAR* buffAns = new WCHAR[MAX_LOADSTRING / 10];
	WCHAR sign;

	number1 = Random_ab(_level * delta * -1, _level * delta * 3);
	number2 = Random_ab(_level * delta / 2 * -1, _level * delta * 3 + _level * 2);

	if ((Random_ab(3, 4)) == 3)
	{
		sign = L'-';
		result = number1 - number2;
	}
	else
	{
		sign = L'+';
		result = number1 + number2;
	}

	dummyNumber = result;

	while (dummyNumber == result)
	{
		dummyNumber = Random_ab(result - delta - _level * 2, result + delta + _level*2);
	}


	wsprintf(buffQuiz, L"%d %c %d = ?", number1, sign, number2);
	SetWindowText(txvQiz, buffQuiz);

	if ((Random_ab(1, 10) ) <= 5)
	{
		answerID = 0;
		wsprintf(buffAns, L"%d", result);
		SetWindowText(btnAnswer[0], buffAns);
		wsprintf(buffAns, L"%d", dummyNumber);
		SetWindowText(btnAnswer[1], buffAns);
	}
	else
	{
		answerID = 1;
		wsprintf(buffAns, L"%d", result);
		SetWindowText(btnAnswer[1], buffAns);
		wsprintf(buffAns, L"%d", dummyNumber);
		SetWindowText(btnAnswer[0], buffAns);
	}
	
	delete buffAns;
	delete buffQuiz;
}

void UpdateInfor()
{
	WCHAR* buff;
	buff = new WCHAR[50];
	wsprintf(buff, L"Score: %d - Level: %d", numPassed, _level);
	SetWindowText(txvNumPassed, buff);
	delete buff;
}

void LoadMaxPoint()
{
	FILE *file;
	file = fopen("infor.dat", "rb");

	if (file == NULL)
	{
		file = fopen("infor.dat", "wb");
		fclose(file);
		return;
	}

	fread(&maxPoint, 1, 1, file);
	fclose(file);
}

bool WriteMaxPoint(int currentPoint)
{
	if (currentPoint <= maxPoint)
		return false;

	maxPoint = currentPoint;
	FILE *file;
	file = fopen("infor.dat", "wb");
	
	fwrite(&maxPoint, 4, 1, file);
	fclose(file);
	return true;
}