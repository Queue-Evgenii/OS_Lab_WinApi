#include "Definitions.h"
#include <algorithm>
#include <string>

int*(_cdecl* countCharInSequence)(char* str);
int* count;

LRESULT CALLBACK MainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HICON Icon, LPCWSTR Name, HINSTANCE hInst, WNDPROC Procedure);

HANDLE NetworkThread;
bool isServerEnabled = false;
char* str = nullptr;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
	WNDCLASS MainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), LoadIcon(NULL, IDI_APPLICATION), L"MainClassSecondApp", hInst, MainProcedure);

	if (!RegisterClassW(&MainClass)) return -1;

	CreateWindow(L"MainClassSecondApp", L"OS* Lab Second App", WS_OVERLAPPEDWINDOW | WS_VISIBLE, WND_POS_X, WND_POS_Y, WND_WIDTH, WND_HEIGHT, NULL, NULL, NULL, NULL);

	HMODULE DllModule = LoadLibrary(_T("OS_Lab_Dll.dll"));
	if (DllModule == nullptr) {
		MessageBox(NULL, _T("Cannot open file."), L"Error", MB_ICONERROR | MB_OK);
		return 1;
	}
	countCharInSequence = (int* (*)(char*))GetProcAddress(DllModule, "countCharInSequence");
	if (countCharInSequence == nullptr) {
		FreeLibrary(DllModule);
		MessageBox(NULL, _T("Procedure countCharInSequence is undefined."), L"Error", MB_ICONERROR | MB_OK);
		return 1;
	}

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

void DrawGraph(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);

	RECT clientRect;
	GetClientRect(hWnd, &clientRect);

	int offset = 40;
	int zoneHeight = (WND_HEIGHT - 120);

	int scaleX = (WND_WIDTH) / LENGTH_IN_ALPHABET;
	int scaleY = (WND_HEIGHT) / 15; 

	// Рисуем ось X
	MoveToEx(hdc, offset, zoneHeight, nullptr);
	LineTo(hdc, (WND_WIDTH), zoneHeight);

	// Рисуем значения на оси X
	for (int i = 0; i < LENGTH_IN_ALPHABET / 2; ++i) {
		wchar_t letter = L'A' + i;
		wchar_t str[2] = { letter, L'\0' };
		TextOut(hdc, i * scaleX + offset, zoneHeight + 10, str, 2);
	}
	for (int i = 0; i < LENGTH_IN_ALPHABET / 2; ++i) {
		wchar_t letter = L'a' + i;
		wchar_t str[2] = { letter, L'\0' };
		TextOut(hdc, ((LENGTH_IN_ALPHABET / 2) + i) * scaleX + offset, zoneHeight + 10, str, 2);
	}

	// Рисуем ось Y
	MoveToEx(hdc, offset, 0, nullptr);
	LineTo(hdc, offset, zoneHeight);

	int max = 0;
	for (int i = 0; i < LENGTH_IN_ALPHABET; ++i) {
		if (max < count[i]) max = count[i];
	}

	// Рисуем значения на оси Y
	for (int i = 0; i <= max; ++i) {
		std::wstring valueText = std::to_wstring(i);
		TextOut(hdc, offset - 10, (zoneHeight) - i * scaleY, valueText.c_str(), valueText.length());
	}

	// Подписи осей
	TextOut(hdc, WND_WIDTH / 2 - 20, zoneHeight + 30, L"Index", 5);
	TextOut(hdc, 10, WND_HEIGHT / 2 - 10, L"Value", 5);

	// Рисуем график
	for (int i = 0; i < LENGTH_IN_ALPHABET - 1; ++i)
	{
		MoveToEx(hdc, i * scaleX + offset, zoneHeight - count[i] * scaleY, nullptr);
		LineTo(hdc, (i + 1) * scaleX + offset, zoneHeight - count[i + 1] * scaleY);
	}

	ReleaseDC(hWnd, hdc);
}


LRESULT CALLBACK MainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	PARAMETERS params = { hWnd, str };
	PAINTSTRUCT ps;
	RECT rect = { 20, 20, WND_WIDTH - 40, WND_HEIGHT - 160 };

	switch (msg)
	{
	case WM_CREATE:
		AddMainMenu(hWnd);
		if (CreateConnection(hWnd, params)) break;
		

		break;
	case WM_PAINT:
		if (count == nullptr) break;
		BeginPaint(hWnd, &ps);

		DrawGraph(hWnd);
		
		EndPaint(hWnd, &ps);
		break;
	case WM_COMMAND:
		switch (wp) {
		case ID_ROOT_MENU_ITEM_AUTHOR:
			MessageBoxA(hWnd, "ITINF-22-2 Student.\nYevhenii Dolhyi", "About Author", MB_OK);
			break;
		case ID_ROOT_MENU_ITEM_PROJECT:
			MessageBoxA(hWnd, "ITINF-22-2 Student`s Lab.\nSubject: Operating Systems\nEnvironment: C++/WinApi\nTCP/IP get application", "About Project", MB_OK);
			break;
		case ID_ROOT_MENU_ITEM_EXIT:
			PostQuitMessage(0);
			break;
		default:

			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
}


WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HICON Icon, LPCWSTR Name, HINSTANCE hInst, WNDPROC Procedure) {
	WNDCLASS NWC = { 0 };

	NWC.hCursor = Cursor;
	NWC.hIcon = Icon;
	NWC.lpszClassName = Name;
	NWC.hbrBackground = BGColor;
	NWC.lpfnWndProc = Procedure;

	return NWC;
}

void CloseNetworkThread() {
	isServerEnabled = false;

	if (NetworkThread != NULL) {
		CloseHandle(NetworkThread);
		NetworkThread = NULL;
	}
}

BOOL CreateConnection(HWND& hWnd, PARAMETERS& params) {
	if (isServerEnabled) {
		MessageBoxA(hWnd, "Server already running.", "Already running", MB_OK | MB_ICONWARNING);
		return 1;
	}
	params.receivingData = new char[GEN_CHARS_STR_ROW_LEN * CHARS_STR_COL_LEN];

	NetworkThread = CreateThread(NULL, 0, NetworkThreadDelegate, &params, 0, NULL);
	return 0;
}

void ProcessReceivedData(HWND& hWnd, char* data) {
	MessageBoxA(NULL, data, "Received data", MB_ICONINFORMATION | MB_OK);
	count = countCharInSequence(data);
	
}