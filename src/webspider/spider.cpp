#include <stdio.h>
#include "url.h"
#include "pcre/inc/pcre.h"
#include "spider.h"
#include "queue.h"

/* ½âÎöµØÖ· */
static void   spider_anlize(spider_t * spider, queue_t * mbuf, const char * url);
static char * spider_format(char * furl, char * url, const char * host);
static int    spider_is_text(const char * type);

static void spider_anlize(spider_t * spider, queue_t * mbuf, const char * url)
{
	int ret;
	int  ovector[100];
	
	char * html = queue_data(mbuf);
	int len = queue_size(mbuf);

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

void spider_init(spider_t ** spider)
{
	const char *error;  
	int erroffset;  

	*spider = (spider_t *)malloc(sizeof(spider_t));
	memset(*spider, 0, sizeof(spider_t));

	(*spider)->imgage = pcre_compile("src[\\s]*=[\\s]*\\\".*?\"", 0, &error, &erroffset, 0); 
	(*spider)->link   = pcre_compile("href[\\s]*=[\\s]*\\\".*?\"", 0, &error, &erroffset, 0); 

	network_create(&(*spider)->network);
	(*spider)->http = http_create((*spider)->network);

	todo_create(&(*spider)->history);
	todo_create(&(*spider)->images);
	todo_create(&(*spider)->urls);
}

void spider_config(spider_t * spider, int cmd, void * parm)
{
}


void spider_start(spider_t * spider, const char * first_host)
{
	queue_t mbuf;
	queue_init(&mbuf, 8192, 1024);


	const char * url = first_host;
	todo_insert(spider->urls, first_host);
	todo_insert(spider->history, first_host);

	while(1)
	{
		url = todo_one(spider->urls);
		if(url == 0)
			break;
		
		http_request_t * request = http_get(spider->http, url);
	
		while(request)
		{
			int ret = network_procmsg(spider->network);

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
			
			if(type && spider_is_text(type) == 0)
				break;
			
			char buf[1028] = {0};
			ret = http_request_read(request, buf, 1024);

			if(ret > 0)
				queue_enqueue(&mbuf, buf, ret);
			
			spider_anlize(spider, &mbuf, url);
			
			if(ret < 0)
				break;

			if(http_request_statu(request) <= 0)
					break;
		}

		if(request)
			printf("[done] [%d] %s\n", http_request_recode(request), url);
		else
			printf("[erro] [  ] %s\n", url);

		todo_remove(spider->urls);

		http_request_close(request);
	}

	printf("All done\n");
	queue_destroy(&mbuf);
}

void spider_delete(spider_t * spider)
{
	pcre_free(spider->imgage);
	pcre_free(spider->link);
	http_free(spider->http);
	network_delete(spider->network);

	todo_free(spider->history);
	todo_free(spider->images);
	todo_free(spider->urls);

	free(spider);
}



