/*******************************************************************************
**                                                  
** 版    本:  v 1.1.0                                                
** 功    能:  实现HTTP请求模块
** 创    期:  Wed. 26 Feb 2014   修改于 Thur 3 Mar 2014 
** 版    权:  
** 作    者:  张昆伦 
** 备    注:          
** 更    新:  
*******************************************************************************/

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

typedef int (*http_callback_read)(void * handle, char * buf, int size);


http_t * http_create(network_t * network); /* 创建 HTTP 模块 */
void http_free(http_t * http);    /* 删除 HTTP 模块 */
void http_config(http_t * http, http_config_t cmd, void * parms); /* 配置 HTTP 模块 */

http_request_t * http_get (http_t * http, const char * urlstr); // 【3】
http_request_t * http_post(http_t * http, const char * urlstr, http_callback_read read, void * handle);// 【3】


/* 关于请求返回相关的操作 */
struct http_head * http_request_header(http_request_t * request);   // 【4】
int                http_request_statu (http_request_t * request);
void               http_request_close (http_request_t * request);     // 【5】
int                http_request_read  (http_request_t * request, char * buf, int len);   // 【4】
int                http_request_recode(http_request_t * request);


void   http_header_version(struct http_head * header);   // 【4】
char * http_header_getkey(struct http_head * header, const char * key, int n);    // 【4】

#endif