#ifndef STR_API_H
#define STR_API_H

/* ɾ���ַ���β����ĳЩ�ַ� */
char * rtrimchr( char *str, char cflag );
char * ltrimchr( char *str, char cflag );
const char * getsubstr( char * sdest, int nsize, const char * ssrc, char cflag);

#endif