#ifndef SPIDER_H
#define SPIDER_H

#include <pcre.h>
#include "http.h"
#include "todolist.h"

typedef struct 
{
	int goon;
	todo_list_t * history;
	todo_list_t * images;
	todo_list_t * urls;

	network_t * network;
	http_t * http;
	pcre * imgage;
	pcre * link;
	pcre * title;
	queue_t * buffer;
}spider_t;


spider_t * spider_create();
void       spider_config(spider_t * spider, unsigned int cmd, void * parm);
void       spider_exec(spider_t * spider, const char * start_host);
void       spider_quit(spider_t * spider);
void       spider_free(spider_t * spider);
  

#endif
