#include <Windows.h>
#include "skt.h"

void skt_init()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);
}

void skt_close()
{
	WSACleanup();
}

int skt_send(char *buf, int size, void *parm)
{
	int len = 0;
	int num = 0;
	while(len < size)
	{
		num = send((SOCKET)parm, buf+len, size-len, 0);
		if(num < 1)
			break;

		len += num;
	}
	return 0;
}

/* ½ÓÊÕº¯Êý v1.1 */
int  skt_recv(char *buf, int size, void *parm)
{
	int len = 0;
	int num = 0;
	char * str = buf;
	while(size > 0)
	{
		num = recv((SOCKET)parm, str, size, 0);
		if(num < 1)
			break;

		size -= num;
		str  += num;
	}
	return 1;
}