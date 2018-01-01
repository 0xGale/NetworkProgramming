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
	//����ԭʼ�׽���
	SOCKET sRaw = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	char szHostName[56];
	SOCKADDR_IN addr_in;
	struct hostent *pHost;
	gethostname(szHostName, 56);
	if ((pHost = gethostbyname((char *)szHostName)) == NULL)		
	{
		return 0;
	}
	//�ڵ���ioctl֮ǰ���׽��ֱ����
	addr_in.sin_family = AF_INET;		//
	addr_in.sin_port = htons(0);		//ת��Ϊ�����ֽ�˳��
	memcpy(&addr_in.sin_addr.S_un.S_addr, pHost->h_addr_list[0], pHost->h_length);
	cout << "Bindling to interface:" << ::inet_ntoa(addr_in.sin_addr) << endl;
	if (bind(sRaw, (PSOCKADDR)&addr_in, sizeof(addr_in)) == SOCKET_ERROR)
	{
		cout << "bind error";
		return 0;
	}
		
	//����SIO_RCVALL���ƴ��룬�Ա�������е�IP��
	DWORD dwValue = 1;
	//WSAIoctl(rawsock, SIO_RCVALL, &lpvBuffer, sizeof(lpvBuffer), NULL, 0, &lpcbBytesReturned, NULL, NULL);
	if (ioctlsocket(sRaw, SIO_RCVALL, &dwValue) != 0)			//winsock1 �еĺ�����winsock2��ʹ��WSAIoctl
	{
		return 0;
	}
	//��ʼ���շ��
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
	//���滹���Ը���Ŀ�Ķ˿ںŽ�һ������Ӧ�ò�Э��
	switch (::ntohs(pTCPHdr->destinationPort))
	{
	case 21:
		break;
	case 80:
	case 8080:
		cout << "����ǰ���������ҳ��" << endl;
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
	//��IPͷ��ȡ��ԴIP��ַ��Ŀ��IP��ַ
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