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
