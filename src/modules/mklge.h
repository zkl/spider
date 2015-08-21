/**************************************************************/
/*                                                            */
/* ��    ��:            v1.1.1                                */
/* ��������:      Mon  8 Otc 2012                             */
/* �޸�����:      Mon  8 Sep 2012                             */
/* ��    Ȩ:                                                  */
/* ��    ע:  �ն˳���ͨ�ÿ��                                */
/**************************************************************/


#ifndef MKLGE_H
#define MKLGE_H

#include <string.h>

// id defines.
#define MARK_KEY  1
#define MARK_ATT  2
#define MARK_TXT  3
#define MARK_BTW  4

typedef int (*callback)(int id, const char * buf, int size, void * parm);

void anlize(const char *buf, int bufsize, callback deal, void * parm);

#endif
