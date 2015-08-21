#ifndef STRING_EXTERN_H
#define STRING_EXTERN_H

/* 删除字符串尾部的某些字符 */
char * rtrimchr( char *str, char cflag );
char * ltrimchr( char *str, char cflag );
const char * getsubstr( char * sdest, unsigned int nsize, const char * ssrc, char cflag);

#endif