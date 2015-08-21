#include <stdio.h>
#include "strapi.h"
#include "http.h"
#include "url.h"


static void http_header_init  (struct _http_head_ * head);
static void http_header_delete(struct _http_head_ * head);
static void http_header_write_head(http_t * http, http_request_t * request);
static void http_header_set_retcode(http_request_t * request, char * buf);
static void http_header_set_retword(http_request_t * requset, char * buf);
static void http_header_set_keyword(struct _http_head_ * head, const char * key, 
	const char * val, int override);
static struct _http_keyword_t_ * http_header_get_keyword(
	struct _http_head_ * head, const char * key, 
	struct _http_keyword_t_ * current);
static void http_recv_event(network_t * network, net_socket_t * netsocket);
static void http_send_event(network_t * network, net_socket_t * netsocket);
static void http_error_event(network_t * network, net_socket_t * netsocket);
static void http_request_free(http_request_t * request);

/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 创建HTTP协议处理模块
** 入  参： http    - HTTP模块指针的指针， 出参
			network - 网络模块指针

** 返回值： void
** 备  注： 使用完成后需要调用http_delete进行资源释放
*******************************************************************************/
http_t * http_create( network_t * network)
{
	http_t * http;
	http = (http_t *)malloc(sizeof(http_t));
	memset(http, 0, sizeof(http_t));
	
	http_header_init(&http->head);
	http->network = network;
	
	http_header_set_keyword(&http->head, "Host", "", 0);
	http_header_set_keyword(&http->head, "Accept", "*/*", 0);
	http_header_set_keyword(&http->head, "Accept-Charset", "GB2312", 0);
	http_header_set_keyword(&http->head, "User-Agent", "Mozilla/5.0", 0);
	http_header_set_keyword(&http->head, "Connection", "Keep-Alive", 0);
	http_header_set_keyword(&http->head, "Keep-Alive", "20", 0);

	return http;
}

void http_free(http_t * http)
{
	http_header_delete(&http->head);
	free(http);
}

void http_config(http_t * http, http_config_t cmd, void * parms)
{
	switch(cmd)
	{
	case HTTP_SET_HEADER:
		{
			struct parms_t {char * key; char * val; int overrite;};
			struct parms_t * p = (struct parms_t *)parms;
			http_header_set_keyword(&http->head, p->key, p->val, p->overrite);
			break;
		}
	case HTTP_SET_RECV_EVENT:
		{
			http->recv_event = (data_event)parms;
			break;
		}
	case HTTP_SET_SEND_EVENT:
		{
			http->send_event = (data_event)parms;
			break;
		}
	case HTTP_SET_HEAD_RECVED:
		{
			http->head_recved = (data_event)parms;
			break;
		}
	case HTTP_SET_ERROR_EVENT:
		{
			http->error_event = (data_event)parms;
			break;
		}
	}
}

static void http_header_init(struct _http_head_ * head)
{
	memset(head, 0, sizeof(struct _http_head_));
}

