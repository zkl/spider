#include <stdio.h>
#include "strapi.h"
#include "http.h"
#include "url.h"

static void http_header_init(struct http_head * head);
static void http_header_delete(struct http_head * head);
static void http_header_write_head(http_t * http, http_request_t * request);
static void http_header_set_retcode(http_request_t * request, char * buf);
static void http_header_set_retword(http_request_t * requset, char * buf);

static void http_header_set_keyword(struct http_head * head, const char * key, 
	const char * val, int override);
static struct http_ketword_t * http_header_get_keyword(struct http_head * head, const char * key, 
	struct http_ketword_t * current);


/*******************************************************************************
** ��  ���� v 1.1     
** ��  �ܣ� ����HTTPЭ�鴦��ģ��
** ��  �Σ� http    - HTTPģ��ָ���ָ�룬 ����
			network - ����ģ��ָ��

** ����ֵ�� void
** ��  ע�� ʹ����ɺ���Ҫ����http_delete������Դ�ͷ�
*******************************************************************************/
void http_create(http_t ** http, network_t * network)
{
	*http = (http_t *)malloc(sizeof(http_t));
	memset(*http, 0, sizeof(http_t));
	
	http_header_init(&(*http)->head);
	(*http)->network = network;
	
	http_header_set_keyword(&(*http)->head, "Host", "", 0);
	http_header_set_keyword(&(*http)->head, "Accept", "*/*", 0);
	http_header_set_keyword(&(*http)->head, "Accept-Charset", "GB2312", 0);
	http_header_set_keyword(&(*http)->head, "User-Agent", "Mozilla/5.0", 0);
	http_header_set_keyword(&(*http)->head, "Connection", "Keep-Alive", 0);
	http_header_set_keyword(&(*http)->head, "Keep-Alive", "2000", 0);
}

void http_delete(http_t * http)
{
	http_header_delete(&http->head);
	free(http);
}

void http_config(http_t * http, http_config_t cmd, void * parms)
{
	switch(cmd)
	{
	case SET_HEADER:
		{
			struct parms_t {char * key; char * val; int overrite;};
			struct parms_t * p = (struct parms_t *)parms;
			http_header_set_keyword(&http->head, p->key, p->val, p->overrite);
			break;
		}
	}
}

static void http_header_init(struct http_head * head)
{
	memset(head, 0, sizeof(struct http_head));
}

static void http_header_delete(struct http_head * head)
{
	for(int i=0; i<head->last_keyword; i++)
	{
		free(head->words[i].key);
		free(head->words[i].value);
	}

	free(head->words);
}

/*******************************************************************************
** ��  ���� v 1.1     
** ��  �ܣ� ����HTTP����ͷ����
** ��  �Σ� http - HTTPģ��ָ���ָ�룬 ����
			t    - ����ͷ�ֶα�ʶ��
			val  - ����ͷ�ֶε�����

** ����ֵ�� void
** ��  ע�� 
*******************************************************************************/
static void http_header_set_keyword(struct http_head * head, const char * key, 
							 const char * val, int override)
{
 // printf("key = %s : val = %s\n", key, val);

	if(override)
	{
		struct http_ketword_t * kw = http_header_get_keyword(head, key, 0);
		if(kw != 0)
		{
			free(kw->value);
			kw->value = strdup(val);
			return ;
		}
	}

	if(head->last_keyword == head->max_keyword)
	{
		head->max_keyword += 20;
		head->words = (struct http_ketword_t *)realloc(head->words,
			sizeof(struct http_ketword_t) * head->max_keyword);
	}

	head->words[head->last_keyword].key   = strdup(key);
	head->words[head->last_keyword].value = strdup(val);
	head->last_keyword++;
}


struct http_ketword_t * http_header_get_keyword(struct http_head * head, const char * key, 
	struct http_ketword_t * current)
{
	if(current == 0)
		current = head->words;

	int index = current - head->words;

	if(index < 0)
		return 0;

	for(index; index < head->last_keyword; index++)
	{
		if(stricmp(head->words[index].key, key) == 0)
			return head->words + index;
	}

	return 0;
}

/*******************************************************************************
** ��  ���� v 1.1     
** ��  �ܣ� ����GET����
** ��  �Σ� http    - HTTPģ��ָ���ָ�룬 ����
			urlstr  - ��NULL��β���ַ�����ַ

** ����ֵ�� ָ������ģ���ָ��
** ��  ע�� 
*******************************************************************************/
http_request_t * http_get (http_t * http, const char * urlstr)
{
	url_t * url;
	url_create(&url, urlstr);
	if(url->port == 0)
		url->port = 80;

	net_socket_t * netsocket = network_connect(http->network, url->host, url->port);
	if(netsocket == 0)
		return 0;

	http_request_t * request = (http_request_t *)malloc(sizeof(http_request_t));
	memset(request, 0, sizeof(http_request_t));
	
	request->netsocket = netsocket;
	
	http_header_set_keyword(&http->head, "Host", url->host, 1);

	char buf[2048];
	sprintf(buf, "GET %s HTTP/1.1\r\n", url->file);

	net_socket_write(request->netsocket, buf, strlen(buf));
	http_header_write_head (http, request);
	return request;
}

