#ifndef SPIDER_H
#define SPIDER_H

#include "http.h"

typedef struct 
{
	http_t * http;
}spider_t;

void spider_init(spider_t ** spider);
void spider_config(spider_t * spider, int cmd, void * parm);
void spider_start(spider_t * spider, const char * start_host);

#endif