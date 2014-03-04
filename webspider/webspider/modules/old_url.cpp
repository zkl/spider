#include "url.h"
#include "mstring.h"

/**************************************************************
** 版本:              v 1.0                                   
** 功能: 将不规范的URL转换为规范的URL(默认使用HTTP协议)    
**
** 备注: urlval长度最大比urlstr大8(strlen("http://")+strlen("/"))
**       urlval和urlstr不能使用同一个字符数组
**
***************************************************************/
void hurl_format_hurl(char *urlval, const char *urlstr)
{
	*urlval = 0;
	const char *path;
	const char *host = strstr(urlstr, "://");

	if(host == NULL)
		strcpy(urlval, "http://");

	strcat(urlval, urlstr);

	if(host == NULL)
		path = strchr(urlstr, '/');
	else
		path = strchr(host+3, '/');

	if(path == NULL)
		strcat(urlval, "/");
}

/**************************************************************
** 版本:              v 1.0                                   
** 功能: 解析字符串的url到url结构体，
**           其中结构体URL中的procotol可能为空    
** 返回: 
** 备注: 
**                                                            
***************************************************************/
URL * url_new_url(const char *urlstr)
{
	URL * url = (URL *)malloc(sizeof(URL));
	const char *host = strstr(urlstr, "://");
	const char *path;

	memset(url, 0, sizeof(URL));
	
	if(host != NULL)
	{
		url->procotol = (char *)malloc(host-urlstr+1);
		strncpy_l2(url->procotol, urlstr, host-urlstr);
		host += 3;
	}
	else
	{
		host = urlstr;
	}

	path = strchr(host, '/');
	if(path != NULL)
	{
		url->argvs = malloc_str(path, -1);
		url->host = (char *)malloc(path-host+1);
		strncpy_l2(url->host, host, path-host);
	}
	else
	{
		url->host = malloc_str(host, -1);
	}
	return url;
}


/**************************************************************
** 版本:              v 1.0                                   
** 功能: 释放URL结构体占用的资源
** 备注: 无错误处理
**                                                            
***************************************************************/
void url_del_url(URL *url)
{
	free(url->host);
	free(url->argvs);
	free(url->procotol);
	free(url);
}


/**************************************************************
** 版本:              v 1.0                                   
** 功能: 设置URL结构体中host成员
** 备注: 
**                                                            
***************************************************************/
void url_set_host(URL *url, char *host)
{
	free(url->host);
	url->host = malloc_str(host, -1);
}
void url_set_proc(URL *url, char *proc)
{
	free(url->procotol);
	url->procotol = malloc_str(proc, -1);
}
void url_set_argv(URL *url, char *argvs)
{
	free(url->argvs);
	url->argvs = malloc_str(argvs, -1);
}

HTTP_URL * hurl_new_hurl()
{
	HTTP_URL * http = (HTTP_URL *)malloc(sizeof(HTTP_URL));
	memset(http, 0, sizeof(HTTP_URL));
	return http;
}


void hurl_set_method (HTTP_URL * url, char *method)
{
	url->method = malloc_str(method, -1);
}

void hurl_set_version(HTTP_URL * url, short hv, short lv)
{
	url->version.hv = hv;
	url->version.lv = lv;
}

void hurl_set_pagrvs (HTTP_URL * url, char *path)
{
	free(url->path);
	url->path = malloc_str(path, -1);
}

void hurl_set_keyval (HTTP_URL * url, char * key, char * val)
{
	int i;
	for(i=0; i<url->keynum; i++)
	{
		if(!strcmp(url->keys[i]->key, key))
		{
			if(url->keys[i]->val)
				free(url->keys[i]->val);
			url->keys[i]->val = malloc_str(val, -1);
			return ;
		}
	}
	if(url->keynum == url->maxkeynum)
	{
		CKEY ** tmp = url->keys;
		url->maxkeynum += 1024;
		
		url->keys = (CKEY **)malloc(sizeof(CKEY *) * url->maxkeynum );
		//memset(url->keys, 0, url->maxkeynum*sizeof(CKEY*));
		memcpy(url->keys, tmp, url->keynum);
	}

	url->keys[url->keynum] = (CKEY *)malloc(sizeof(CKEY));
	url->keys[url->keynum]->key = malloc_str(key, -1);
	url->keys[url->keynum]->val = malloc_str(val, -1);
	url->keynum++;
}

void hurl_del_keyval (HTTP_URL * url, char * key)
{
	int i = 0;
	int cps = 0;
	for(i=0; i<url->keynum; i++)
	{
		if(cps != i)
			url->keys[cps] = url->keys[i];

		if(cps == i && !strcmp(url->keys[i]->key, key))
		{
			free(url->keys[i]->key);
			free(url->keys[i]->val);
			free(url->keys[i]);
			cps--;
		}
		cps++;
	}
	url->keynum--;
}

void hurl_del_hurl(HTTP_URL * url)
{
	int i=0;
	for(i; i<url->keynum; i++)
	{
		free(url->keys[i]->key);
		free(url->keys[i]->val);
		free(url->keys[i]);
	}
	free(url->keys);
	free(url->path);
	free(url->method);
	memset(url, 0, sizeof(HTTP_URL));
}

void hurl_sed_hurl(HTTP_URL * url, std_bufot cbfun, void * parm)
{
	char request[8192];

	sprintf(request, "%s %s HTTP/%d.%d\r\n", url->method, url->path, url->version.hv, url->version.lv);
	cbfun(request, strlen(request), parm);

	
	for(int i=0; i<url->keynum; i++)
	{
		sprintf_s(request, 8192, "%s: %s\r\n", url->keys[i]->key, url->keys[i]->val);
		cbfun(request, strlen(request), parm);
	}

	cbfun("\r\n", 2, parm);
}

