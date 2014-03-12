/*******************************************************************************
**                                                  
** ��    ��:  v 1.0.0                                                  
** ��    ��:  �����������ӣ���Ҫ��������ͻ���
** ��    ��:  Wed. 26 Feb 2014   �޸��� Wed. 26 Feb 2014 
** ��    Ȩ:  
** ��    ��:  ������ 
** ��    ע:                                              
*******************************************************************************/
#ifndef NETWORK_H
#define NETWORK_H

#include <windows.h>
#include "dns.h"
#include "queue.h"
#include "modules/list.h"

typedef struct _network_
{
	int dsp;
	dns_t * dns;
	linked_list_t * net_sockets;
}network_t;

typedef struct _net_socket_
{
	int dsp;
	int statu;
	network_t * network;
	queue_t rdque;
	queue_t wtque;
	linked_list_node_t * linked_node;
}net_socket_t;

void           network_create (network_t ** network); /* ��ʼ�� ����ģ�� */
void           network_delete (network_t * network);
net_socket_t * network_connect(network_t * network, const char * host, int port);
int            network_procmsg(network_t * network); /* ����������Ϣ */


int    net_socket_size (net_socket_t * socket);    // �ɶ����ݴ�С
char * net_socket_data (net_socket_t * socket);    // ���ݵ�ַָ��
char * net_socket_pop  (net_socket_t * socket, int bytes); // ���ݳ���
int    net_socket_write(net_socket_t * socket, const char * buf, int size);
int    net_socket_statu(net_socket_t * socket);
void   net_socket_close(net_socket_t * socket);


#endif