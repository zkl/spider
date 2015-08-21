/*******************************************************************************
**                                                  
** 版    本:  v 1.0.0                                                  
** 功    能:  管理网络连接，主要用于网络客户端
** 创    期:  Wed. 26 Feb 2014   修改于 Wed. 26 Feb 2014 
** 版    权:  
** 作    者:  张昆伦 
** 备    注:                                              
*******************************************************************************/

#ifndef SPD_URL_H
#define SPD_URL_H

typedef struct 
{
	int    port;
	char * buf;
	char * protocol;
	char * host;
	char * file;
}url_t;

url_t * spd_url_create(const char * url_string);
void spd_url_delete(url_t * url);

#endif