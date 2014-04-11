#include <stdio.h>
#include "url.h"
#include "pcre/inc/pcre.h"
#include "spider.h"
#include "queue.h"

/* 解析地址 */
static void   spider_anlize(spider_t * spider, queue_t * mbuf, 
					http_request_t * request);
static char * spider_format(char * furl, char * url, const char * host);
static int    spider_is_text(const char * type);
static void   spider_arrived(http_request_t * request);
static void   spider_header (http_request_t * request);
static void   spider_errors (http_request_t * request);
static void   spider_reqnext(spider_t * spider);

void spider_anlize(spider_t * spider, queue_t * mbuf, 
					http_request_t * request)
{
	int ret;
	int  ovector[100];
	
	char * html = queue_data(mbuf);
	int len = queue_size(mbuf);

	struct _http_head_ * header = http_request_send_header(request);

	const char * url = http_header_getkey(header, "Host", 0);
	ret = pcre_exec(spider->link, 0, html, len, 0, 0, ovector, 100);
 
	char url_buf [1024];
	char furl_buf[1024];
	if (ret > 0) 
	{
        char * start = html + ovector[0]; 
        int len = ovector[1] - ovector[0];
 
		sprintf(url_buf, "%.*s\n", len, start); 
		if(spider_format(furl_buf, url_buf, url))
		{
			if(todo_insert(spider->history, furl_buf) == 0)
			{
				todo_insert(spider->urls, furl_buf);
				//printf("%s\n", furl_buf);
			}
		}

		char * substring_end = start + len;
		queue_dequeue(mbuf, substring_end - html);
	}
	  
}

static char * spider_format(char * furl, char * url, const char * host)
{
	char * sp = strchr(url, '"');
	if(sp == 0)
		return 0;

	char * ep = strchr(++sp, '"');
	if(ep == 0)
		return 0;

	*ep = 0;

	if(strstr(sp, "://") != 0)
	{
		strcpy(furl, sp);
	}
	else
	{
		url_t * u;
		url_create(&u, host);
		sprintf(furl, "http://%s/%s", u->host, sp); 
	}
	return furl;
}

static int spider_is_text(const char * type)
{
	if(strstr(type, "text") != 0)
		return 1;
	else
		return 0;
}

spider_t * spider_create()
{
	const char *error;  
	int erroffset;  

	spider_t * spider = (spider_t *)malloc(sizeof(spider_t));
	memset(spider, 0, sizeof(spider_t));

	spider->imgage = pcre_compile("src[\\s]*=[\\s]*\\\".*?\"", 0, &error, 
			&erroffset, 0); 

	spider->link   = pcre_compile("href[\\s]*=[\\s]*\\\".*?\"", 0, &error, 
			&erroffset, 0); 

	spider->network = network_create();
	spider->http = http_create(spider->network);

	http_config(spider->http, HTTP_SET_RECV_EVENT, (void *)&spider_arrived);
	http_config(spider->http, HTTP_SET_HEAD_RECVED, (void *)&spider_header);
	http_config(spider->http, HTTP_SET_ERROR_EVENT, (void *)&spider_errors);

	spider->goon = 1;
	spider->buffer = queue_create(1024, 1024);
	spider->history = todo_create();
	spider->images  = todo_create();
	spider->urls    = todo_create();
	return spider;
}

void spider_config(spider_t * spider, int cmd, void * parm)
{
}


void spider_exec(spider_t * spider, const char * first_host)
{
	todo_insert(spider->urls, first_host);
	spider_reqnext(spider);

	while(spider->goon)
	{
		int ret = network_procmsg(spider->network);
		if(ret == SOCKET_ERROR)
		{
			printf("prco network error\n");
			break;
		}
	}
}

void spider_quit(spider_t * spider)
{
	spider->goon = 0;
}

void spider_free(spider_t * spider)
{
	pcre_free(spider->imgage);
	pcre_free(spider->link);
	http_free(spider->http);
	network_free(spider->network);

	todo_free(spider->history);
	todo_free(spider->images);
	todo_free(spider->urls);

	free(spider);
}


void spider_arrived(http_request_t * request)
{
	char buf[1024] = {0};
	spider_t * spider = (spider_t *)http_request_get_user_data(request);
	int num;
	do
	{
		num = http_request_read(request, buf, 1000);
		if(num > 0)
			queue_enqueue(spider->buffer, buf, num);
	}while(num > 0);

	if(num == -1)
	{
		printf("\nrecv done\n");
		spider_anlize(spider, spider->buffer, request);
		http_request_close(request);
		spider_reqnext(spider);
	}
}

void spider_header (http_request_t * request)
{
	spider_t * spider = (spider_t *)http_request_get_user_data(request);
	struct _http_head_ * head = http_request_header(request);
	int rtcode = http_request_recode(request);

	/* 需要跳转 */ 
	switch(rtcode)
	{
	case 302:
		todo_insert(spider->urls, http_header_getkey(head, "Location", 0));
		spider_reqnext(spider);
		http_request_close(request);
		break;
	case 200:
		break;
	}
}

void spider_errors (http_request_t * request)
{
	printf("spider errors\n");
	spider_t * spider = (spider_t *)http_request_get_user_data(request);
	http_request_close(request);
	spider_reqnext(spider);
}

void spider_reqnext(spider_t * spider)
{
	const char * url = todo_one(spider->urls);
	printf("%s\n", url);
	queue_dequeue(spider->buffer, queue_size(spider->buffer));

	if(url == 0)
	{
		spider_quit(spider);
		return ;
	}

	http_request_t * request = 0;

	do
	{
		todo_insert(spider->history, url);
		request = http_get(spider->http, url);

		todo_insert(spider->history, url);
		todo_remove(spider->urls);
		url = todo_one(spider->urls);
	}while(request == 0 && url != 0);

	if(request == 0)
	{
		spider_quit(spider);
		return ;
	}

	http_request_set_user_data(request, spider);
}

