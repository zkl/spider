#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include "mstring.h"
#include "mklge.h"

int  isnote(const char  * mark);
void anlizetextbtw(const char * buf, callback deal, void * parm);
char * anlizemark(char *mark, int maxsize, const char ** pend, const char * buf);
char * getkey(char * dest , int size, char const ** endstr, const char * sou);
char * splitkey(char * key, int maxkey, char * value, int maxvalue, const char ** pend, const char * buf);
const char * getmark(const char * mark, const char * buf);
const char * strchr_s(const char *p, char c);
const char * strstr_s(const char * buf, const char * str);
const char * anlizetext(const char * mark, const char * buf, callback deal, void * pram); 


/**************************************************************/
/*                                                            */
/* 版    本:            v1.2                                  */
/* 入    参:  解析函数                                        */
/* 出    参:                                                  */
/* 备    注:                                                  */
/**************************************************************/
void anlize(const char *buf, int bufsize, callback deal, void *parm)
{
	int sign = 0;
	const char *bptr = buf;

	while(buf && *buf && buf-bptr <= bufsize)
	{
		char mark[2048] = {0};
		char name[2048];
		char attib[2048];
		char value[2048];

		const char * pstart = mark;
		const char * pend   = mark;
		const char * markend= 0;

		anlizetextbtw(buf, deal, parm);
		anlizemark(mark, 2048, &buf, buf);
		
		if(isnote(mark) > 0)
			continue;
		
		getkey(name, 2048, &pend, pstart);
		if(strlen(name) == 0)
			break;

		deal(MARK_KEY, name, strlen(name), parm);

		while(pend-mark < strlen(mark))
		{
			char tmpatt[2048];
			pstart = pend+1;
			getkey(tmpatt, 2048, &pend, pstart);

			if(splitkey(attib, 2048, value, 2048, 0, tmpatt))
			{
				strcpy(&attib[strlen(attib)+1], value);
				deal(MARK_ATT, attib, strlen(attib)+strlen(value)+1, parm);
			}
			else
				break;
			
			if(pend == 0)
				break;
		}

		markend = anlizetext(name, buf, deal, parm);
		strtolower(name, name);	
		if(!strcmp(name, "script"))
			buf = markend;
	}
}

char * anlizemark(char * mark, int maxsize, const char ** pend, const char *buf)
{
	const char * pstart  = strchr(buf, '<');
	const char * pendstr = strchr_s(pstart, '>');

	if(pstart && pendstr && pendstr-pstart < maxsize) 
	{
		strncpy(mark, pstart+1, pendstr-pstart-1);
		mark[pendstr-pstart-1] = 0;
		*pend = pendstr+1;
		return mark;
	}
	else
	{
		return 0;
	}
}

const char * anlizetext(const char * mark, const char * buf, callback deal, void * parm)
{
	char  * marklow = (char *)malloc(strlen(mark)+1);
	strtolower(marklow, mark);

	if(marklow[0] != '/')
	{
		int count = 1;
		char bname[1024];
		char nameend[1024];
		char tmpmark[1024];
		const char * bufbp = buf;
		const char * next = buf;

		strcpy(nameend, "/");
		strcat(nameend,  marklow);

		if(!strcmp(marklow, "script")) //   跳过关于script内部的内容
		{
			char * tw = 0;
			strcpy(nameend, "</");
			strcat(nameend, marklow);
			
			buf = getmark(nameend, buf);
			if(buf == 0)
			{
				free(marklow);
				return bufbp;
			}
			tw = malloc_str(bufbp, buf-bufbp);
			if(tw)
			{
				deal(MARK_BTW, tw, buf-bufbp, parm);
				free(tw);
			}
			count = 0;
		}

		while(count)
		{
			buf = next;
			if(!anlizemark(tmpmark, 1024, &next, buf))
				break ;

			if(isnote(tmpmark))
				continue;

			if(!getkey(bname, 1024, 0, tmpmark))
				break;

			strtolower(bname, bname);
			if(!strcmp(bname, marklow))
				count++;
			if(!strcmp(bname, nameend))
				count--;
			buf = strchr_s(buf, '<');
		}

		while(count == 0)
		{
			char * tmpbuf = malloc_str(bufbp, buf-bufbp);
			if(!tmpbuf)
				break;
				
			deal(MARK_TXT, tmpbuf, buf-bufbp+1, parm);
			free(tmpbuf);
			break;
		}
	}
	free(marklow);
	return buf;
}

