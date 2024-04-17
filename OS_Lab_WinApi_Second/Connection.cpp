#include "Definitions.h"

BOOL InitWinSock(HWND& window, WSADATA& wsaData) {
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    MessageBox(NULL, _T("WinSock initializing failed."), L"Error", MB_ICONERROR | MB_OK);
    SendMessage(window, WM_ERROR, 0, 0);
    return 1;
  }
  return 0;
}

DWORD WINAPI NetworkThreadDelegate(LPVOID params) {
  HWND& window = ((PARAMETERS*)params)->window;
  char* data = ((PARAMETERS*)params)->receivingData;
  WSADATA wsaData;
  SOCKADDR_IN addr;
  SOCKET connectionSocket;
  int addrSize = sizeof(addr);

  if (InitWinSock(window, wsaData)) return 1;

  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  addr.sin_port = htons(DEFAULT_PORT);
  addr.sin_family = AF_INET;

  connectionSocket = socket(AF_INET, SOCK_STREAM, NULL);

  if (connect(connectionSocket, (SOCKADDR*)&addr, addrSize) != 0) {
    MessageBox(NULL, _T("Client connection failed (Client side)."), L"Error", MB_ICONERROR | MB_OK);
    SendMessage(window, WM_ERROR, 0, 0);
    return 1;
  }
  data = new char[GEN_CHARS_STR_ROW_LEN * CHARS_STR_COL_LEN + 1];
  while (recv(connectionSocket, data, GEN_CHARS_STR_ROW_LEN * CHARS_STR_COL_LEN, NULL) == 0) {}
  data[GEN_CHARS_STR_ROW_LEN * CHARS_STR_COL_LEN] = '\0';

  ProcessReceivedData(window, data);

  return 0;
}
