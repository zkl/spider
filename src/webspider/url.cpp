#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "url.h"

void url_create(url_t ** url, const char * urlstr)
{
	int len = strlen(urlstr) + 10;
	*url = (url_t *)malloc(sizeof(url_t));
	
	(*url)->port = 0;
	(*url)->buf  = (char *)malloc(len);
	
	const char * ps, * pe;
	char * buf = (*url)->buf;
	memset(buf, 0, len);

	pe = urlstr;
	ps = strstr(pe, "://"); // 处理协议部分
	(*url)->protocol = buf;
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
	(*url)->host = buf;
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


	(*url)->file = buf;
	strcpy(buf, pe);
	if(*buf == 0)
		strcpy(buf, "/");


	buf = strchr((*url)->host, ':');
	if(buf != 0)
	{
		*buf = 0;
		(*url)->port = atoi(buf+1);
	}
}

void url_delete(url_t * url)
{
	free(url->buf);
	free(url);
}