#include <stdio.h>
#include "url.h"
#include "wget.h"
#include "skt.h"

void dwn_wget(url_t * url, char * file, unsigned long ip)
{
	FILE * f = fopen(file, "wb");
	if(f == 0)
		return ;

	HTTP_URL * hurl;
	HTTP_DATA_HEADER * head;
	SOCKET socketfd;
	sockaddr_in addr_in;
	int len_addr_in = sizeof(sockaddr_in);

	addr_in.sin_family = AF_INET;
	addr_in.sin_port   = htons(80);
	addr_in.sin_addr.S_un.S_addr = ip;

	hurl = hurl_new_hurl();
	head = hurl_new_hhead();

	hurl_set_version(hurl, 1, 1);
	hurl_set_method (hurl, "GET");
	hurl_set_pagrvs (hurl, url->argvs);
	hurl_set_keyval (hurl, "HOST", url->host);
	hurl_set_keyval (hurl, "Accept", "*/*");
	hurl_set_keyval (hurl, "User-Agent", "Mozilla/5.0");
	hurl_set_keyval (hurl, "Connection", "Keep-Alive");
	hurl_set_keyval (hurl, "Keep_Alive", "20");

	socketfd = socket(AF_INET, SOCK_STREAM, 0);

	if(connect (socketfd, (sockaddr *)&addr_in, len_addr_in) != SOCKET_ERROR)
	{
		hurl_sed_hurl(hurl,  skt_send, (void *)socketfd);
		hurl_rcv_hhead(head, skt_recv, (void *)socketfd);
		hurl_rcv_hdata(head, skt_recv, (void *)socketfd, dwn_save, (void *)f);
	}

	hurl_del_hurl (hurl);
	hurl_del_hhead(head);

	fclose(f);
}

int dwn_save(char * buf, int size, void * parm)
{
	int rlen = 0;
	while(rlen < size)
	{
		rlen += fwrite(buf+rlen, sizeof(char), size, (FILE *)parm);
	}
	return 1;
}