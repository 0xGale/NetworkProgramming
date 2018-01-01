#include <iostream>
#include "InSocket.h"
#include <Mstcpip.h>
#include "Header.h"
#define SIO_RCVALL _WSAIOW(IOC_VENDOR,1)
using namespace std;
CInSocket Ssocket;
void DecodeTCPPacket(char *pData);
void DecodeIPPacket(char *pData);
int main()
{
	//创建原始套接字
	SOCKET sRaw = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	char szHostName[56];
	SOCKADDR_IN addr_in;
	struct hostent *pHost;
	gethostname(szHostName, 56);
	if ((pHost = gethostbyname((char *)szHostName)) == NULL)		
	{
		return 0;
	}
	//在调用ioctl之前，套接字必须绑定
	addr_in.sin_family = AF_INET;		//
	addr_in.sin_port = htons(0);		//转化为网络字节顺序
	memcpy(&addr_in.sin_addr.S_un.S_addr, pHost->h_addr_list[0], pHost->h_length);
	cout << "Bindling to interface:" << ::inet_ntoa(addr_in.sin_addr) << endl;
	if (bind(sRaw, (PSOCKADDR)&addr_in, sizeof(addr_in)) == SOCKET_ERROR)
	{
		cout << "bind error";
		return 0;
	}
		
	//设置SIO_RCVALL控制代码，以便接收所有的IP包
	DWORD dwValue = 1;
	//WSAIoctl(rawsock, SIO_RCVALL, &lpvBuffer, sizeof(lpvBuffer), NULL, 0, &lpcbBytesReturned, NULL, NULL);
	if (ioctlsocket(sRaw, SIO_RCVALL, &dwValue) != 0)			//winsock1 中的函数，winsock2中使用WSAIoctl
	{
		return 0;
	}
	//开始接收封包
	char buff[1024];
	int nRet;
	while (TRUE)
	{
		nRet = recv(sRaw, buff, 1024, 0);
		if (nRet > 0)
		{
			DecodeIPPacket(buff);
		}
	}
	
	return 0;
}
void DecodeTCPPacket(char *pData)
{
	TCPHeader *pTCPHdr = (TCPHeader *)pData;
	cout << endl;
	cout << "Port: " << ntohs(pTCPHdr->sourcePort) << "-> " << ntohs(pTCPHdr->destinationPort);
	//下面还可以根据目的端口号进一步解析应用层协议
	switch (::ntohs(pTCPHdr->destinationPort))
	{
	case 21:
		break;
	case 80:
	case 8080:
		cout << "您当前正在浏览网页。" << endl;
		break;
	}
}

void DecodeIPPacket(char *pData)
{
	IPHeader *pIPHdr = (IPHeader *)pData;
	in_addr source, dest;
	char szSourceIp[32], szDestIp[32];
	cout << endl;
	cout << "----------------------------" << endl;
	//从IP头中取出源IP地址和目的IP地址
	source.S_un.S_addr = pIPHdr->ipSource;
	dest.S_un.S_addr = pIPHdr->ipDestination;
	strcpy(szSourceIp, ::inet_ntoa(source));
	strcpy(szDestIp, ::inet_ntoa(dest));
	cout << szSourceIp << "->" << szDestIp;
	int nHeaderLen = (pIPHdr->iphVerLen & 0xf)*sizeof(ULONG);
	switch (pIPHdr->ipProtocol)
	{
	case IPPROTO_TCP://TCP
		cout << endl;
		cout << "This is a TCP protocol";
		DecodeTCPPacket(pData + nHeaderLen);
		break;
	case IPPROTO_UDP:
		cout << endl;
		cout << "This is a UDP protocol";
		break;
	case IPPROTO_ICMP:
		cout << endl;
		cout << "This is a ICMP protocol";
		break;
	}
}