#ifndef HTTP_H
#define HTTP_H

#include "network.h"

typedef enum
{
	HTTP_HOST = 0,
	HTTP_ACCEPT,
	HTTP_USER_AGENT,
	HTTP_CONNECTION,
	HTTP_KEEP_ALIVE,
	HTTP_NOUSED
}http_word_t;

/* 定义HTTP协议头中能过包含的最大的字段 */
#define HTTP_MAX_WORD HTTP_NOUSED

struct http_word 
{
	char key[100]; // HTTP字段
	char * val;
};

typedef struct 
{
	network_t * network;
	struct http_word header[HTTP_MAX_WORD];
}http_t;


typedef struct 
{
	net_socket_t * netsocket;
}http_request_t;


void             http_create(http_t ** http, network_t * network); // 【1】
void             http_delete(http_t * http);  // 【6】
void             http_set(http_t * http, http_word_t t, const char * val);// 【2】
http_request_t * http_get (http_t * http, const char * urlstr); // 【3】
http_request_t * http_post(http_t * http, const char * urlstr, const char * data);// 【3】
http_request_t * http_post_big_data(http_t * http, const char * urlstr, const char * data);// 【3】

void             http_request_header(); // 【4】
void             http_request_close();  // 【5】
void             http_request_read();   // 【4】

void             http_header_version(); // 【4】
void             http_header_data_size(); // 【4】
void             http_header_get(); // 【4】


#endif