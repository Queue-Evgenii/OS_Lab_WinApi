#pragma once

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define WINVER 0x0501
#define _WIN32_WINNT 0x0501

#pragma comment(lib, "Ws2_32.lib")
#include <SDKDDKVer.h>
#include "Definitions.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "Windows.h"
#include <WindowsX.h>
#include <ShellAPI.h>
#include "Models.h"
#include "Prototypes.h"
#include "IdDefinitions.h"
#include <tchar.h>
#include <thread>
#include <future>

#define DEFAULT_PORT 8800

#define WND_WIDTH 1000
#define WND_HEIGHT 600
#define WND_POS_X 500
#define WND_POS_Y 300

#define GEN_CHARS_STR_ROW_LEN 8
#define CHARS_STR_ROW_LEN GEN_CHARS_STR_ROW_LEN * 2
#define CHARS_STR_COL_LEN 20
