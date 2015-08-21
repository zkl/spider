#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include "spider.h"

const char * host[] = {
	"http://www.baidu.com",
	"www.google.com",
	"www.iqiyi.com",
	"www.youku.com",
};

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD( 2, 2);
	WSAStartup( wVersionRequested, &wsaData );

	spider_t * spider = spider_create();

	spider_exec(spider, host[0]);

	spider_free(spider);

	printf("done\n");
	getchar();
	return 0;
}
