/*******************************************************************************
**                                                  
** ��    ��:  v 1.0.0                                                  
** ��    ��:  �����������ӣ���Ҫ��������ͻ���
** ��    ��:  Wed. 26 Feb 2014   �޸��� Wed. 26 Feb 2014 
** ��    Ȩ:  
** ��    ��:  ������ 
** ��    ע:                                              
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