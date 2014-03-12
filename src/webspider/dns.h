/*******************************************************************************
**                                                  
** 版    本:  v 1.0.0                                                  
** 功    能: 
** 创    期:  Sat. 3 Mar 2012  修改于 Wed. 26 Feb 2014 
** 版    权:  
** 作    者:  张昆伦 
** 备    注:                                              
*******************************************************************************/

#ifndef DNS_H
#define DNS_H

#include "modules/list.h"

typedef struct _dns_node_
{
	char * host;
	unsigned long ip;
}DNS_NODE, dns_node_t;

typedef struct _dns_t_
{
	int max_size;
	linked_list_t * list;
}DNS_COM_DATA, dns_t;

void dns_create(dns_t ** dns, int size);
void dns_delete(dns_t * dns);

unsigned long dns_host_ip(dns_t * dns, const char * host);


#endif