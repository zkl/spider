#ifndef HTML_H
#define HTML_H

#include "modules/list.h"

typedef struct _html_data_
{
	LIST * url_list;
	LIST * img_list;
}HTML_DATA;

void html_anlize(char * html, LIST * url_list, LIST * img_list);
int  html_deal(int id, const char * buf, int size, void * parm);
char * html_format(char * dest, const char *str);

#endif