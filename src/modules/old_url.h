/**************************************************************/
/*                                                            */
/* ��    ��:            v1.1.3                                */
/* ��������:      Wed  8 Aug 2012                             */
/* �޸�����:      Mon  8 Otc 2012                             */
/* ��    Ȩ:                                                  */
/* ��    ע:  HTTPЭ�����                                    */
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

typedef struct _key_value_     // Э��ļ�ֵ�Ľṹ��
{
	char * key;
	char * val;
}CKEY;

typedef struct _url_           // ͨ��URl�ṹ�嶨��
{
	char *host;
	char *argvs;
	char *procotol;
}URL;

typedef struct _http_url_       // HTTPЭ������ṹ��
{
	VER   version;
	short keynum;
	short maxkeynum;

	char *method;
	char *path;
	CKEY **keys;
}HTTP_URL;

typedef struct _http_data_header_ /* HTTP����Э��ͷ�Ľṹ�� */
{
	int   retCode;
	VER   version;
	short keynum;
	short maxkeynum;

	CKEY  **keys;
}HTTP_DATA_HEADER;


/* ͨ��URL���� */

URL * url_new_url(const char *urlstr);       /* ����һ��url��ַ��������һ��URL�ṹ�� */
void  url_del_url(URL * url);                /* �ͷ���Դ */
void  url_set_host(URL * url, char *host);   /* ����URL�ķ����� */
void  url_set_proc(URL * url, char *proc);   /* ����Э������ */
void  url_set_argv(URL * url, char *argv);   /* ���ò�����·�� */


/* HTTP������ز��� */

HTTP_URL * hurl_new_hurl(void);
void hurl_format_hurl(char *urlval, const char *urlstr);             /* �����淶��URLת��Ϊ�淶��URL��Ĭ��ʹ��HTTPЭ�飩*/
void hurl_set_version(HTTP_URL * url, short hv, short lv);           /* ����HTTPЭ��汾�� */
void hurl_set_pagrvs (HTTP_URL * url, char *path);                   /* ���������·���Ͳ��� */
void hurl_set_method (HTTP_URL * url, char *method);                 /* �������󷽷� */
void hurl_set_keyval (HTTP_URL * url, char * key, char * val);       /* ���ü�ֵ�� */
void hurl_del_keyval (HTTP_URL * url, char * key);                   /* ɾ����ֵ�� */
void hurl_del_hurl(HTTP_URL * url);                                  /* �ͷ���Դ */

#define std_bufot void *
void hurl_sed_hurl(HTTP_URL * url, std_bufot cbfun, void * parm);    /* �������� */


/* HTTP���ݷ��ز��� */

HTTP_DATA_HEADER * hurl_new_hhead();
const char * hurl_get_keyval(HTTP_DATA_HEADER * head, const char * key);
void hurl_del_hhead(HTTP_DATA_HEADER * head);
void hurl_rcv_hhead(HTTP_DATA_HEADER * head, std_bufot cbfun, void * parm);
void hurl_rcv_hdata(HTTP_DATA_HEADER * head, std_bufot cbrecv, void * rparm, std_bufot cbsave, void * sparm);

void hurl_mkHostAddr(sockaddr_in *add, char *hostName, int port);

#endif