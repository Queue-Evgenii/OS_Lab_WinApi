#pragma once

LRESULT CALLBACK MainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HICON Icon, LPCWSTR Name, HINSTANCE hInst, WNDPROC Procedure);

void AddMainMenu(HWND hWnd);
void AddTCPInterface(HWND hWnd);
void AddStringGeneratorInterface(HWND hWnd, HWND& TextBox1, HWND& TextBox2);

void CreateCharSequences();
BOOL CreateConnection(HWND& hWnd);

void CloseNetworkThread();
void CloseSubThread();
void CloseApp();

void SetTextFromMatrix(int i, char* str, char** strMatr);

DWORD WINAPI NetworkThreadDelegate(LPVOID);
DWORD WINAPI SubThreadDelegate(LPVOID);
