#ifndef HTTP_H
#define HTTP_H

#include "network.h"

struct http_ketword_t
{
	char * key;
	char * value;
};

struct http_head
{
	int max_keyword;
	int last_keyword;
	struct http_ketword_t * words;
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

typedef enum 
{
	SET_HEADER,  // 参数定义 struct parms_t {char * key; char * val; int overrite;};
}http_config_t;

struct http_config_header_t
{
	char * key;
	char * val;
	int overrite;
};

typedef int (*http_callback_read)(void * handle, char * buf, int size);

void http_create(http_t ** http, network_t * network); // 【1】
void http_delete(http_t * http);  // 【6】
void http_config(http_t * http, http_config_t cmd, void * parms);

http_request_t * http_get (http_t * http, const char * urlstr); // 【3】
http_request_t * http_post(http_t * http, const char * urlstr, http_callback_read read, void * handle);// 【3】

struct http_head * http_request_header(http_request_t * request);   // 【4】
int                http_request_statu (http_request_t * request);
void               http_request_close (http_request_t * request);   // 【5】
int                http_request_read  (http_request_t * request, char * buf, int len);   // 【4】
int                http_request_recode(http_request_t * request);

void   http_header_version(struct http_head * header);   // 【4】
char * http_header_getkey(struct http_head * header, const char * key, int n);    // 【4】

#endif
