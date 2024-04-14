#pragma once

struct PARAMETERS
{
  HWND& window;
  char* receivingData;
};

void AddMainMenu(HWND hWnd);

BOOL CreateConnection(HWND& hWnd, PARAMETERS& params);

DWORD WINAPI NetworkThreadDelegate(LPVOID);
void CloseNetworkThread();