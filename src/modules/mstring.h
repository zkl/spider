/*
	×Ö·û´®´¦Àí v1.0.0
*/

#ifndef MSTRING_H
#define MSTRING_H

#include <string.h>

#define DBG mstring_debug

typedef struct _mstring_
{
	int size;
	int maxsize;
	char * str;
}MSTRING;


MSTRING mstring_new(char * str, int size);
void mstring_del(MSTRING mstr);

void debug(const char *msg, ...);
void strtoupper(char *ostr, const char *istr);
void strtolower(char *ostr, const char *istr);

char * strcpy_chr(char *ostr, int size, const char *istr, char c);
char * strncpy_l2(char *dest, const char *str, int len);
char * malloc_str(const char * buf, int size);
char * strgetfiled(char * dest , int size, char const ** endstr, const char * sou, const char *splitchars);
char * strreplace(char * dest, int size, const char * sou, const char *sstr, const char *dstr);
char * strheadremovechr(char *dest, const char *sou, char c);
char * strlastremovechr(char *dest, const char *sou, char c);
char * strdelentry(char *dest, const char *src, char *chrs);

char * utf8tomult(char * mulstr, int size, const char * utf8str);

int utf8toucs2(wchar_t *unicode, const char **endstr, const char *utf8);

#endif