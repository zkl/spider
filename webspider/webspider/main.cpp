#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include "spider.h"

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
	WORD wVersionRequested = MAKEWORD( 2, 2);
	WSAStartup( wVersionRequested, &wsaData );

	spider_t * spider;

	spider_init(&spider);
	spider_start(spider, host[0]);
	spider_delete(spider);
	
	getchar();
	return 0;
}