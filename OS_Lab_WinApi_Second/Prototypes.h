#pragma once

struct PARAMETERS
{
  HWND& window;
  char* receivingData;
};

void AddMainMenu(HWND hWnd);

BOOL CreateConnection(HWND& hWnd, PARAMETERS& params);
void ProcessReceivedData(HWND& window, char* data);

DWORD WINAPI NetworkThreadDelegate(LPVOID);
void CloseNetworkThread();