HTTP_DATA_HEADER * hurl_new_hhead()
{
	HTTP_DATA_HEADER * head = (HTTP_DATA_HEADER *)malloc(sizeof(HTTP_DATA_HEADER));
	memset(head, 0, sizeof(HTTP_DATA_HEADER));
	return head;
}

const char * hurl_get_keyval(HTTP_DATA_HEADER * head, const char * key)
{
	int i = 0;
	char *lkey = malloc_str(key, -1);
	strtolower(lkey, key);

	for(i=0; i<head->keynum; i++)
	{
		if(!strcmp(head->keys[i]->key, lkey))
		{
			free(lkey);
			return head->keys[i]->val;
		}
	}
	free(lkey);
	return 0;
}

void hurl_del_hhead(HTTP_DATA_HEADER * head)
{
	int i=0;
	for(i; i<head->keynum; i++)
	{
		free(head->keys[i]->key);
		free(head->keys[i]->val);
		free(head->keys[i]);
	}
	free(head->keys);
	memset(head, 0, sizeof(HTTP_DATA_HEADER));
	free(head);
}

void hurl_rcv_hhead(HTTP_DATA_HEADER * head, std_bufot cbfun, void * parm)
{
	char buf[2048];

	/* 读取第一行，处理HTTP协议版本和返回码 */
	int  rcnum = getline(buf, 2048, cbfun, parm);
	buf[rcnum] = 0;

	strdelentry(buf, buf, " \r\n");
	if(*buf == 0)
		return ;

	const char *ptr_str = strchr(buf, '/');
	if(ptr_str == 0)
		return ;

	char version[30];
	char hver[10];
	char lver[10];
	char rtcd[10];

	const char *subkey;
	strgetfiled(version, 30, &ptr_str, ptr_str+1, " ");
	strgetfiled(hver, 10, &subkey, version, ".");
	strgetfiled(lver, 10, 0, subkey, ".");
	strgetfiled(rtcd, 10, 0, ptr_str, " ");

	head->version.hv = (short)strtol(hver, 0, 10);
	head->version.lv = (short)strtol(lver, 0, 10);
	head->retCode    = (short)strtol(rtcd, 0, 10);

	/* 处理HTTP协议头的其他数据 */
	while(1)
	{
		rcnum = getline(buf, 2048, cbfun, parm);
		buf[rcnum] = 0;
		if(rcnum == 0)
			break ;

		strdelentry(buf, buf, " \r\n");
		if(*buf == 0)
			return ;

		if(*buf == 0)
			break;

		char key[1024];
		const char *val;

		/* 新增HEAD的键值对 */
		if(head->keynum == head->maxkeynum)
		{
			CKEY ** tmp = head->keys;
			head->maxkeynum += 1024;

			head->keys = (CKEY **)malloc(sizeof(CKEY *) * head->maxkeynum );
			memcpy(head->keys, tmp, head->keynum);
		}
		head->keys[head->keynum] = (CKEY *)malloc(sizeof(CKEY));
		
		strgetfiled(key, 1024, &val, buf, ":");
		strtolower(key, key);
		strdelentry(key, key, " \r\n");
		head->keys[head->keynum]->key = malloc_str(key, -1);

		strdelentry(key, val+1, " \r\n");
		head->keys[head->keynum]->val = malloc_str(key, -1);
		head->keynum++;
		/***************************/
	}
}

void hurl_rcv_hdata(HTTP_DATA_HEADER * head, std_bufot cbrecv, void * rparm, std_bufot cbsave, void * sparm)
{
	char buf[2049] = {0};
	const char * len = hurl_get_keyval(head, "content-length");
	const char * code= hurl_get_keyval(head, "transfer-encoding");
	
	if(len == NULL || strtol(len, 0, 10) == 0)
	{
		if(code == NULL)
			return ;

		if(strcmp(code, "chunked"))
			return ;

		int  len;
		int  rcnum;
		while(1)
		{
			rcnum = getline(buf, 2048, cbrecv, rparm);
			if(rcnum < 1)
				return ;

			if((len = strtol(buf, 0, 16)) <= 0)
				break;

			while(len > 2048)
			{
				cbrecv(buf, 2048, rparm);
				cbsave(buf, 2048, sparm);
				len -= 2048;
			}

			buf[len] = 0;
			cbrecv(buf, len, rparm);
			cbsave(buf, len, sparm);
			getline(buf, 2048, cbrecv, rparm); // 跳过最后的换行
		}
		cbsave(0, 0, sparm);
	}
	else
	{
		int  size = strtol(len, 0, 10);
		while(size > 2048)
		{
			cbrecv(buf, 2048, rparm);
			cbsave(buf, 2048, sparm);
			size -= 2048;
		}
		cbrecv(buf, size, rparm);
		cbsave(buf, size, sparm);
		cbsave(0, 0, sparm);
	}
}

void hurl_mkHostAddr(sockaddr_in *addr_in, char *hostName, int port)
{
	struct hostent *host = gethostbyname (hostName);

	addr_in->sin_family = AF_INET;
	addr_in->sin_port   = htons(port);

	if(host && hostName)
	{
		char *p = host->h_addr_list[0];
		if(p)
			addr_in->sin_addr.S_un.S_addr = *(ULONG *)p;
	}
	else
		addr_in->sin_addr.S_un.S_addr = INADDR_ANY;

}

