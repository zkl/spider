#ifndef _KDSTRAPI_H_
#define _KDSTRAPI_H_

//#include "codecvt.h"

#define PWD_MAX_LEN 10

int cutsubstr( char * sdest, int nsize, char *ssrc, char cflag );
int _cutsubstr( char * sdest, int nsize, char *&ssrc, char cflag );
int getsubstr( char * sdest, int nsize, char *ssrc, int ncnt, char cflag );
int rfillex( char * sdest, int nsize, char * ssrc, int ncnt, char cflag );
int lfillex( char * sdest, int nsize, char * ssrc, int ncnt, char cflag );
int rfill( char * sdest, int nsize, char cflag );
int lfill( char * sdest, int nsize, char cflag );
void alltrim( char *str );
void rtrim( char *str );
void ltrim( char *str );

int lstrncat( char * sdest, int nsize, char * ssrc, int ncnt );
int lstrcat( char * sdest, int nsize, char * ssrc );
int rstrncat( char * sdest, int nsize, char * ssrc, int ncnt );
int rstrcat( char * sdest, int nsize, char * ssrc );

int strxor(char *sdest, int nsize, char *ssrc1, char *ssrc2, char cflag);
int stror(char *sdest, int nsize, char *ssrc1, char *ssrc2, char cflag);
int strand(char *sdest, int nsize, char *ssrc1, char *ssrc2, char cflag);
int strnotin(char *sdest, int nsize, char *ssrc1, char *ssrc2, char cflag);
int strchrand(char *sdest, int nsize, char *ssrc1, char *ssrc2);

int strrpl(char * sdest, int nsize, char * ssrc, const char *ssub, const char *srpl);
void getsystime( long &ndate, long &ntime );
int getfileinfo( const char *psfilename, int &nfilesize, char *scdate, int ncdtsize, char *sctime, int nctmsize,
					char *smdate, int nmdtsize, char *smtime, int nmtmsize );
int substr( char * sdest, unsigned int nsize, char *ssrc, unsigned int noffset, unsigned int ncnt );

double round(double dbIn, int nRatio, int nPrec = 15);

char * strencrypt0(const char* szsrc, const char* szkey, char * szresult);
char * strdecrypt0(const char* szsrc, const char* szkey, char * szresult);

bool isdigital( const char * szSrc );
bool isdigitalex( const char * szSrc );
bool isengchror( const char * szSrc );
bool isengchrordigital( const char * szSrc );

char chrisnull( char chrsrc );

void ltrimchr( char *str, char cflag );
void rtrimchr( char *str, char cflag );
void alltrimchr( char *str, char cflag );

int datediff( unsigned int date1, unsigned int date2, int &ndaydiff );
int datediff( unsigned int date1, unsigned int date2, char cDatepart, int &nNum );
int dateplus( unsigned int date1, int nday, unsigned int &date2 );
int dateplus( unsigned int date1, char cDatepart, int nNum, unsigned int &date2 );

void strdistinct(char * psrc);
void strdelchr(char * str, char cflag );
void strdelstrchr(char * operstr, char * parstr);

int t2b(char *pstr, void *pbuf, int nsize);
int b2t(void *pvd, int nlen, char *pszbuf, int nsize);

int EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen);
int DecodeBase64(const char* pSrc, unsigned char* pDst, int nSrcLen);
int strConvert(char * psrc);
void fillcomma(char * sStrin);

int charreplchar(char *sSrc, char cTarget, char cRepl);
int Idno15or18(char *sTostr, const char *sSstr, int nLen);
int cutsubbetweenflagstr(char *sDest, int nSize, char *sSrc, char cStartFlag, char cEndFlag);
int strright(char *sdest, int nsize, char *ssrc, int ncnt, char cflag);
int getchrnum(char *ssrc, char cflag);
char *charindextoin(char *ssrc, char *sdest, char ssprt);
int dayOfWeek(long ndate)  ;
int datesubtract(  int date1, int nday,  int &date2 );

#endif

