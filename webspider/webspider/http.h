#ifndef HTTP_H
#define HTTP_H

#include "network.h"

struct http_ketword 
{
	char * key;
	char * value;
};

struct http_head
{
	int max_keyword;
	int last_keyword;
	struct http_ketword * words;
};

typedef struct 
{
	network_t * network;
	struct http_head head;
}http_t;


typedef struct 
{
	int read_ret;
	int read_end_head;
	long left_data;

	int ret_code;
	char * message;
	struct http_head head;
	net_socket_t * netsocket;
}http_request_t;

typedef int (*http_callback_read)(void * handle, char * buf, int size);

void             http_create(http_t ** http, network_t * network); // ¡¾1¡¿
void             http_delete(http_t * http);  // ¡¾6¡¿

void             http_config(http_t * http, int cmd, void  * val);

http_request_t * http_get (http_t * http, const char * urlstr); // ¡¾3¡¿
http_request_t * http_post(http_t * http, const char * urlstr, http_callback_read read, void * handle);// ¡¾3¡¿

struct http_head * http_request_header(http_request_t * request);   // ¡¾4¡¿
int              http_request_statu (http_request_t * request);
void             http_request_close (http_request_t * request);   // ¡¾5¡¿
int              http_request_read(http_request_t * request, char * buf, int len);   // ¡¾4¡¿

void             http_header_version(struct http_head * header);   // ¡¾4¡¿
void             http_header_getkey(struct http_head * header, const char * key);    // ¡¾4¡¿

#endif