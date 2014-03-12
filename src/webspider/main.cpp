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

	spider_t * spider;

	spider_init(&spider);
	spider_start(spider, host[0]);
	spider_delete(spider);
	
	getchar();
	return 0;
}