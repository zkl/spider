#include <stdio.h>
#include "http.h"
#include "url.h"

static void http_send_header(http_t * http, http_request_t * request);
static void http_send_block(http_request_t * request, const char * buf, int len);

/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 创建HTTP协议处理模块
** 入  参： http    - HTTP模块指针的指针， 出参
			network - 网络模块指针

** 返回值： void
** 备  注： 使用完成后需要调用http_delete进行资源释放
*******************************************************************************/
void http_create(http_t ** http, network_t * network)
{
	*http = (http_t *)malloc(sizeof(http_t));
	memset(*http, 0, sizeof(http_t));

	(*http)->network = network;

	strcpy((*http)->header[HTTP_HOST].key      , "Host");
	strcpy((*http)->header[HTTP_ACCEPT].key    , "Accept");
	strcpy((*http)->header[HTTP_USER_AGENT].key, "User-Agent");
	strcpy((*http)->header[HTTP_CONNECTION].key, "Connection");
	strcpy((*http)->header[HTTP_KEEP_ALIVE].key, "Keep-Alive");


	http_set(*http, HTTP_HOST, "");
	http_set(*http, HTTP_ACCEPT, "*/*");
	http_set(*http, HTTP_USER_AGENT, "Mozilla/5.0");
	http_set(*http, HTTP_CONNECTION, "Keep-Alive");
	http_set(*http, HTTP_KEEP_ALIVE, "2000");
}

/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 设置HTTP请求头内容
** 入  参： http - HTTP模块指针的指针， 出参
			t    - 请求头字段标识符
			val  - 请求头字段的内容

** 返回值： void
** 备  注： 
*******************************************************************************/
void http_set(http_t * http, http_word_t t, const char * val)
{
	if(http->header[t].val != 0)
		free(http->header[t].val);

	http->header[t].val = strdup(val);
}

/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 发送GET请求
** 入  参： http    - HTTP模块指针的指针， 出参
			urlstr  - 以NULL结尾的字符串地址

** 返回值： 指向请求模块的指针
** 备  注： 
*******************************************************************************/
http_request_t * http_get (http_t * http, const char * urlstr)
{
	url_t * url;
	url_create(&url, urlstr);
	if(url->port == 0)
		url->port = 80;

	net_socket_t * netsocket = network_connect(http->network, url->host, url->port);
	if(netsocket == 0)
		return 0;

	http_request_t * request = (http_request_t *)malloc(sizeof(http_request_t));
	request->netsocket = netsocket;
	
	http_set(http, HTTP_HOST, url->host);

	char buf[2048];
	sprintf(buf, "GET %s HTTP/1.1\r\n", url->file);

	http_send_block(request, buf, strlen(buf));
	http_send_header(http, request);
	return request;
}


void http_send_header(http_t * http, http_request_t * request)
{
	char buf[2048];
	for(int i=0; i<HTTP_MAX_WORD; i++)
	{
		if(http->header[i].val == 0)
			continue;

		sprintf(buf, "%s: %s\r\n", http->header[i].key, http->header[i].val);
		http_send_block(request, buf, strlen(buf));
	}

	strcpy(buf, "\r\n");
	http_send_block(request, buf, strlen(buf));
}

void http_send_block(http_request_t * request, const char * buf, int len)
{
	int ret = net_socket_write(request->netsocket, buf, len);

	if(ret <= 0)
		return; 

	if(ret < len)
	{
		network_procmsg(request->netsocket->network);

		http_send_block(request, buf+ret, len-ret);
	}
}