#pragma once
#include <WinSock2.h>
#pragma comment (lib,"ws2_32.lib")

class CInSocket
{
public:
	CInSocket(BYTE minorVer = 2, BYTE majorVer = 2)
	{
		//≥ı ºªØWS2_32.dll
		WSADATA wsadata;
		WORD sockVersion = MAKEWORD(minorVer, majorVer);
		if (WSAStartup(sockVersion, &wsadata) != 0)
			exit(0);
	}
	~CInSocket()
	{
		WSACleanup();
	}
};