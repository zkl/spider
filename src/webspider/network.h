/*******************************************************************************
**                                                  
** ��    ��:  v 1.1.0                                                  
** ��    ��:  �����������ӣ���Ҫ��������ͻ���
** ��    ��:  Wed. 26 Feb 2014   �޸��� Thur 3 Mar 2014 
** ��    Ȩ:  
** ��    ��:  ������ 
** ��    ע:          
** ��    ��:  3 Mar 2014 - �����¼��������       
             19 Mar 2014 - ����ע�ͺ�˵��
*******************************************************************************/

/*
void           network_create (network_t ** network); 
void           network_delete (network_t * network);
net_socket_t * network_connect(network_t * network, const char * host, int port);

����������Ϣ����Ҫ�Ǹ�����պͷ���������Ϣ�����һص������¼�
int network_procmsg(network_t * network); 

��������ģ��
void network_config (network_t * network, network_config_t cmd, void * parm);
����˵����
network:
	����ģ��ָ��
cmd
	���������֣������¼��ֿ�ѡֵ
	NET_SET_RECV_EVENT 
		�����������ʱ�Ļص�������parmָ�� void function(network_t * network, 
		net_socket_t * netsocket) ���ͺ���ָ�롣
	NET_SET_SEND_EVENT
		�������緢��ʱ�Ļص�������parmָ�� void function(network_t * network, 
		net_socket_t * netsocket) ���ͺ���ָ�롣
parm 
	����������ָ����Ӧ�Ĳ���


��ȡ������Ϣ���еĴ�С
int net_socket_size (net_socket_t * socket); 

��ȡ������Ϣ�������ݵ��׵�ַ���õ�ַ���Է��ʲ��������ݶ��д�С������
char * net_socket_data (net_socket_t * socket);

ɾ��������Ϣ���ж������ݣ�����������Ϣ���е��׵�ַ����
char * net_socket_pop  (net_socket_t * socket, int bytes);

������д�뵽������Ϣ�����в��з������ݣ����buf�ǿ�ָ�루buf == NULL����ֻ������
��Ϣ�����е����ݷ��ͳ�ȥ
int    net_socket_write(net_socket_t * socket, const char * buf, int size);

��ȡ��ǰ����״̬��0 ���类�ر� -1 ���类�Ͽ� 1 ��������
int    net_socket_statu(net_socket_t * socket);

�ر��������Ӳ����ͷ�����ģ��ռ�õ���Դ
void   net_socket_close(net_socket_t * socket);

����ģ���ṩһ�����԰��û����ݵĽӿ�net_socket_set_user_data���û�����ʹ�øýӿڿ��ٶ�λ����
Ӧ�����ݣ�������������¼��Ĵ���
void   net_socket_set_user_data(net_socket_t * socket, void * d);
void * net_socket_get_user_data(net_socket_t * socket);

*/

#ifndef NETWORK_H
#define NETWORK_H

#include <WINDOWS.H>
#include "dns.h"
#include "queue.h"
#include "modules/list.h"

struct _network_;
struct _net_socket_;

typedef void (*network_event_t)(struct _network_ * network, 
	struct _net_socket_ * netsocket);

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
	int valid;
	network_t * network;
	queue_t * rdque;
	queue_t * wtque;
	linked_list_node_t * linked_node;
	void * user_data;
	network_event_t arrived_callback;
	network_event_t writeed_callback;
	network_event_t error_callback;
}net_socket_t;

enum net_socket_config_t
{
	NET_SET_RECV_EVENT,
	NET_SET_SEND_EVENT,
	NET_SET_ERROR_EVENT,
};

network_t *    network_create (); 
void           network_free   (network_t * network);
net_socket_t * network_connect(network_t * network, const char * host,
					int port);
int            network_procmsg(network_t * network);

int    net_socket_size (net_socket_t * socket);
char * net_socket_data (net_socket_t * socket);
char * net_socket_pop  (net_socket_t * socket, int bytes);
int    net_socket_write(net_socket_t * socket, const char * buf, int size);
int    net_socket_statu(net_socket_t * socket);
void   net_socket_close(net_socket_t * socket);
void   net_socket_config(net_socket_t * network, net_socket_config_t cmd, 
			void * parm);

void   net_socket_set_user_data(net_socket_t * socket, void * d);
void * net_socket_get_user_data(net_socket_t * socket);

#endif
