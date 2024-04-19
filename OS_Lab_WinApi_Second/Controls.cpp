#include "Definitions.h"

void AddMainInterface(HWND hWnd) {
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

void AddTCPInterface(HWND hWnd) {
	CreateWindowA("button", "Connect and fetch chars", WS_VISIBLE | WS_CHILD, 5, WND_HEIGHT - 90, WND_WIDTH - 30, 20, hWnd, (HMENU)ID_BUTTON_CREATE_CONNECTION, NULL, NULL);
}