static void http_header_delete(struct _http_head_ * head)
{
	for(int i=0; i<head->last_keyword; i++)
	{
		free(head->words[i].key);
		free(head->words[i].value);
	}

	free(head->words);
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
static void http_header_set_keyword(struct _http_head_ * head, const char * key,
	const char * val, int override)
{
 // printf("key = %s : val = %s\n", key, val);

	if(override)
	{
		struct _http_keyword_t_ * kw = http_header_get_keyword(head, key, 0);
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
		head->words = (struct _http_keyword_t_ *)realloc(head->words,
			sizeof(struct _http_keyword_t_) * head->max_keyword);
	}

	head->words[head->last_keyword].key   = strdup(key);
	head->words[head->last_keyword].value = strdup(val);
	head->last_keyword++;
}

struct _http_keyword_t_ * http_header_get_keyword(struct _http_head_ * head, 
		const char * key, struct _http_keyword_t_ * current)
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
	url_t * url = spd_url_create(urlstr);
	if(url->port == 0)
		url->port = 80;

	

	net_socket_t * netsocket = network_connect(http->network, url->host, 
		url->port);

	net_socket_config(netsocket, NET_SET_RECV_EVENT, (void *)&http_recv_event);
	net_socket_config(netsocket, NET_SET_SEND_EVENT, (void *)&http_recv_event);
	net_socket_config(netsocket, NET_SET_ERROR_EVENT,
		(void *)&http_error_event);

	if(netsocket == 0)
		return 0;

	http_request_t * request = (http_request_t *)malloc(sizeof(http_request_t));
	memset(request, 0, sizeof(http_request_t));
	
	request->valid = 1;
	request->netsocket  = netsocket;
	request->http = http;
	request->recv_event = http->recv_event;
	request->send_event = http->send_event;
	request->head_recved= http->head_recved;
	request->error_event= http->error_event;

	
	net_socket_set_user_data(netsocket, request);
	http_header_set_keyword(&http->head, "Host", url->host, 1);

	char buf[2048];
	sprintf(buf, "GET %s HTTP/1.1\r\n", url->file);

	net_socket_write(request->netsocket, buf, strlen(buf));
	http_header_write_head (http, request);
	return request;
}

http_request_t * http_post(http_t * http, const char * urlstr, 
		http_callback_read read, void * handle)
{
	return 0;
}

struct _http_head_ * http_request_header(http_request_t * request)
{
	char buf[2048];
	
	if(request->read_end_head)
		return &request->head;

	int size = net_socket_size(request->netsocket);
	char * ps= net_socket_data(request->netsocket);

	while(request->read_end_head == 0 && getsubstr(buf, 2048, ps, '\n') )
	{
		net_socket_pop(request->netsocket, strlen(buf)+1);

		rtrimchr(buf, '\r');

		if(request->read_ret == 0)
		{
			http_header_set_retcode(request, buf);
		}
		else
		{
			http_header_set_retword(request, buf);
		}
 	}

	struct _http_keyword_t_ * key = http_header_get_keyword(&request->head, 
			"Content-Length", 0);

	if(key != 0)
		request->left_data = atoi(key->value);

	if(request->read_end_head)
		return &request->head;
	else
		return 0;
}

struct _http_head_ * http_request_send_header(http_request_t * request)
{
	return &request->http->head;
}

int http_request_statu (http_request_t * request)
{
	return net_socket_statu(request->netsocket);
}

/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 关闭网络连接，并且释放资源
** 入  参： request - 请求模块指针
** 返回值： void
** 备  注： 
*******************************************************************************/
void http_request_close (http_request_t * request)
{
	request->valid = 0;
	net_socket_close(request->netsocket);
}

void http_request_free(http_request_t * request)
{
	http_header_delete(&request->head);
	if(request->message)
		free(request->message);

	free(request);
}

/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 读取HTTP协议数据
** 入  参：request - 请求模块指针
		   buf     - 缓冲区地址
		   size    - 读取大小
           
** 返回值： 成功返回0， 失败返回 SOCKET_ERROR                             
** 备  注： 主要函数
*******************************************************************************/
int http_request_read(http_request_t * request, char * buf, int size)
{
	struct _http_head_ * head = http_request_header(request);
	if(head == 0)
		return 0;
	
	int left_size = request->left_data;
	int buff_size = net_socket_size(request->netsocket);

	size = size > left_size? left_size : size;
	size = size > buff_size? buff_size : size;
	
	if(size > 0)
	{
		memcpy(buf, net_socket_data(request->netsocket), size);
		net_socket_pop(request->netsocket, size);
	}

	request->left_data -= size;
	if(request->left_data == 0)
	{
		struct _http_keyword_t_ * key = http_header_get_keyword(&request->head, 
				"Content-Length", 0);

		if(key == 0)
		{
			char len[1024];
			char * ps= net_socket_data(request->netsocket);
			if(getsubstr(len, 1024, ps, '\n') != 0)
			{
				net_socket_pop(request->netsocket, strlen(len)+1);
				request->left_data = strtol(len, 0, 16);
				
				if(size == 0 && request->left_data == 0)
					return -1;
			}
		}
		else
		{
			//printf("数据读取完成\n");
			return -1;
		}
	}
	
	return size;
}

