#include "Definitions.h"

void AddMainMenu(HWND hWnd) {
	HMENU RootMenu = CreateMenu();
	HMENU SubMenuFile = CreateMenu();
	HMENU SubMenuAbout = CreateMenu();

	AppendMenuW(SubMenuFile, MF_STRING, ID_ROOT_MENU_ITEM_EXIT, L"Exit");
	AppendMenuW(RootMenu, MF_POPUP, (UINT_PTR)SubMenuFile, L"File");


	AppendMenuW(SubMenuAbout, MF_STRING, ID_ROOT_MENU_ITEM_AUTHOR, L"Author");
	AppendMenuW(SubMenuAbout, MF_STRING, ID_ROOT_MENU_ITEM_PROJECT, L"Project");
	AppendMenuW(RootMenu, MF_POPUP, (UINT_PTR)SubMenuAbout, L"About");

	SetMenu(hWnd, RootMenu);
}

void AddStringGeneratorInterface(HWND hWnd, HWND& TextBox1, HWND& TextBox2) {
	TextBox1 = CreateWindowA("static", "Nothing generated now.", WS_VISIBLE | WS_CHILD | ES_CENTER | WS_DISABLED, 5, 5, WND_WIDTH / 2 - 10, WND_HEIGHT - 200, hWnd, NULL, NULL, NULL);
	TextBox2 = CreateWindowA("static", "Nothing generated now.", WS_VISIBLE | WS_CHILD | ES_CENTER | WS_DISABLED, WND_WIDTH / 2, 5, WND_WIDTH / 2 - 10, WND_HEIGHT - 200, hWnd, NULL, NULL, NULL);
	CreateWindowA("button", "Generate and sort chars", WS_VISIBLE | WS_CHILD, 5, WND_HEIGHT - 120, WND_WIDTH - 30, 20, hWnd, (HMENU)ID_BUTTON_GENERATE_N_SORT, NULL, NULL);
}

void AddTCPInterface(HWND hWnd) {
	CreateWindowA("button", "Send in another application", WS_VISIBLE | WS_CHILD, 5, WND_HEIGHT - 90, WND_WIDTH - 30, 20, hWnd, (HMENU)ID_BUTTON_SEND_DATA, NULL, NULL);
}

void AddTrayIcon(HWND hWnd, UINT uID, HICON hIcon, LPCWSTR lpszTip) {
  NOTIFYICONDATA nid;
  ZeroMemory(&nid, sizeof(NOTIFYICONDATA));
  nid.cbSize = sizeof(NOTIFYICONDATA);
  nid.hWnd = hWnd;
  nid.uID = uID;
  nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
  nid.uCallbackMessage = WM_USER + 1;
  nid.hIcon = hIcon;
  lstrcpyn(nid.szTip, lpszTip, sizeof(nid.szTip));
  Shell_NotifyIcon(NIM_ADD, &nid);
}

void RemoveTrayIcon(HWND hWnd, UINT uID) {
  NOTIFYICONDATA nid;
  ZeroMemory(&nid, sizeof(NOTIFYICONDATA));
  nid.cbSize = sizeof(NOTIFYICONDATA);
  nid.hWnd = hWnd;
  nid.uID = uID;
  Shell_NotifyIcon(NIM_DELETE, &nid);
}