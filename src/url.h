/*******************************************************************************
**                                                  
** ��    ��:  v 1.0.0                                                  
** ��    ��:  �����������ӣ���Ҫ��������ͻ���
** ��    ��:  Wed. 26 Feb 2014   �޸��� Wed. 26 Feb 2014 
** ��    Ȩ:  
** ��    ��:  ������ 
** ��    ע:                                              
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