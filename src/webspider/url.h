/*******************************************************************************
**                                                  
** 版    本:  v 1.0.0                                                  
** 功    能:  管理网络连接，主要用于网络客户端
** 创    期:  Wed. 26 Feb 2014   修改于 Wed. 26 Feb 2014 
** 版    权:  
** 作    者:  张昆伦 
** 备    注:                                              
*******************************************************************************/

#ifndef URL_H
#define URL_H

typedef struct 
{
	int    port;
	char * buf;
	char * protocol;
	char * host;
	char * file;
}url_t;

void url_create(url_t ** url, const char * urlstr);
void url_delete(url_t * url);

//char * url_protocol(url_t * url);
//char * url_host(url_t * url);
//char * url_file(url_t * url);
//char * url_port(url_t * url);

#endif