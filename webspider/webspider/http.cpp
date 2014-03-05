#include <stdio.h>
#include "strapi.h"
#include "http.h"
#include "url.h"

static void http_head_init(struct http_head * head);
static void http_header_write_head(http_t * http, http_request_t * request);
static void http_header_set_retcode(http_request_t * request, char * buf);

static void http_header_set_keyword(struct http_head * head, const char * key, 
	const char * val, int over_write);
static struct http_ketword * http_header_get_keyword(struct http_head * head, const char * key, 
	struct http_ketword * current);


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
	
	http_head_init(&(*http)->head);
	(*http)->network = network;
	
	http_header_set_keyword(&(*http)->head, "Host", "", 0);
	http_header_set_keyword(&(*http)->head, "Accept", "*/*", 0);
	http_header_set_keyword(&(*http)->head, "User-Agent", "Mozilla/5.0", 0);
	http_header_set_keyword(&(*http)->head, "Connection", "Keep-Alive", 0);
	http_header_set_keyword(&(*http)->head, "Keep-Alive", "2000", 0);
}

static void http_head_init(struct http_head * head)
{
	memset(head, 0, sizeof(struct http_head));
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
static void http_header_set_keyword(struct http_head * head, const char * key, 
							 const char * val, int over_write)
{
	if(over_write)
	{
		struct http_ketword * kw = http_header_get_keyword(head, key, 0);
		if(kw != 0)
		{
			free(kw->value);
			kw->value = strdup(val);
			return ;
		}
	}

	if(head->last_keyword == head->max_keyword)
	{
		head->max_keyword += 20;
		head->words = (struct http_ketword *)realloc(head->words,
			sizeof(struct http_ketword) * head->max_keyword);
	}

	head->words[head->last_keyword].key   = strdup(key);
	head->words[head->last_keyword].value = strdup(val);
	head->last_keyword++;
}


struct http_ketword * http_header_get_keyword(struct http_head * head, const char * key, 
	struct http_ketword * current)
{
	if(current == 0)
		current = head->words;

	int index = current - head->words;

	if(index < 0)
		return 0;

	for(index; index < head->last_keyword; index++)
	{
		if(stricmp(head->words[index].key, key) == 0)
			return head->words + index;
	}

	return 0;
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
	memset(request, 0, sizeof(http_request_t));
	
	request->netsocket = netsocket;
	
	http_header_set_keyword(&http->head, "Host", url->host, 1);

	char buf[2048];
	sprintf(buf, "GET %s HTTP/1.1\r\n", url->file);

	net_socket_write(request->netsocket, buf, strlen(buf));
	http_header_write_head (http, request);
	return request;
}

http_request_t * http_post(http_t * http, const char * urlstr, http_callback_read read, void * handle)
{
	return 0;
}

struct http_head * http_request_header(http_request_t * request)
{
	char buf[2048];
	
	if(request->read_end_head)
		return &request->head;

	int size = net_socket_size(request->netsocket);
	char * ps= net_socket_data(request->netsocket);

	while(getsubstr(buf, 2048, ps, 0, '\n') == 0)
	{
		net_socket_pop(request->netsocket, strlen(buf)+1);
		
		rtrimchr(buf, '\r');

		if(request->read_ret == 0)
		{
			http_header_set_retcode(request, buf);
		}
		else
		{
			if(strlen(buf) <= 2)
				request->read_end_head = 1;
		}

		printf("%s\n", buf);

	
	}

	return 0;
}

static void http_header_set_retcode(http_request_t * request, char * buf)
{
	request->read_ret = 1;
		
	char version[20];
	char retcode[20];
	char message[1024];

	request->ret_code = atoi(retcode);
}

void http_header_write_head(http_t * http, http_request_t * request)
{
	char buf[2048];
	for(int i=0; i<http->head.last_keyword; i++)
	{
		if(http->head.words[i].value == 0)
			break;

		sprintf(buf, "%s: %s\r\n",http->head.words[i].key, http->head.words[i].value);
		net_socket_write(request->netsocket, buf, strlen(buf));
	}

	strcpy(buf, "\r\n");
	net_socket_write(request->netsocket, buf, strlen(buf));
}