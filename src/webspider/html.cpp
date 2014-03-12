#include <stdio.h>
#include <malloc.h>
#include "modules/mstring.h"
#include "modules/mklge.h"
#include "html.h"

int  g_used = 0;
char g_html[1024*1024*10]; // ×î´ó10M

void html_anlize(char * html, LIST * url_list, LIST * img_list)
{
	FILE * file;
	HTML_DATA data;

	if(g_used > 0)
		return ;

	data.url_list = url_list;
	data.img_list = img_list;

	g_used++;

	file = fopen(html, "rb");

	if(file != NULL)
	{
		fread(g_html, sizeof(char), 1024*1024*10, file);


		anlize(g_html, strlen(g_html), html_deal, &data);
		
		fclose(file);
	}

	g_used--;

}
int  html_deal(int sig, const char * buf, int size, void * parm)
{
	static int img = 0;
	static int lnk = 0;


	if(sig == MARK_KEY || sig == MARK_ATT)
	{
		char * data = malloc_str(buf, size);
		html_format(data, data);
		strtolower(data, data);

		if(sig == MARK_KEY)
		{		
			if(!strcmp(data, "img"))
				img = 1;
			else
				img = 0;
	
			if(!strcmp(data, "a") || !strcmp(data, "link"))
				lnk = 1;
			else
				lnk = 0;

			//if(!strcmp(data, "/html"))
			//	DBG("END");
		}
		else
		{
			const char * val = buf + strlen(buf) + 1;
			if(img && !strcmp(data, "src"))
			{
				//DBG("IMG : %s", val);
			}

			if(lnk && !strcmp(data, "href"))
			{
				//DBG("LNK : %s", val);
			}
		}
		free(data);
	}
	return 0;
}

char * html_format(char * dest, const char *str)
{
	char * bp = dest;
	strcpy(dest, str);

	while(*str && (*str == '\n' || *str == ' '|| *str== '\t' || *str == '\r'))
		str++;

	while(*str)
	{
		if(*str != '\n' && *str != '\r')
			*dest++ = *str;

		str++;
	}
	dest--;
	while(dest != bp && (*dest == '\r'||*dest == ' ' ||*dest == '\t'||*dest == '\n'))
		dest--;
	*(dest+1) = 0;

	return bp;
}