void anlizetextbtw(const char * buf, callback deal, void * parm)
{
	char * texttw;
	const char * pstart = strchr_s(buf, '<');
	
	if(pstart == 0 || pstart-buf == 0)
		return ;
	
	texttw = malloc_str(buf, pstart-buf);
	deal(MARK_BTW, texttw, pstart-buf, parm);
	free(texttw);
}
const char * getmark(const char * mark, const char * buf)
{
	return strstr_s(buf, mark);
}

char * getkey(char * dest , int size, char const ** endstr, const char * sou)
{
	int signal_d = 0;
	int signal_s = 0;
	int length= 0;
	char *p = dest;

	*p = 0;
	if(!sou || *sou == 0)
		return 0;

	while(*sou && ( *sou == ' ' || *sou == '\n' || *sou == '\r'))
		sou++;
	
	while(*sou && ( *sou != ' ' && *sou != '\n' || signal_s || signal_d) && length <= size)
	{
		if(*sou == '"' && signal_s == 0)
			signal_d = signal_d ? 0:1;
		
		if(*sou == '\'' && signal_d == 0)
			signal_s = signal_s ? 0:1;

		*p++ = *sou++;
		length++;
	}
	if(endstr)
		*endstr = sou;
	*p = 0;
	return dest;
}

char * splitkey(char * key, int maxkey, char * val, int maxvalue, const char ** pend, const char * buf)
{
	const char * pv;
	char *backupstr;

	if(!buf && *buf)
		return 0;

	backupstr = malloc_str(buf, -1);
	*key = 0;
	*val = 0;
	
	pv = strchr_s(backupstr, '=');
	if(pv != NULL)
	{
		backupstr[pv-backupstr] = ' '; //将key和alue使用空格分割
		getkey(key, maxkey, 0, backupstr);
		getkey(val, maxvalue, pend, pv);
	}
	else
	{
		getkey(key, maxkey, pend, backupstr);
	}

	free(backupstr);
	
	if(*key == 0)
		return 0;

	return key;
}

const char * strchr_s(const char *p, char c) //跳过引号内的内容
{
	int signal_s = 0;
	int signal_d = 0;
	while(p && *p)
	{
		if(tolower(*p) == tolower(c) && signal_s == 0 && signal_d == 0)
			return p;

		if(*p == '\'' && signal_d == 0)
			signal_s = signal_s ? 0:1;

		if(*p == '\"' && signal_s == 0)
			signal_d = signal_d ? 0:1;

		p++;
	}
	return 0;
}
const char * strstr_s(const char * buf, const char * str)
{
	int  strlength  = strlen(str);
	char * strlow_s = (char *)malloc(strlength+1);
	char * strcmp_s = (char *)malloc(strlength+1);
	const char * first = 0;

	strtolower(strlow_s, str);
	while(*buf)
	{
		first = strchr_s(buf, *strlow_s);
		if(!first)
			break;
		strncpy(strcmp_s, first, strlength);
		*(strcmp_s+strlength) = 0;
		strtolower(strcmp_s, strcmp_s);

		if(!strcmp(strlow_s, strcmp_s))
			break;

		buf = first+1;
	}
	free(strlow_s);
	free(strcmp_s);
	return first;
}

int isnote(const char * mark)
{
	const char * start = mark;
	const char * end   = mark + strlen(mark)-1;

	if(strlen(mark) < 5)
		return 0;

	while(*start == ' ')
		start++;
	while(*end == ' ')
		end--;
	if(*start == '!' && *(start+1) == '-' && *(start+2) == '-')
	{
		if(*end == '-' && *(end-1) == '-')
			return 1;
		else
			return -1;
	}
	return 0;
}