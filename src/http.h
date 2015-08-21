#ifndef HTTP_H
#define HTTP_H

#include "network.h"

struct _http_keyword_t_
{
	char * key;
	char * value;
};

struct _parms_t_;
struct _http_head_;
typedef struct _http_t_         http_t;
typedef struct _http_request_t_ http_request_t;

typedef void (*data_event)(http_request_t * request);

struct _http_head_
{
	int max_keyword;
	int last_keyword;
	struct _http_keyword_t_ * words;
};

struct _parms_t
{
	char * key; 
	char * val; 
	int overrite;
};

typedef struct _http_t_
{
	network_t * network;
	struct _http_head_ head;
	data_event recv_event;
	data_event send_event;
	data_event head_recved;
	data_event error_event;
}http_t;


typedef struct _http_request_t_
{
	int read_ret;
	int read_end_head;
	long left_data;

	int ret_code;
	int valid;
	char * message;
	void * user_data;
	struct _http_t_  * http;
	struct _http_head_ head;
	net_socket_t * netsocket;

	data_event recv_event;
	data_event send_event;
	data_event head_recved;
	data_event error_event;
}http_request_t;

typedef enum 
{
	HTTP_SET_HEADER,	    // 参数定义struct parms_t
	HTTP_SET_RECV_EVENT, // 
	HTTP_SET_SEND_EVENT,
	HTTP_SET_HEAD_RECVED,
	HTTP_SET_ERROR_EVENT,
}http_config_t;

struct _http_config_header_t_
{
	char * key;
	char * val;
	int overrite;
};

typedef int (*http_callback_read)(void * handle, char * buf, int size);

http_t *         http_create(network_t * network); // 【1】
void             http_free(http_t * http);  // 【6】
void             http_config(http_t * http, http_config_t cmd, void * parms);
http_request_t * http_get (http_t * http, const char * urlstr); // 【3】
http_request_t * http_post(http_t * http, const char * urlstr, 
						http_callback_read read, void * handle);// 【3】

struct _http_head_ * http_request_header(http_request_t * request);   // 【4】
struct _http_head_ * http_request_send_header(http_request_t * request);   // 【4】
int                  http_request_statu (http_request_t * request);
void                 http_request_close (http_request_t * request);   // 【5】
int                  http_request_read  (http_request_t * request, char * buf, 
						int len);   // 【4】
int                  http_request_recode(http_request_t * request);
void                 http_request_set_user_data(http_request_t * req, void * d);
void *               http_request_get_user_data(http_request_t * req);

void   http_header_version(struct _http_head_ * header);   // 【4】
char * http_header_getkey(struct _http_head_ * header, const char * key, int n);

#endif
