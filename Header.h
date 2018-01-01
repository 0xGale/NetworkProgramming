#pragma once
#include <WinSock2.h>
//#pragma comment (lib,"ws2_32.lib")
typedef struct _IPHeader
{
	UCHAR iphVerLen;			//4 bits Version && 4 bits Type
	UCHAR ipTOS;			//8 bits TOS
	USHORT ipLength;		//16 bits Total Length
	USHORT ipID;		//16 bits ID(Identification)
	USHORT ipFlags;		//3 bits flags && 13 bits fragment offset
	UCHAR ipTTL;		//8 bits TLL(time to live)
	UCHAR ipProtocol;	//8 bits protocol
	USHORT ipChecksum;	//16 bits header checksum
	ULONG ipSource;		//32 bits Source IP address
	ULONG ipDestination;	//32 bits Destination IP address
}IPHeader,*PIPHeader;			//IP Packet

typedef struct _TCPHeader
{
	USHORT sourcePort;		//16-bit source port number
	USHORT destinationPort;		//16-bit destination port number
	ULONG sequenceNumber;		//32-bit sequerce number
	ULONG acknowledgeNumber;	//32-bit acknowledgment number
	UCHAR dataoffset;		//4-bit header length And 6-bit reserve
	UCHAR flags;		//6-bit	flag contain URG,ACK,PSH,RST,SYN,FIN
	UCHAR windows;		//16-bit windows
	UCHAR checksum;		//16-bit checksum
	USHORT urgentPointer;		//16-bit Pointer
}TCPHeader,*PTCPHeader;