int http_request_recode(http_request_t * request)
{
	return request->ret_code;
}

char * http_header_getkey(struct _http_head_ * header, const char * key, int n)
{
	struct _http_keyword_t_ * wd = http_header_get_keyword(header, key, 0);
	for(int i=1; i<n; i++)
	{
		wd = http_header_get_keyword(header, key, wd);
		if(wd == 0)
			break;
	}

	return wd ? wd->value : 0;
}

static void http_header_set_retcode(http_request_t * request, char * buf)
{
	request->read_ret = 1;

	char version[20];	
	char retcode[20];
	
	const char * ps = getsubstr(version, 20, buf, ' ');
	if(ps == 0)
		return ;

	//printf("版本号 %s\n", version);

	buf += (ps-buf);
	if(buf == 0)
		return ;

	ltrimchr(buf, ' ');
	ps = getsubstr(retcode, 20, buf, ' ');

	request->ret_code = atoi(retcode);

	//printf("返回码 %s\n", retcode);

	buf += (ps-buf);
	ltrimchr(buf, ' ');

	request->message = strdup(buf);
	//printf("描述符 %s\n", buf);
}

void http_request_set_user_data(http_request_t * req, void * d)
{
	req->user_data = d;
}

void * http_request_get_user_data(http_request_t * req)
{
	return req->user_data;
}

static void http_header_set_retword(http_request_t * requset, char * buf)
{
	char key[1024];
	
	if(strlen(buf) <= 0) // 空行，解析完成
	{
		requset->read_end_head = 1;
		return ;
	}

	const char * ps = getsubstr(key, 1024, buf, ':');
	if(ps == 0)
		return ;

	// printf("关键字 %s\n", key);

	buf += (ps-buf);
	ltrimchr(buf, ' ');
	
	// printf("值 [%s]\n", buf);

	http_header_set_keyword(&requset->head, key, buf, 0);
}

void http_header_write_head(http_t * http, http_request_t * request)
{
	char buf[2048];
	for(int i=0; i<http->head.last_keyword; i++)
	{
		if(http->head.words[i].value == 0)
			break;

		sprintf(buf, "%s: %s\r\n",http->head.words[i].key, 
				http->head.words[i].value);

		net_socket_write(request->netsocket, buf, strlen(buf));
	}

	strcpy(buf, "\r\n");
	net_socket_write(request->netsocket, buf, strlen(buf));
}

/*******************************************************************************
** 版  本： v 1.1     
** 功  能： 接收事件
** 入  参：
** 返回值： 
** 备  注： 
*******************************************************************************/
void http_recv_event(network_t * network, net_socket_t * netsocket)
{
	http_request_t * request = 
		(http_request_t *)net_socket_get_user_data(netsocket);

	if(request->read_end_head == 0)
	{
		struct _http_head_ * head = http_request_header(request);	
		if(head != 0)
			request->head_recved(request);
	}

	if(request->valid != 0)
	{
		if(request->recv_event)
			request->recv_event(request);
	}

	if(request->valid == 0)
		http_request_free(request);
}

void http_send_event(network_t * network, net_socket_t * netsocket)
{
	http_request_t * request = 
		(http_request_t *)net_socket_get_user_data(netsocket);

	if(request->send_event)
		request->send_event(request);

	if(request->valid == 0)
		http_request_free(request);
}

void http_error_event(network_t * network, net_socket_t * netsocket)
{
	http_request_t * request = 
		(http_request_t *)net_socket_get_user_data(netsocket);

	if(request->error_event)
		request->error_event(request);

	if(request->valid == 0)
		http_request_free(request);
}
