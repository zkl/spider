#ifndef SPIDER_H
#define SPIDER_H

#include "pcre/inc/pcre.h"
#include "http.h"
#include "todolist.h"

typedef struct 
{
	todo_list_t * history;
	todo_list_t * images;
	todo_list_t * urls;

	network_t * network;
	http_t * http;
	pcre * imgage;
	pcre * link;
}spider_t;

void spider_init(spider_t ** spider);
void spider_config(spider_t * spider, int cmd, void * parm);
void spider_start(spider_t * spider, const char * start_host);
void spider_delete(spider_t * spider);

#endif