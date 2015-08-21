#ifndef STRING_EXTERN_H
#define STRING_EXTERN_H

/* É¾³ý×Ö·û´®Î²²¿µÄÄ³Ð©×Ö·û */
char * rtrimchr( char *str, char cflag );
char * ltrimchr( char *str, char cflag );
const char * getsubstr( char * sdest, unsigned int nsize, const char * ssrc, char cflag);

#endif