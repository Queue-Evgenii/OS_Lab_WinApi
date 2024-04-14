#include "Definitions.h"

HWND DefaultCharStrTextBox = NULL;
HWND SortedCharStrTextBox = NULL;
char** strMatr = NULL;
char* str = NULL;

HANDLE SubThread, NetworkThread;
bool isServerEnabled = false;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
	WNDCLASS MainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), LoadIcon(NULL, IDI_APPLICATION), L"MainClass", hInst, MainProcedure);

	if (!RegisterClassW(&MainClass)) return -1;

	CreateWindow(L"MainClass", L"OS* Lab", WS_OVERLAPPEDWINDOW | WS_VISIBLE, WND_POS_X, WND_POS_Y, WND_WIDTH, WND_HEIGHT, NULL, NULL, NULL, NULL);
	
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}


LRESULT CALLBACK MainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg)
	{
	case WM_CREATE:
		AddMainMenu(hWnd);
		AddStringGeneratorInterface(hWnd, DefaultCharStrTextBox, SortedCharStrTextBox);
		AddTCPInterface(hWnd);

		break;
	case WM_COMMAND:
		switch (wp) {
		case ID_ROOT_MENU_ITEM_AUTHOR:
			MessageBoxA(hWnd, "ITINF-22-2 Student.\nYevhenii Dolhyi", "About Author", MB_OK);
			break;
		case ID_ROOT_MENU_ITEM_PROJECT:
			MessageBoxA(hWnd, "ITINF-22-2 Student`s Lab.\nSubject: Operating Systems\nEnvironment: C++/WinApi", "About Project", MB_OK);
			break;
		case ID_BUTTON_GENERATE_N_SORT:
			CreateCharSequences();
			break;
		case ID_BUTTON_SEND_DATA:
			if (CreateConnection(hWnd)) break;
			break;
		case ID_ROOT_MENU_ITEM_EXIT:
			CloseApp();
			break;
		default: 
			break;
		}
		break;
	case WM_SOCKET_READY:
		isServerEnabled = true;
		break;
	case WM_DESTROY:
		for (int i = 0; i < CHARS_STR_COL_LEN; i++)
			if (strMatr[i] != nullptr)
				delete[]strMatr[i];
		delete[]strMatr;
		strMatr = NULL;
		CloseApp();
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

void CloseSubThread() {
	if (SubThread != NULL) {
		CloseHandle(SubThread);
		SubThread = NULL;
	}
}

void CloseApp() {
	CloseSubThread();
	CloseNetworkThread();
	PostQuitMessage(0);
}

void CreateCharSequences() {
	if (SubThread != NULL) return;
	
	strMatr = new char*[CHARS_STR_COL_LEN];
	for (int i = 0; i < CHARS_STR_COL_LEN; i++)
		strMatr[i] = new char[GEN_CHARS_STR_ROW_LEN];

	TextFields fields = { DefaultCharStrTextBox, SortedCharStrTextBox };
	GeneratorParams params = { strMatr, fields };
	SubThread = CreateThread(NULL, NULL, SubThreadDelegate, &params, NULL, NULL);
}

BOOL CreateConnection(HWND& hWnd) {
	if (isServerEnabled) {
		MessageBoxA(hWnd, "Server already running.", "Already running", MB_OK | MB_ICONWARNING);
		return 1;
	}
	if (strMatr != nullptr) {
		str = new char[GEN_CHARS_STR_ROW_LEN * CHARS_STR_COL_LEN + 1];
		str[0] = '\0';
		for (int i = 0; i < CHARS_STR_COL_LEN; ++i) {
			strcat(str, strMatr[i]);
		}
		PARAMETERS params;
		params.sendingData = str;
		params.window = hWnd;
		NetworkThread = CreateThread(NULL, 0, NetworkThreadDelegate, &params, 0, NULL);
		MessageBoxA(hWnd, "The server was started successfully.", "Success", MB_OK | MB_ICONINFORMATION);
	}
	return 0;
}