http_request_t * http_post(http_t * http, const char * urlstr, http_callback_read read, void * handle)
{
	return 0;
}

struct http_head * http_request_header(http_request_t * request)
{
	char buf[2048];
	
	if(request->read_end_head)
		return &request->head;

	int size = net_socket_size(request->netsocket);
	char * ps= net_socket_data(request->netsocket);

	while(request->read_end_head == 0 && getsubstr(buf, 2048, ps, '\n') )
	{
		net_socket_pop(request->netsocket, strlen(buf)+1);

		rtrimchr(buf, '\r');

		if(request->read_ret == 0)
		{
			http_header_set_retcode(request, buf);
		}
		else
		{
			http_header_set_retword(request, buf);
		}
 	}

	http_ketword_t * key = http_header_get_keyword(&request->head, "Content-Length", 0);
	if(key != 0)
		request->left_data = atoi(key->value);

	if(request->read_end_head)
		return &request->head;
	else
		return 0;
}

int http_request_statu (http_request_t * request)
{
	return net_socket_statu(request->netsocket);
}

void http_request_close (http_request_t * request)
{
	http_header_delete(&request->head);
	net_socket_close(request->netsocket);
	if(request->message)
		free(request->message);

	free(request);
}

/*******************************************************************************
** ��  ���� v 1.1     
** ��  �ܣ� ��ȡHTTPЭ������
** ��  �Σ�request - ����ģ��ָ��
		   buf     - ��������ַ
		   size    - ��ȡ��С
           
** ����ֵ�� �ɹ�����0�� ʧ�ܷ��� SOCKET_ERROR                             
** ��  ע�� ��Ҫ����
*******************************************************************************/
int http_request_read(http_request_t * request, char * buf, int size)
{
	struct http_head * head = http_request_header(request);
	if(head == 0)
		return 0;
	
	int left_size = request->left_data;
	int buff_size = net_socket_size(request->netsocket);

	size = size > left_size? left_size : size;
	size = size > buff_size? buff_size : size;
	
	if(size > 0)
	{
		memcpy(buf, net_socket_data(request->netsocket), size);
		net_socket_pop(request->netsocket, size);
	}

	request->left_data -= size;
	if(request->left_data == 0)
	{
		http_ketword_t * key = http_header_get_keyword(&request->head, "Content-Length", 0);
		if(key == 0)
		{
			char len[1024];
			char * ps= net_socket_data(request->netsocket);
			if(getsubstr(len, 1024, ps, '\n') != 0)
			{
				net_socket_pop(request->netsocket, strlen(len)+1);
				request->left_data = strtol(len, 0, 16);
				
				if(request->left_data == 0)
					return -1;
			}
		}
		else
		{
			//printf("���ݶ�ȡ���\n");
			return -1;
		}
	}
	
	return size;
}

int http_request_recode(http_request_t * request)
{
	return request->ret_code;
}

char * http_header_getkey(struct http_head * header, const char * key, int n)
{
	struct http_ketword_t * wd = http_header_get_keyword(header, key, 0);
	for(int i=1; i<n; i++)
	{
		wd = http_header_get_keyword(header, key, wd);
		if(wd == 0)
			break;
	}

	return wd ? wd->value : 0;
}

static void http_header_set_retcode(http_request_t * request, char * buf)
{
	request->read_ret = 1;

	char version[20];	
	char retcode[20];
	
	const char * ps = getsubstr(version, 20, buf, ' ');
	if(ps == 0)
		return ;

	//printf("�汾�� %s\n", version);

	buf += (ps-buf);
	ltrimchr(buf, ' ');
	ps = getsubstr(retcode, 20, buf, ' ');

	request->ret_code = atoi(retcode);

	//printf("������ %s\n", retcode);

	buf += (ps-buf);
	ltrimchr(buf, ' ');

	request->message = strdup(buf);
	//printf("������ %s\n", buf);
}


static void http_header_set_retword(http_request_t * requset, char * buf)
{
	char key[1024];
	
	if(strlen(buf) <= 0) // ���У��������
	{
		requset->read_end_head = 1;
		return ;
	}

	const char * ps = getsubstr(key, 1024, buf, ':');
	if(ps == 0)
		return ;

	// printf("�ؼ��� %s\n", key);

	buf += (ps-buf);
	ltrimchr(buf, ' ');
	
	// printf("ֵ [%s]\n", buf);

	http_header_set_keyword(&requset->head, key, buf, 0);
}

void http_header_write_head(http_t * http, http_request_t * request)
{
	char buf[2048];
	for(int i=0; i<http->head.last_keyword; i++)
	{
		if(http->head.words[i].value == 0)
			break;

		sprintf(buf, "%s: %s\r\n",http->head.words[i].key, http->head.words[i].value);
		net_socket_write(request->netsocket, buf, strlen(buf));
	}

	strcpy(buf, "\r\n");
	net_socket_write(request->netsocket, buf, strlen(buf));
}
