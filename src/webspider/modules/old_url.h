/**************************************************************/
/*                                                            */
/* 版    本:            v1.1.3                                */
/* 创建日期:      Wed  8 Aug 2012                             */
/* 修改日期:      Mon  8 Otc 2012                             */
/* 版    权:                                                  */
/* 备    注:  HTTP协议解析                                    */
/**************************************************************/

#ifndef URL_H
#define URL_H

#include <windows.h>
#include <stdio.h>
#include <string.h>

typedef struct _version_
{
	short hv;
	short lv;
}VER;

typedef struct _key_value_     // 协议的键值的结构体
{
	char * key;
	char * val;
}CKEY;

typedef struct _url_           // 通用URl结构体定义
{
	char *host;
	char *argvs;
	char *procotol;
}URL;

typedef struct _http_url_       // HTTP协议请求结构体
{
	VER   version;
	short keynum;
	short maxkeynum;

	char *method;
	char *path;
	CKEY **keys;
}HTTP_URL;

typedef struct _http_data_header_ /* HTTP返回协议头的结构体 */
{
	int   retCode;
	VER   version;
	short keynum;
	short maxkeynum;

	CKEY  **keys;
}HTTP_DATA_HEADER;


/* 通用URL操作 */

URL * url_new_url(const char *urlstr);       /* 解析一个url地址，并生成一个URL结构体 */
void  url_del_url(URL * url);                /* 释放资源 */
void  url_set_host(URL * url, char *host);   /* 设置URL的服务器 */
void  url_set_proc(URL * url, char *proc);   /* 设置协议类型 */
void  url_set_argv(URL * url, char *argv);   /* 设置参数和路径 */


/* HTTP请求相关操作 */

HTTP_URL * hurl_new_hurl(void);
void hurl_format_hurl(char *urlval, const char *urlstr);             /* 将不规范的URL转换为规范的URL（默认使用HTTP协议）*/
void hurl_set_version(HTTP_URL * url, short hv, short lv);           /* 设置HTTP协议版本号 */
void hurl_set_pagrvs (HTTP_URL * url, char *path);                   /* 设置请求的路径和参数 */
void hurl_set_method (HTTP_URL * url, char *method);                 /* 设置请求方法 */
void hurl_set_keyval (HTTP_URL * url, char * key, char * val);       /* 设置键值对 */
void hurl_del_keyval (HTTP_URL * url, char * key);                   /* 删除键值对 */
void hurl_del_hurl(HTTP_URL * url);                                  /* 释放资源 */

#define std_bufot void *
void hurl_sed_hurl(HTTP_URL * url, std_bufot cbfun, void * parm);    /* 发送请求 */


/* HTTP数据返回操作 */

HTTP_DATA_HEADER * hurl_new_hhead();
const char * hurl_get_keyval(HTTP_DATA_HEADER * head, const char * key);
void hurl_del_hhead(HTTP_DATA_HEADER * head);
void hurl_rcv_hhead(HTTP_DATA_HEADER * head, std_bufot cbfun, void * parm);
void hurl_rcv_hdata(HTTP_DATA_HEADER * head, std_bufot cbrecv, void * rparm, std_bufot cbsave, void * sparm);

void hurl_mkHostAddr(sockaddr_in *add, char *hostName, int port);

#endif