#include <Windows.h>
#include "modules/mstring.h"
#include "dns.h"

void dns_create(dns_t ** dns, int size)
{
	*dns = (dns_t *)malloc(sizeof(dns_t));
	memset(*dns, 0, sizeof(dns_t));
	
	(*dns)->max_size = size;
	linked_list_create(&(*dns)->list);
}

unsigned long dns_host_ip(dns_t * dns, const char * host)
{
	dns_node_t * dnode = 0;

	linked_list_node_t * lnode = linked_list_first(dns->list);

	for(lnode; lnode; lnode = linked_list_next(lnode))
	{
		dnode = (dns_node_t *)linked_list_data(lnode);
		if(!strcmp(dnode->host, host))
			return dnode->ip;
	}


	struct hostent * hns = gethostbyname (host);

	if(hns)
	{
		char *ip = hns->h_addr_list[0];
		if(ip)
		{
			dnode = (dns_node_t *)malloc(sizeof(dns_node_t));

			dnode->host = strdup(host);
			dnode->ip = *(ULONG *)ip;
			
			linked_list_insert(dns->list, 0, dnode);

			if(linked_list_size(dns->list) > dns->max_size)
			{
				lnode = linked_list_last(dns->list);

				free(linked_list_data(lnode));

				linked_list_remove(dns->list, lnode);
			}

			return dnode->ip;
		}	
	}
	return 0;
}

void dns_delete(dns_t *  dns)
{
	linked_list_delete(dns->list);
	free(dns);
}