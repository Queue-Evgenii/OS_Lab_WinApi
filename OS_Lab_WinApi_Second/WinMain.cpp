#include "Definitions.h"

int*(_cdecl* countCharInSequence)(char* str);

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


LRESULT CALLBACK MainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	PARAMETERS params = { hWnd, str };
	//HWND text;
	switch (msg)
	{
	case WM_CREATE:
		AddMainMenu(hWnd);
		if (CreateConnection(hWnd, params)) break;
		

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