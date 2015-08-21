#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "url.h"

url_t * spd_url_create(const char * url_string)
{
	url_t * url = (url_t *)malloc(sizeof(url_t));
	int len = strlen(url_string) + 10; // @todo 10 增加分隔符的数量
	
	url->port = 0;
	url->buf  = (char *)malloc(len);
	
	const char * ps, * pe;
	char * buf = url->buf;
	memset(buf, 0, len);

	pe = url_string;
	ps = strstr(pe, "://"); // 处理协议部分
	url->protocol = buf;
	if(ps == 0)
	{
		buf++;
	}
	else
	{
		strncpy(buf, pe, ps-pe);
		buf += ps-pe + 1;
		pe = ps + 3;
	}

	ps = strchr(pe, '/');
	url->host = buf;
	if(ps == 0)
	{
		strcpy(buf, pe);
		buf += strlen(pe) + 1;
		pe  += strlen(pe);
	}
	else
	{
		strncpy(buf, pe, ps-pe);
		buf += ps-pe+1;
		pe  += ps-pe;
	}


	url->file = buf;
	strcpy(buf, pe);
	if(*buf == 0)
		strcpy(buf, "/");


	buf = strchr(url->host, ':');
	if(buf != 0)
	{
		*buf = 0;
		url->port = atoi(buf+1);
	}

	return url;
}

void spd_url_delete(url_t * url)
{
	free(url->buf);
	free(url);
}