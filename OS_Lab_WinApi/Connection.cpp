#include "Definitions.h"

int iResult = 0;

BOOL InitWinSock(HWND& window, WSADATA& wsaData) {
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		MessageBox(NULL, _T("WinSock initializing failed."), L"Error", MB_ICONERROR | MB_OK);
		SendMessage(window, WM_ERROR, 0, 0);
		return 1;
	}
	return 0;
}

DWORD WINAPI NetworkThreadDelegate(LPVOID params) {
	char* data_ref = ((PARAMETERS*)params)->sendingData;
	HWND window = ((PARAMETERS*)params)->window;

	WSADATA wsaData;
	SOCKADDR_IN addr;
	SOCKET listeningSocket, clientSocket;
	int addrSize = sizeof(addr);

	if (InitWinSock(window, wsaData)) return 1;

	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(DEFAULT_PORT);
	addr.sin_family = AF_INET;

	listeningSocket = socket(AF_INET, SOCK_STREAM, NULL);
	bind(listeningSocket, (SOCKADDR*)&addr, addrSize);
	listen(listeningSocket, SOMAXCONN);

	clientSocket = accept(listeningSocket, (SOCKADDR*)&addr, &addrSize);
	if (clientSocket == 0) {
		MessageBox(NULL, _T("Client connection failed (Server side)."), L"Error", MB_ICONERROR | MB_OK);
	}
	else {
		send(clientSocket, data_ref, GEN_CHARS_STR_ROW_LEN * CHARS_STR_COL_LEN, NULL);
	}

	return 0;
}


