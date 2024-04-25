#include "Definitions.h"
#include <algorithm>
#include <string>

int*(_cdecl* countCharInSequence)(char* str);
int* count;
char* maxOccurrencesChars;

LRESULT CALLBACK MainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HICON Icon, LPCWSTR Name, HINSTANCE hInst, WNDPROC Procedure);

HANDLE NetworkThread;
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

void DrawGraph(HWND hWnd) {
	if (count == nullptr) return;
	HDC hdc = GetDC(hWnd);

	RECT clientRect;
	GetClientRect(hWnd, &clientRect);

	int offset = 40;
	int zoneHeight = (WND_HEIGHT - 120);

	int scaleX = (WND_WIDTH) / LENGTH_IN_ALPHABET;
	int scaleY = (WND_HEIGHT) / 15; 

	MoveToEx(hdc, offset, zoneHeight, nullptr);
	LineTo(hdc, (WND_WIDTH), zoneHeight);

	for (int i = 0; i < LENGTH_IN_ALPHABET / 2; ++i) {
		wchar_t letter = L'A' + i;
		wchar_t str[2] = { letter, L'\0' };
		TextOut(hdc, i * scaleX + offset - 5, zoneHeight + 10, str, 2);
	}
	for (int i = 0; i < LENGTH_IN_ALPHABET / 2; ++i) {
		wchar_t letter = L'a' + i;
		wchar_t str[2] = { letter, L'\0' };
		TextOut(hdc, ((LENGTH_IN_ALPHABET / 2) + i) * scaleX + offset - 5, zoneHeight + 10, str, 2);
	}

	MoveToEx(hdc, offset, 0, nullptr);
	LineTo(hdc, offset, zoneHeight);

	int max = 0;
	for (int i = 0; i < LENGTH_IN_ALPHABET; ++i) {
		if (max < count[i]) max = count[i];
	}

	for (int i = 0; i <= max; ++i) {
		std::wstring valueText = std::to_wstring(i);
		TextOut(hdc, offset - 10, (zoneHeight) - i * scaleY, valueText.c_str(), valueText.length());
	}

	for (int i = 0; i < LENGTH_IN_ALPHABET - 1; ++i) {
		MoveToEx(hdc, i * scaleX + offset, zoneHeight - count[i] * scaleY, nullptr);
		LineTo(hdc, (i + 1) * scaleX + offset, zoneHeight - count[i + 1] * scaleY);
	}

	ReleaseDC(hWnd, hdc);
}

std::string RetrieveDataFromRegistry() {
	HKEY hKey;
	if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\OS_Lab", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
		char buffer[256];
		DWORD bufferSize = sizeof(buffer);
		if (RegQueryValueExA(hKey, "topFive", NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
			return std::string(buffer);
		}
		RegCloseKey(hKey);
	}
	return "Registry is empty";
}

void DeleteDataFromRegistry() {
	RegDeleteKeyA(HKEY_CURRENT_USER, "Software\\OS_Lab");
}

LRESULT CALLBACK MainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	PARAMETERS params = { hWnd, str };
	PAINTSTRUCT ps;

	switch (msg)
	{
	case WM_CREATE:
		AddMainInterface(hWnd);
		AddTCPInterface(hWnd);
		break;
	case WM_KEYDOWN:
		if (wp == 'H' && GetKeyState(VK_CONTROL) < 0) {
			MessageBoxA(hWnd, (LPCSTR)RetrieveDataFromRegistry().c_str(), "Top five chars", MB_OK);
		}
		if (wp == 'P' && GetKeyState(VK_CONTROL) < 0) {
			DeleteDataFromRegistry();
			MessageBoxA(hWnd, "Registry is cleaning", "Top five chars", MB_OK);
		}
		break;
	case WM_PAINT:
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
		case ID_BUTTON_CREATE_CONNECTION:
			CreateConnection(hWnd, params);
			break;
		case ID_ROOT_MENU_ITEM_EXIT:
			DeleteDataFromRegistry();
			PostQuitMessage(0);
			break;
		default:

			break;
		}
		break;
	case WM_DESTROY:
		DeleteDataFromRegistry();
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
	if (NetworkThread != NULL) {
		CloseHandle(NetworkThread);
		NetworkThread = NULL;
	}
}

BOOL CreateConnection(HWND& hWnd, PARAMETERS& params) {
	params.receivingData = new char[GEN_CHARS_STR_ROW_LEN * CHARS_STR_COL_LEN];

	NetworkThread = CreateThread(NULL, 0, NetworkThreadDelegate, &params, 0, NULL);
	return 0;
}

BOOL WriteToRegistry(const char* keyName, const char* valueName, char* valueData) {
	HKEY hKey;
	LONG result = RegCreateKeyExA(HKEY_CURRENT_USER, keyName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
	if (result != ERROR_SUCCESS) {
		MessageBoxA(NULL, "Error creating/opening registry key", "Registry error", MB_ICONERROR | MB_OK);
		return 1;
	}

	result = RegSetValueExA(hKey, valueName, 0, REG_SZ, (BYTE*)valueData, strlen(valueData) + 1);
	if (result != ERROR_SUCCESS) {
		MessageBoxA(NULL, "Error writing to registry", "Registry error", MB_ICONERROR | MB_OK);
		RegCloseKey(hKey);
		return 1;
	}

	RegCloseKey(hKey);
	return 0;
}
char* GetMaxNIndexes(int* count, int n) {
	char* maxOccurrences = new char[n + 1];
	for (int k = 0; k < n; ++k) {
		int maxFrequency = 0, maxFrequencyIndex = 0;
		char mostFrequentChar;
		for (int i = 0; i < LENGTH_IN_ALPHABET; i++) {
			if (count[i] > maxFrequency) {
				maxFrequencyIndex = i;
				maxFrequency = count[i];
				maxOccurrences[k] = (char)('A' + i + ((i < LENGTH_IN_ALPHABET / 2) ? 0 : 6));
			}
		}
		count[maxFrequencyIndex] = 0;
	}
	maxOccurrences[n] = '\0';
	return maxOccurrences;
}

void ProcessReceivedData(HWND& hWnd, char* data) {
	CloseNetworkThread();
	MessageBoxA(NULL, data, "Received data", MB_ICONINFORMATION | MB_OK);
	count = countCharInSequence(data);
  maxOccurrencesChars = GetMaxNIndexes(count, 5);
	WriteToRegistry("Software\\OS_Lab", "topFive", maxOccurrencesChars);
	count = countCharInSequence(data);
}