#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include "spider.h"

const char * host[] = {
	"www.baidu.com",
	"www.google.com",
	"www.iqiyi.com",
	"www.youku.com",
};

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD( 2, 2);
	WSAStartup( wVersionRequested, &wsaData );

	
	http_t * http;
	network_t * network;
	network_create(&network);
	http_create(&http, network);

	http_request_t * request = http_get(http, "www.baidu.com");
	
	while(request)
	{
		int ret = network_procmsg(network);

		if(ret == SOCKET_ERROR)
		{
			printf("socket error\n");
			break;
		}

		
		struct http_head * head = http_request_header(request);
		if(head == 0)
		{
			if(http_request_statu(request) <= 0)
				break;

			continue;
		}

		char * type = http_header_getkey(head, "Content-Type", 0);
		
		
		
		char buf[1028] = {0};
		ret = http_request_read(request, buf, 1024);
		
		printf("%s", buf);
		Sleep(1000);
		
		if(ret < 0)
			break;

		if(http_request_statu(request) <= 0)
				break;
	}

	
	getchar();
	return 0;
}
