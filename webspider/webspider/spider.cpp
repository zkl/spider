#include <stdio.h>
#include "pcre/inc/pcre.h"
#include "spider.h"
#include "queue.h"

/* ½âÎöµØÖ· */
static void spider_anlize(spider_t * spider, queue_t * mbuf);

static void spider_anlize(spider_t * spider, queue_t * mbuf)
{
	int ret;
	int  ovector[100];
	
	char * html = queue_data(mbuf);
	int len = queue_size(mbuf);

	ret = pcre_exec(spider->link, 0, html, len, 0, 0, ovector, 100);

	if (ret > 0) {
        char * start = html + ovector[0]; 
        int len = ovector[1] - ovector[0];
       
		printf("%.*s\n", len, start); 
		char * substring_end = start + len;
		queue_dequeue(mbuf, substring_end - html);
	}
	  
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
	http_create(&(*spider)->http, (*spider)->network);
}

void spider_config(spider_t * spider, int cmd, void * parm)
{
}


void spider_start(spider_t * spider, const char * start_host)
{
	queue_t mbuf;
	queue_init(&mbuf, 8192, 1024);

	while(1)
	{
		http_request_t * request = http_get(spider->http, start_host);
	
		while(1)
		{
			int ret = network_procmsg(spider->network);

			if(ret == SOCKET_ERROR)
			{
				printf("socket error\n");
				break;
			}
			
			struct http_head * head = http_request_header(request);
			if(head == 0)
				continue;
			
			char buf[1028] = {0};
			ret = http_request_read(request, buf, 1024);

			if(ret > 0)
				queue_enqueue(&mbuf, buf, ret);
			
			spider_anlize(spider, &mbuf);
			
			if(ret < 0)
				break;
		}

		printf("[done] %s\n", start_host);
		break;
	}

	queue_destroy(&mbuf);
}

void spider_delete(spider_t * spider)
{
	pcre_free(spider->imgage);
	pcre_free(spider->link);
	http_delete(spider->http);
	network_delete(spider->network);
	free(spider);
}



