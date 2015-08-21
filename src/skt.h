#ifndef SKT_H
#define SKT_H

void skt_init();
void skt_close();
int  skt_send(char *buf, int size, void *parm);
int  skt_recv(char *buf, int size, void *parm);

#endif