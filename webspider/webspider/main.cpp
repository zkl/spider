#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include "todolist.h"
#include "html.h"
#include "dns.h"
#include "network.h"
#include "url.h"
#include "http.h"

const char * test1 = "http://53359.com/";
const char * test2 = "bbs8899.net/Sort.aspx?number=3";

const char * host[] = {
	"53359.com",
	"www.baidu.com",
	"www.google.com",
	"www.iqiyi.com",
	"www.youku.com",
	"www.yunio.com",
	"smtp.qq.com:25",
	"http://www.baidu.com/index.html",
	"http://www.kerneltravel.net/",
};

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD( 2, 2 );
	WSAStartup( wVersionRequested, &wsaData );

	network_t * network;
	http_t * http;

	network_create(&network);
	http_create(&http, network);

	http_request_t * request = http_get(http, host[1]);
	if( request == 0)
	{
		printf("连接到服务器错误\n");
	}

	while(1)
	{
		if(network_procmsg(network) == SOCKET_ERROR)
			printf("socket error\n");

		char buf[1028] = {0};
		if(http_request_read(request, buf, 1024))
			printf("%s\n", buf);
	
		Sleep(1000);
	}

	network_delete(network);

	getchar();
	return 0;
}