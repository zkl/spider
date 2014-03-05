/***************************************************************
* ��Ȩ���� : 2003-2010, ���ڽ�֤�Ƽ��ɷݹ�˾.
* �ļ���   : kdstrapi.cpp
* ����     : rdc-zhangz  
* �汾     : 1.0.0.0 
* �������� : 20030916
* ����     : �ַ���������������
* ����     : 
* �����б� : 
     
  ltrimchr                  ɾ���ַ���ͷ����ĳ�ַ�
  rtrimchr                  ɾ���ַ���β����ĳ�ַ�
  alltrimchr                ɾ���ַ���ͷ����ĳ�ַ�
  ltrim                     ɾ���ַ���ͷ���Ŀո�
  rtrim                     ɾ���ַ���β���Ŀո�
  alltrim                   ɾ���ַ���ͷ����β���Ŀո�
  lfill                     ���ַ���ͷ������ָ���ַ�
  rfill                     ���ַ���β������ָ���ַ�
  lfillex                   ���ַ���ͷ������ָ���ַ�
  rfillex                   ���ַ���β������ָ���ַ�
  getsubstr                 ���ݷָ�������Ŵ�ָ���ַ�����ȡ��Ӧ�Ӵ�
  cutsubstr                 ���ݷָ�����ָ���ַ����������ȡ�Ӵ�
  lstrncat                  ���ַ���ͷ��׷��ָ���ַ�����ǰn���ַ�
  lstrcat                   ���ַ���ͷ��׷��ָ���ַ���
  rstrncat                  ���ַ���β��׷��ָ���ַ�����ǰn���ַ�,��strncat��ȫ������Խ��
  rstrcat                   ���ַ���β��׷��ָ���ַ�������strcat��ȫ������Խ��
  strand                    �ַ������������ȡ2���ַ������������Ӵ�
  stror                     �ַ�����������ϲ�2���ַ�����ʹĿ�괮����2���ַ�����ȫ���Ӵ�
  strxor                    �ַ�����������ȡ��2���ַ����в�ͬ���Ӵ�
  strnotin                  ȡ���ַ���2�����ַ���1�е��Ӵ�
  strchrand                 �ַ������������ȡ2���ַ������������Ӵ�

  getfileinfo               �õ��ļ���Ϣ
  getsystime                ȡ��ϵͳ����ʱ��
  strrpl                    �ַ����滻

  strencrypt0               �ַ������ܷ���0
  strdecrypt0               �ַ������ܷ���0

  chrisnull                 �ж��ַ��Ƿ�ΪNULL
  isengchrordigital         �ж��ַ��Ƿ�Ϊ���ֻ�Ӣ���ַ�
  isengchr                  �ж��ַ��Ƿ�ΪӢ���ַ�
  isdigital                 �ж��ַ��Ƿ�Ϊ����
  isdigitalex               �ж��ַ������Ƿ�ȫ�����֣�����(-+)��С����(.)

  datediff                  ���������ڵĲ�ֵ��������
  dateplus                  ��һ�����ڼ���һ�������ĺ�ֵ�����ڣ�
  datesubtract              ��һ�����ڼ�ȥһ�������Ĳ�ֵ�����ڣ�

  strdistinct               ɾ���ַ������ظ����ַ�
  strdelchr                 ɾ���ַ�����ĳ���ַ�
  strdelstrchr              ���ַ���1�� ɾ�� �ַ���2�������ַ�
  t2b                       ���ı�ת���ɶ�����
  b2t                       �򵥶�����ת�����ı�

  strright						ȡ�ַ����ұ�Nλ�ַ�
  cutsubbetweenflagstr		���ݷָ�����ָ���ַ����н�ȡ�����ָ����е��Ӵ�(Դ�����ı�)
  Idno15or18				ʮ��λ���֤��ʮ��λ���֤֮���ת��
* �޸���ʷ :    

***************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/stat.h>
#include <math.h>
#include "strapi.h"



/***************************************************************
* �������� : round
* ����     : rdc-zhangz 
* ����     : ��������
* ����     : double dbIn  Ҫ��������ĸ�����
             int nRatio   ����һλ��ʼ��������
			 int nPrec    Ҫ��ľ���   
* ����ֵ   : ��
* �޸���ʷ : 
***************************************************************/
double round(double dbIn, int nRatio, int nPrec )
{
	double db, dbErr;
	char szTmp[64];
	__int64 ll;
	unsigned int  nLen;
    unsigned int nTime;

    if( nRatio > 10 )
        nRatio = 10;

    if( nRatio < 0 )
		nRatio = 0;

	szTmp[0] = 0;
	ll = (__int64)dbIn;
	
    if( ll == 0 )
    {
        nLen = 16;
    }
    else
    {
	    sprintf( szTmp, "%I64d", ll );    
	    nLen = 16 - strlen(szTmp);
    }
    
	nPrec += 1;
	nLen = nLen<(unsigned int)nPrec?nLen:nPrec;
	
	dbErr = 5.00;
	 unsigned int i = 0;
	for(i = 0; i < nLen; i++ )
	{
		dbErr = dbErr/10.00;
	}
	
    for( i = 0, nTime = 1; i < (unsigned int)nRatio; i++ )
    {
        nTime = 10*nTime;
    }

    db=floor((dbIn+dbErr)*nTime+0.5);
    return (db=db/nTime);    
}

/***************************************************************
* �������� : ltrimchr
* ����     : rdc-zhangz  
* ����     : ɾ���ַ���ͷ����ĳ�ַ��Ĵ�
* ����     : char *str  Ҫ�����Ĵ�
             char cflag Ҫɾ�����ַ�
* ����ֵ   : ��
* �޸���ʷ : 
***************************************************************/
void ltrimchr( char *str, char cflag )
{
	int i ;
	int len;
	
	len = (int)strlen( str );

	for( i = 0 ; i < len && str[i] == cflag; i++ );
	if( i > 0 )
		strcpy( str , str + i );
	return;
}

/***************************************************************
* �������� : rtrimchr
* ����     : rdc-zhangz  
* ����     : ɾ���ַ���β����ĳ�ַ��Ĵ�
* ����     : char *str  Ҫ�����Ĵ�
             char cflag Ҫɾ�����ַ�
* ����ֵ   : ��
* �޸���ʷ : 
***************************************************************/
void rtrimchr( char *str, char cflag )
{
	int i;
	int len;

	len = (int)strlen( str ) - 1;

	for( i = len; i >= 0 && str[i] == cflag; i-- )
		str[i] = 0;
	return;
}

/***************************************************************
* �������� : allltrimchr
* ����     : rdc-zhangz  
* ����     : ɾ���ַ���ͷβ����ĳ�ַ��Ĵ�
* ����     : char *str  Ҫ�����Ĵ�
             char cflag Ҫɾ�����ַ�
* ����ֵ   : ��
* �޸���ʷ : 
***************************************************************/
void alltrimchr( char *str, char cflag )
{
	ltrimchr( str, cflag );
	rtrimchr( str, cflag ) ;
	return ;
}

/***************************************************************
* �������� : ltrim
* ����     : rdc-zhangz  
* ����     : ɾ���ַ���ͷ���Ŀո�
* ����     : char *str
* ����ֵ   : ��
* �޸���ʷ : 
***************************************************************/
void ltrim( char *str )
{
	ltrimchr( str, 0x20 );
}

/***************************************************************
* �������� : rtrim
* ����     : rdc-zhangz  
* ����     : ɾ���ַ���β���Ŀո�
* ����     : char *str
* ����ֵ   : ��
* �޸���ʷ : 
***************************************************************/
void rtrim( char *str )
{
	rtrimchr( str, 0x20 );
}

/***************************************************************
* �������� : alltrim
* ����     : rdc-zhangz  
* ����     : ɾ���ַ���ͷ����β���Ŀո�
* ����     : char *str
* ����ֵ   : ��
* �޸���ʷ : 
***************************************************************/
void alltrim( char *str )
{
    ltrimchr( str, 0x20 );
    rtrimchr( str, 0x20 );
    return;
}

/***************************************************************
* �������� : lfill
* ����     : rdc-zhangz  
* ����     : ���ַ���ͷ������ָ���ַ�
* ����     : char *sdest  - �������ַ���
*            int nsize    - �ַ����Ŀռ��С(���󳤶�=nsize-1)
*            char cflag   - �����ַ�(����Ϊ0x00)
* ����ֵ   : 0   - �ɹ�
*            200 - ��������
* �޸���ʷ : 
***************************************************************/
int lfill( char * sdest, int nsize, char cflag )
{
    return lfillex( sdest, nsize, sdest, nsize-1, cflag );
}

/***************************************************************
* �������� : rfill
* ����     : rdc-zhangz  
* ����     : ���ַ���β������ָ���ַ�
* ����     : char *sdest  - �������ַ���
*            int nsize    - �ַ����Ŀռ��С(���󳤶�=nsize-1)
*            char cflag   - �����ַ�(����Ϊ0x00)
* ����ֵ   : 0   - �ɹ�
*            200 - ��������
* �޸���ʷ : 
***************************************************************/
int rfill( char * sdest, int nsize, char cflag )
{
    return rfillex( sdest, nsize, sdest, nsize-1, cflag );
}

/***************************************************************
* �������� : lfillex
* ����     : rdc-zhangz  
* ����     : ���ַ���ͷ������ָ���ַ�
* ����     : char *sdest  - ����Ĵ�
*            int nsize    - �ַ����Ŀռ��С
*            char *ssrc   - Դ��
*            int ncnt     - ������ַ������ܳ���(ҪС��nsize)
*            char cflag   - �����ַ�(����Ϊ0x00)
* ����ֵ   : 0   - �ɹ�
*            100 - Ŀ�괮�ռ䲻���Դ�����ȴ���ncnt
*            200 - ��������
* �޸���ʷ : 
***************************************************************/
int lfillex( char * sdest, int nsize, char * ssrc, int ncnt, char cflag )
{
    int nsrclen;
    
    if( ncnt <= 0 || nsize <= 0 || cflag == 0 )
        return 200;
    
    nsrclen = strlen(ssrc);
    
    if( nsize <= ncnt || ncnt < nsrclen || nsize <= nsrclen )
        return 100;
    
    if( sdest != ssrc )
    {
        memset( sdest, cflag, ncnt-nsrclen );
        strcpy( sdest+ncnt-nsrclen, ssrc );
    }
    else //same address
    {
        strrev(ssrc);
        memset(ssrc+nsrclen, cflag, ncnt-nsrclen );
        ssrc[ncnt] = 0x00;
        strrev(ssrc);
    }
    return 0;
}

/***************************************************************
* �������� : rfillex
* ����     : rdc-zhangz  
* ����     : ���ַ���β������ָ���ַ�
* ����     : char *sdest  - ����Ĵ�
*            int nsize    - �ַ����Ŀռ��С
*            char *ssrc   - Դ��
*            int ncnt     - ������ַ������ܳ���(ҪС��nsize)
*            char cflag   - �����ַ�(����Ϊ0x00)
* ����ֵ   : 0   - �ɹ�
*            100 - Ŀ�괮�ռ䲻���Դ�����ȴ���ncnt
*            200 - ��������
* �޸���ʷ : 
***************************************************************/
int rfillex( char * sdest, int nsize, char * ssrc, int ncnt, char cflag )
{
    int nsrclen;
    
    if( nsize <= 0 || ncnt <= 0 ||  cflag == 0 )
        return 200;
    
    nsrclen = strlen(ssrc);
    if( nsize <= ncnt || ncnt < nsrclen || nsize <= nsrclen )
        return 100;
    
    strncpy( sdest, ssrc, nsize-1 );
    memset( sdest+nsrclen, cflag, ncnt-nsrclen );
    sdest[ncnt] = 0x00;
    return 0;
}

/***************************************************************
* �������� : getsubstr
* ����     : rdc-zhangz  
* ����     : ���ݷָ�������Ŵ�ָ���ַ�����ȡ��Ӧ�Ӵ�(Դ������)
* ����     : char *sdest  - ȡ�����Ӵ�
*            int nsize    - �Ӵ��Ŀռ��С
*            char *ssrc   - Դ��
*            int ncnt     - �Ӵ����(��0��ʼ)
*            char cflag   - �ָ���(����Ϊ0x00)
* ����ֵ   : 0   - �ɹ�
*            1   - �Ӵ������ڻ�Դ��Ϊ��
*            100 - �Ӵ��ռ䲻��
*            200 - ��������
* �޸���ʷ : 
***************************************************************/
int getsubstr( char * sdest, int nsize, char *ssrc, int ncnt, char cflag )
{
    int i;
    char *ps;
    char *ps1;
    char *ps2;
    
    if( nsize < 1 || ncnt < 0 || cflag == 0)
        return 200;
    
    if( *ssrc == 0 )
        return 1;
    
    ps = ssrc;
    ps1 = ps;
    ps2 = ps;
    for( i = 0; (ps2=strchr(ps,cflag))!=NULL && i<ncnt; i++ )
    {
        ps1 = ps2+1;
        ps = ps2+1;
    }
    
    if( i == ncnt )
    {
        if( ps2 == NULL )
        {
            if( (int)strlen(ssrc)-(ps1-ssrc) < nsize )
            {
                strcpy( sdest, ps1 );
            }
            else
                return 100;
        }
        else
        {
            if( ps2-ps1 < nsize )
            {
                memcpy( sdest, ps1, ps2-ps1 );
                sdest[ps2-ps1] = 0x00;
            }
            else
                return 100;
        }
    }
    else
    {
        return 1;
    }
    return 0;
}

/***************************************************************
* �������� : _cutsubstr
* ����     : rdc-zhangz  
* ����     : ���ݷָ�����ָ���ַ����������ȡ�Ӵ�(Դ������ı䣬ֻ��Դ��ָ����ƶ�)
* ����     : char *sdest  - ��ȡ�����Ӵ�
*            int nsize    - �Ӵ��Ŀռ��С
*            char *ssrc   - Դ��
*            char cflag   - �ָ���(����Ϊ0x00)
* ����ֵ   : 0   - �ɹ�
*            1   - Դ��Ϊ��(sdest���ÿ�)
*            100 - �Ӵ��ռ䲻��
*            200 - ��������
* �޸���ʷ : 
***************************************************************/
int _cutsubstr( char * sdest, int nsize, char *&ssrc, char cflag )
{
    char * ps;
    
    if( nsize < 1 || cflag == 0 )
        return 200;
    
    if( strlen(ssrc) == 0 )
    {
        *sdest = 0x00;
        return 1;
    }
    
    ps = strchr( ssrc, cflag );
    if( ps == NULL )
    {
        if( (int)strlen(ssrc) < nsize )
        {
            strcpy( sdest, ssrc );
            ssrc += strlen(ssrc);
        }
        else
            return 100;		
    }
    else
    {
        if( ps-ssrc < nsize )
        {
            memcpy( sdest, ssrc, ps-ssrc );
            sdest[ps-ssrc] = 0x00;
            ssrc = ps+1;
        }
        else
            return 100;
    }	
    return 0;
}

/***************************************************************
* �������� : cutsubstr
* ����     : rdc-zhangz  
* ����     : ���ݷָ�����ָ���ַ����������ȡ�Ӵ�(Դ�����ı�)
* ����     : char *sdest  - ��ȡ�����Ӵ�
*            int nsize    - �Ӵ��Ŀռ��С
*            char *ssrc   - Դ��
*            char cflag   - �ָ���(����Ϊ0x00)
* ����ֵ   : 0   - �ɹ�
*            1   - Դ��Ϊ��(sdest���ÿ�)
*            100 - �Ӵ��ռ䲻��
*            200 - ��������
* �޸���ʷ : 
***************************************************************/
int cutsubstr( char * sdest, int nsize, char *ssrc, char cflag )
{
    char * ps;
    
    if( nsize < 1 || cflag == 0 )
        return 200;
    
    if( strlen(ssrc) == 0 )
    {
        *sdest = 0x00;
        return 1;
    }
    
    ps = strchr( ssrc, cflag );
    if( ps == NULL )
    {
        if( (int)strlen(ssrc) < nsize )
        {
            strcpy( sdest, ssrc );
            *ssrc = 0x00;
        }
        else
            return 100;		
    }
    else
    {
        if( ps-ssrc < nsize )
        {
            memcpy( sdest, ssrc, ps-ssrc );
            sdest[ps-ssrc] = 0x00;
            strcpy( ssrc, ps+1 );
        }
        else
            return 100;
    }	
    return 0;
}

/***************************************************************
* �������� : lstrncat
* ����     : chensz
* ����     : ���ַ���ͷ��׷��ָ���ַ�����ǰn���ַ�
* ����     : char *sdest  - Ŀ�괮
*            int nsize    - Ŀ�괮�Ŀռ��С
*            char *ssrc   - Դ��
*            int ncnt     - ȡԴ�����ַ�����
* ����ֵ   :  0 - �ɹ�
*            -1 - Ŀ�괮�ռ䲻��
*            -2 - ��������
* �޸���ʷ : 
***************************************************************/
int lstrncat( char * sdest, int nsize, char * ssrc, int ncnt )
{
    int nsrclen;
    int ndestlen;
    int i;
    
    if( ncnt < 0 || nsize <= 0 ) 
        return -2;
    
    nsrclen = strlen(ssrc);
    ndestlen = strlen(sdest);
    
    if( ncnt == 0 || nsrclen == 0 )
        return 0;
    
    if( nsrclen > ncnt ) //ֻȡǰncnt���ַ�
        nsrclen = ncnt;
    
    if( nsize <= nsrclen + ndestlen ) //no enough space
        return -1;
    
    for (i=ndestlen-1; i>=0; i--) //move
        *(sdest+i+nsrclen) = *(sdest+i);
    for (i=0; i<nsrclen; i++) 
        *(sdest+i) = *(ssrc+i);
    *(sdest+nsrclen+ndestlen) = 0x00;
    
    return 0;
}

/***************************************************************
* �������� : lstrcat
* ����     : chensz
* ����     : ���ַ���ͷ��׷��ָ���ַ���
* ����     : char *sdest  - Ŀ�괮
*            int nsize    - Ŀ�괮�Ŀռ��С
*            char *ssrc   - Դ��
* ����ֵ   :  0 - �ɹ�
*            -1 - Ŀ�괮�ռ䲻��
*            -2 - ��������
* �޸���ʷ : 
***************************************************************/
int lstrcat( char * sdest, int nsize, char * ssrc )
{
    return lstrncat(sdest, nsize, ssrc, strlen(ssrc));
}

/***************************************************************
* �������� : rstrncat
* ����     : chensz
* ����     : ���ַ���β��׷��ָ���ַ�����ǰn���ַ�
*            ��strncat��ȫ��������Խ��
* ����     : char *sdest  - Ŀ�괮
*            int nsize    - Ŀ�괮�Ŀռ��С
*            char *ssrc   - Դ��
*            int ncnt     - ȡԴ�����ַ�����
* ����ֵ   :  0 - �ɹ�
*            -1 - Ŀ�괮�ռ䲻��
*            -2 - ��������
* �޸���ʷ : 
***************************************************************/
int rstrncat( char * sdest, int nsize, char * ssrc, int ncnt )
{
    int nsrclen;
    int ndestlen;
    int i;
    
    if( ncnt < 0 || nsize <= 0 )
        return -2;
    
    nsrclen = strlen(ssrc);
    ndestlen = strlen(sdest);
    
    if( ncnt == 0 || nsrclen == 0 )
        return 0;
    
    if( nsrclen > ncnt ) //ֻȡǰncnt���ַ�
        nsrclen = ncnt;
    
    if( nsize <= nsrclen + ndestlen )
        return -1;
    
    for (i=ndestlen; i<nsrclen+ndestlen; i++)
        *(sdest+i) = *ssrc++;
    *(sdest+nsrclen+ndestlen) = 0x00;
    return 0;
}

/***************************************************************
* �������� : rstrcat
* ����     : chensz
* ����     : ���ַ���β��׷��ָ���ַ���
*            ��strcat��ȫ��������Խ��
* ����     : char *sdest  - Ŀ�괮
*            int nsize    - Ŀ�괮�Ŀռ��С
*            char *ssrc   - Դ��
* ����ֵ   :  0 - �ɹ�
*            -1 - Ŀ�괮�ռ䲻��
*            -2 - ��������
* �޸���ʷ : 
***************************************************************/
int rstrcat( char * sdest, int nsize, char * ssrc )
{
    return rstrncat(sdest, nsize, ssrc, strlen(ssrc));
}

/***************************************************************
* �������� : strchrand
* ����     : rdc-zhangz
* ����     : �ַ������������ȡ2���ַ������������Ӵ�
*            ssrc1 = "1234555"
*            ssrc2 = "257"
*            ���Ϊ "2555"
* ����     : char *sdest  - Ŀ�괮
*            int nsize    - Ŀ�괮�Ŀռ��С
*            char *ssrc1  - Դ��1
*            char *ssrc2  - Դ��2
* ����ֵ   : 0 - �ɹ�
*           -1 - Ŀ�괮�ռ䲻��
*           -2 - ��������
* �޸���ʷ : 
***************************************************************/
int strchrand(char *sdest, int nsize, char *ssrc1, char *ssrc2)
{
    int num;

    if( sdest == NULL || ssrc1 == NULL || ssrc2 == NULL || nsize <= 0 )
        return -2;

	num = 0;
	char * pchr = ssrc1, * pdest = sdest;
    for(; *pchr != 0x00; pchr++ )
    {
        if( strchr( ssrc2, *pchr ) != NULL )
        {
            *pdest = *pchr;
            pdest++;
            num++;
            if( num == nsize )
                return -1;
        }
    }
	*pdest = 0x00;
    return 0;
}

/***************************************************************
* �������� : strand
* ����     : liuyi
* ����     : �ַ������������ȡ2���ַ������������Ӵ�
*            ssrc1 = "001,00002,005,0004"
*            ssrc2 = "002,005,001"
*            ���Ϊ "001,005" 
* ����     : char *sdest  - Ŀ�괮
*            int nsize    - Ŀ�괮�Ŀռ��С
*            char *ssrc1  - Դ��1
*            char *ssrc2  - Դ��2
*            char cflag   - �Ӵ��ָ���
* ����ֵ   : 0 - �ɹ�
*           -1 - Ŀ�괮�ռ䲻��
*           -2 - ��������
* �޸���ʷ : 
***************************************************************/
int strand(char *sdest, int nsize, char *ssrc1, char *ssrc2, char cflag)
{
    int n=0;
    int sdestlen=0;
    char *pdest,*pl,*pr,*pp;
    int nret;
    
    if (nsize <= 0) //error parameter
        return -2;
    
    memset(sdest, 0, nsize);
    
    pdest = sdest;
    
    while ((nret=getsubstr(pdest, nsize-sdestlen, ssrc1, n++, cflag)) == 0)
    {
        pp = ssrc2;
        if ((pl=strstr(pp, pdest)) != NULL) //match a substr
        {
            do
            {
                pr = pl + strlen(pdest);
                if (*pr == cflag || *pr == 0x00) 
                {
                    if (pl == pp) //match a real substr
                        break; 
                    if (*(pl-1) == cflag) //match a real substr
                        break;
                }
                pp = pl+1;
            }while((pl=strstr(pp, pdest)) != NULL);
            
            if (pl == NULL) //not match a real substr
                continue;
        }
        else
            continue;
        
        if (sdestlen != 0)
        {
            if ((int)strlen(sdest) + 1 >= nsize-1)
                return -1;
            *(pdest+strlen(pdest)+1) = 0x00;
            memmove(pdest+1, pdest, strlen(pdest));
            *pdest = cflag;
        }
        sdestlen += strlen(pdest);
        pdest += strlen(pdest); 
    }
    
    if (nret == 100) //no enough space
        return -1;
    
    *pdest = 0x00;
    
    return 0;
}

/***************************************************************
* �������� : stror
* ����     : liuyi
* ����     : �ַ�����������ϲ�2���ַ�����
*            ʹĿ�괮����2���ַ�����ȫ���Ӵ�
*            ssrc1 = "001,00002,005,0004"
*            ssrc2 = "002,005,001"
*            ���Ϊ "001,002,00002,005,0004" 
* ����     : char *sdest  - Ŀ�괮
*            int nsize    - Ŀ�괮�Ŀռ��С
*            char *ssrc1  - Դ��1
*            char *ssrc2  - Դ��2
*            char cflag   - �Ӵ��ָ���
* ����ֵ   : 0 - �ɹ�
*           -1 - Ŀ�괮�ռ䲻��
*           -2 - ��������
* �޸���ʷ : 
***************************************************************/
int stror(char *sdest, int nsize, char *ssrc1, char *ssrc2, char cflag)
{
    int n=0;
    int sdestlen=0;
    char *pdest,*pl,*pr,*pp;
    int nret;
    
    if (nsize <= 0) //error parameter
        return -2;
    
    memset(sdest, 0, nsize);
    
    if ((nret = lstrcat(sdest, nsize, ssrc1)) != 0)
        return nret;
    
    sdestlen = strlen(sdest);
    pdest = sdest + sdestlen;
    
    while ((nret=getsubstr(pdest, nsize-sdestlen, ssrc2, n++, cflag)) == 0)
    {
        pp = sdest;
        while ((pl=strstr(pp, pdest)) != pdest)
        {
            pr = pl + strlen(pdest);
            if (*pr == cflag || *pr == 0x00) 
            {
                if (pl == pp) //match a real substr
                    break; 
                if (*(pl-1) == cflag) //match a real substr
                    break;
            }
            pp = pl+1;
        }
        
        if (pl != pdest) //find a real match
            continue;
        
        if (sdestlen != 0)
        {
            if ((int)strlen(sdest) + 1 >= nsize-1)
                return -1;
            *(pdest+strlen(pdest)+1) = 0x00;
            memmove(pdest+1, pdest, strlen(pdest));
            *pdest = cflag;
        }
        sdestlen += strlen(pdest);
        pdest += strlen(pdest); 
    }
    
    if (nret == 100) //no enough space
        return -1;
    
    *pdest = 0x00;
    
    return 0;
}

/***************************************************************
* �������� : strxor
* ����     : liuyi
* ����     : �ַ�����������ȡ��2���ַ����в�ͬ���Ӵ�
*            ssrc1 = "001,00002,005,0004"
*            ssrc2 = "002,005,001"
*            ���Ϊ "002,00002,0004" 
* ����     : char *sdest  - Ŀ�괮
*            int nsize    - Ŀ�괮�Ŀռ��С
*            char *ssrc1  - Դ��1
*            char *ssrc2  - Դ��2
*            char cflag   - �Ӵ��ָ���
* ����ֵ   : 0 - �ɹ�
*           -1 - Ŀ�괮�ռ䲻��
*           -2 - ��������
* �޸���ʷ : 
***************************************************************/
int strxor(char *sdest, int nsize, char *ssrc1, char *ssrc2, char cflag)
{
    int n=0;
    int sdestlen=0;
    char *pdest,*pl,*pr,*pp;
    int nret;
    
    if (nsize <= 0) //error parameter
        return -2;
    
    memset(sdest, 0, nsize);
    
    //base on ssrc1
    pdest = sdest;
    
    while ((nret=getsubstr(pdest, nsize-sdestlen, ssrc1, n++, cflag)) == 0)
    {
        pp = ssrc2;
        while ((pl=strstr(pp, pdest)) != NULL)
        {
            pr = pl + strlen(pdest);
            if (*pr == cflag || *pr == 0x00) 
            {
                if (pl == pp) //match a real substr
                    break; 
                if (*(pl-1) == cflag) //match a real substr
                    break;
            }
            pp = pl+1;
        }
        
        if (pl != NULL) //find a real match
            continue;
        
        if (sdestlen != 0)
        {
            if ((int)strlen(sdest) + 1 >= nsize-1)
                return -1;
            *(pdest+strlen(pdest)+1) = 0x00;
            memmove(pdest+1, pdest, strlen(pdest));
            *pdest = cflag;
        }
        sdestlen += strlen(pdest);
        pdest += strlen(pdest); 
    }
    
    if (nret == 100) //no enough space
        return -1;
    
    *pdest = 0x00;
    
    //base on ssrc2
    n = 0;
    while ((nret=getsubstr(pdest, nsize-sdestlen, ssrc2, n++, cflag)) == 0)
    {
        pp = ssrc1;
        while ((pl=strstr(pp, pdest)) != NULL)
        {
            pr = pl + strlen(pdest);
            if (*pr == cflag || *pr == 0x00) 
            {
                if (pl == pp) //match a real substr
                    break; 
                if (*(pl-1) == cflag) //match a real substr
                    break;
            }
            pp = pl+1;
        }
        
        if (pl != NULL) //find a real match
            continue;
        
        if (sdestlen != 0)
        {
            if ((int)strlen(sdest) + 1 >= nsize-1)
                return -1;
            *(pdest+strlen(pdest)+1) = 0x00;
            memmove(pdest+1, pdest, strlen(pdest));
            *pdest = cflag;
        }
        sdestlen += strlen(pdest);
        pdest += strlen(pdest); 
    }
    
    if (nret == 100) //no enough space
        return -1;
    
    *pdest = 0x00;
    
    return 0;
}

/***************************************************************
* �������� : strnotin
* ����     : liuyi
* ����     : ȡ���ַ���2�����ַ���1�е��Ӵ�
*            ssrc1 = "001,00002,005,0004"
*            ssrc2 = "002,005,001"
*            ���Ϊ "002" 
* ����     : char *sdest  - Ŀ�괮
*            int nsize    - Ŀ�괮�Ŀռ��С
*            char *ssrc1  - Դ��1
*            char *ssrc2  - Դ��2
*            char cflag   - �Ӵ��ָ���
* ����ֵ   : 0 - �ɹ�
*           -1 - Ŀ�괮�ռ䲻��
*           -2 - ��������
* �޸���ʷ : 
***************************************************************/
int strnotin(char *sdest, int nsize, char *ssrc1, char *ssrc2, char cflag)
{
    int n=0;
    int sdestlen=0;
    char *pdest,*pl,*pr,*pp;
    int nret;
    
    if (nsize <= 0) //error parameter
        return -2;
    
    memset(sdest, 0, nsize);
    
    //base on ssrc2
    pdest = sdest;
    
    while ((nret=getsubstr(pdest, nsize-sdestlen, ssrc2, n++, cflag)) == 0)
    {
        pp = ssrc1;
        while ((pl=strstr(pp, pdest)) != NULL)
        {
            pr = pl + strlen(pdest);
            if (*pr == cflag || *pr == 0x00) 
            {
                if (pl == pp) //match a real substr
                    break; 
                if (*(pl-1) == cflag) //match a real substr
                    break;
            }
            pp = pl+1;
        }
        
        if (pl != NULL) //find a real match
            continue;
        
        if (sdestlen != 0)
        {
            if ((int)strlen(sdest) + 1 >= nsize-1)
                return -1;
            *(pdest+strlen(pdest)+1) = 0x00;
            memmove(pdest+1, pdest, strlen(pdest));
            *pdest = cflag;
        }
        sdestlen += strlen(pdest);
        pdest += strlen(pdest); 
    }
    
    if (nret == 100) //no enough space
        return -1;
    
    *pdest = 0x00;
    
    return 0;
}

/***************************************************************
* �������� : getfileinfo
* ����     : rdc-zhangz
* ����     : ȡ���ļ�����Ϣ���ļ���С���������ڣ�����ʱ�䣬�޸�����
�޸�ʱ��
*            psfilename = "c:\text.dat"
nfilesize = 789
*            scdate = "1999-12-01"
*            sctime = "13:03:12"
smdate = "2000-03-09"
smtime = "09:03:45"
* ����     : const char *psfilename  - �ļ����ƣ���·����
*            int nfilesize    - �ļ���С
*            char *scdate  - ��������
*            int ncdtsize  - ��������Ŀ�Ĵ��Ŀռ��С
*            char *sctime  - ����ʱ��
*            int nctmsize  - ����ʱ��Ŀ�Ĵ��Ŀռ��С
*            char *smdate  - �޸�����
*            int nmdtsize  - �޸�����Ŀ�Ĵ��Ŀռ��С
*            char *smtime  - �޸�ʱ��
*            int nmtmsize  - �޸�ʱ��Ŀ�Ĵ��Ŀռ��С
* ����ֵ   : 0 - �ɹ�
*           -1 - �ļ�������
*           -2 - Ŀ�Ĵ��ռ䲻��
*           -3 - ��������
* �޸���ʷ : 
***************************************************************/
int getfileinfo( const char *psfilename, int &nfilesize, char *scdate, int ncdtsize, char *sctime, int nctmsize,
                char *smdate, int nmdtsize, char *smtime, int nmtmsize )
{
    struct stat stfile;
    tm * ptm;
    
    const unsigned int DATESIZE = 11;
    const unsigned int TIMESIZE = 9;
    
    if( 0 == *psfilename || NULL == psfilename )
        return -3;
    
    if( scdate == NULL || sctime == NULL 
        || smdate == NULL || smtime == NULL )
        return -3;
    
    if( ncdtsize < DATESIZE || nmdtsize < DATESIZE
        || nctmsize < TIMESIZE || nmtmsize < TIMESIZE )
        return -2;
    
    if( stat( psfilename, &stfile ) != 0 )
        return -1;
    
    nfilesize = stfile.st_size;		
    
    ptm = ::localtime(&stfile.st_ctime);
    
    sprintf( scdate, "%04d-%02d-%02d", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday );
    sprintf( sctime, "%02d:%02d:%02d", ptm->tm_hour, ptm->tm_min, ptm->tm_sec );
    
    ptm = ::localtime(&stfile.st_mtime);
    
    sprintf( smdate, "%04d-%02d-%02d", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday );
    sprintf( smtime, "%02d:%02d:%02d", ptm->tm_hour, ptm->tm_min, ptm->tm_sec );
    
    return 0;
}

/***************************************************************
* �������� : getsystime
* ����     : rdc-zhangz
* ����     : ȡ��ϵͳ���ڣ�ϵͳʱ��
*            ndate = 20031216
*            ntime = 13451612
* ����     : int ndate  - ϵͳ����(����)
*            int ntime  - ϵͳʱ��(���ͣ���ȷ���ٷ�֮һ��)
* ����ֵ   : void
* �޸���ʷ : 
***************************************************************/
void getsystime( long &ndate, long &ntime )
{
    timeb tmb;
    tm * ptm;
    
    ftime( &tmb );
    ptm = ::localtime( &tmb.time );
    
    ndate = 10000*(ptm->tm_year+1900) + 100*(ptm->tm_mon+1) + ptm->tm_mday;
    ntime = 1000000*ptm->tm_hour + 10000*ptm->tm_min + 100*ptm->tm_sec + tmb.millitm/10;
} 

/***************************************************************
* �������� : strrpl
* ����     : rdc-zhangz
* ����     : �ַ����滻
*          ssrc = "abcdef123ijklmn"
*          ssub = "123"
*          srpl = "*******"
*          sdest = "abcdef******ijklmn"
* ����     : char * sdest  - Ŀ�Ĵ�
*           int nsize  - Ŀ�Ĵ��ռ�Ĵ�С
*           char * ssrc - Դ��
*           char * ssub - Դ�����Ӵ�
*           char * srpl - �滻��
* ����ֵ   : >=0 - �ɹ��滻���Ӵ�����
*           -1 - ��������
*           -2 - Ŀ�Ĵ��ռ䲻��
* �޸���ʷ : 
***************************************************************/
int strrpl(char * sdest, int nsize, char * ssrc, const char *ssub, const char *srpl)
{
    char *pdest, *ps1, *ps2;
    int nsublen, nrpllen;
    int nleft;
	int nrplnum;
    
    if( NULL == sdest || NULL == ssrc || 0 == *ssrc 
        || NULL == ssub || NULL == srpl || nsize <= 0 )
        return -1;
    
	if( 0 == *ssub || 0 == *ssrc )
		return 0;

    nleft = nsize; 
    nsublen = strlen(ssub);
    nrpllen = strlen(srpl);
    
    pdest = sdest;	 
    ps1 = ssrc;
    ps2 = ps1;
    
	nrplnum = 0;
    while( (ps2=strstr(ps1,ssub)) != NULL )
    {
        if( nleft <= ps2-ps1+nrpllen )
            return -2;	
        nleft -= ps2-ps1+nrpllen;
        memcpy( pdest, ps1, ps2-ps1 );
        pdest += ps2-ps1;
        strcpy( pdest, srpl );
        pdest += nrpllen;
        ps1 = ps2+nsublen;        
		nrplnum++;
    }
    
    if( nleft <= (int)strlen(ps1) )
        return -2;
    strcpy( pdest, ps1 );
    
    return nrplnum;
}

/***************************************************************
* �������� : substr
* ����     : rdc-zhangz
* ����     : ȡ���ַ����Ӵ�
*          ssrc = "abcdef123ijklmn"
*          noffset = 6
*          ncnt = 3
*          sdest = "123"
*          ssrc = "a123"
*          noffset = 1
*          ncnt = 5
*          sdest = "123"
* ����     : char * sdest  - Ŀ�Ĵ�
*           unsigned int nsize  - Ŀ�Ĵ��ռ�Ĵ�С
*           char * ssrc - Դ��
*           unsigned int noffset - ��Դ����ȡ�ַ���λ��ƫ����
*           unsigned int ncnt - ȡ���ַ��ĸ���
* ����ֵ   : 0 - �ɹ�
*           -1 - ��������
*           -2 - Ŀ�Ĵ��ռ䲻��
* �޸���ʷ : 
***************************************************************/
int substr( char * sdest, unsigned int nsize, char *ssrc, unsigned int noffset, unsigned int ncnt )
{
    unsigned int nsrclen;
    
    if( sdest == NULL || ssrc == NULL )
        return -1;
    
    nsrclen = strlen(ssrc);
    
    if( noffset > nsrclen )
        return -1;
    
    if( nsize <= ncnt && nsize <= nsrclen-noffset )
        return -2;
    
    strncpy( sdest, ssrc+noffset, ncnt );		
    sdest[ncnt] = 0x00;
    return 0;
}

/***************************************************************
* �������� : strencrypt0
* ����     : rdc-zhangz
* ����     : �ַ����򵥼���, ֻ֧�����10λ�������Ӻ�Դ��
* ����     : const unsigned char* szsrc  - Դ����Ҫ���ܵ��ַ���
*           const unsigned char* szkey - KEY�����������Ӵ�
*           unsigned char * szresult - ���ɵļ��ܴ����̶�����Ϊ20���ֽ�
* ����ֵ   :
* �޸���ʷ : 
***************************************************************/
char * strencrypt0(const char * szsrc, const char * szkey, char * szresult)
{
    char szkey1[PWD_MAX_LEN * 2 + 1];
    char szsrc1[PWD_MAX_LEN * 2 + 1];
    char bit1[PWD_MAX_LEN * 8 + 1];
    char bit2[PWD_MAX_LEN * 8 + 1];
    char bit3[PWD_MAX_LEN * 2+ 1];
    int i, j, d;
    int h1, h2;

    memset(szkey1, 0, sizeof(szkey1));
    memset(szsrc1, 0, sizeof(szsrc1));
    memset(bit1, 0, sizeof(bit1));
    memset(bit2, 0, sizeof(bit2));
    memset(bit3, 0, sizeof(bit3));

	strncpy( szkey1, szkey, 10 );
	strncat( szkey1, "1234567890", 10-strlen(szkey1) );

    strncpy(szsrc1, szsrc, 10);
    strrev(szsrc1);

    strcat(szsrc1, "\x5f\x3c\x5e\x40\x5d\x3f\x5c\x3e\x5b\x3d");
    szsrc1[PWD_MAX_LEN] = 0;

    strrev(szsrc1);

    for(i = 0; i < PWD_MAX_LEN; i ++)
    {
        d = szsrc1[i] ^ szkey1[i];

        if(i == 0)
        {
            h1 = 237;
            h2 = 222;
        }
        else if(i == 1)
        {
            h1 = 88;
            h2 = 125;
        }
        else
        {
            h1 = szsrc1[i - 1];
            h2 = szsrc1[i - 2];
        }

        d = (d ^ h1) ^ h2;

        for(j = 7; j >= 0; j --)
        {
            if(d & (1 << j))
                bit1[strlen(bit1)] = '1';
            else
                bit1[strlen(bit1)] = '0';
        }
    }

    for(i = 0; i < 16; i ++)
    {
        for(j = 0; j < 5; j ++)
        {
            bit2[strlen(bit2)] = bit1[j * 16 + i];
        }
    }

    for(i = 0; i < 20; i ++)
    {
        d = 'A';
        for(j = 0; j < 4; j ++)
        {
            if(bit2[i * 4 + j] == '1')
                d = d + (1 << (3 - j));
        }
        bit3[i] = (char)d;
    }
    strcpy(szresult, bit3);
    return szresult;
}

/***************************************************************
* �������� : strdecrypt0
* ����     : rdc-zhangz
* ����     : �ַ������ܣ���Ӧstrencrypt0
* ����     : const char* szsrc  - Դ����Ҫ���ܵ��ַ���
*           const char* szkey - KEY�����������Ӵ�
*           char * szresult - ���ɵĽ��ܴ�
* ����ֵ   :
* �޸���ʷ : 
***************************************************************/
char * strdecrypt0(const char * szsrc, const char * szkey, char * szresult)
{  
	char bit1[PWD_MAX_LEN*8+1], bit2[PWD_MAX_LEN*8+1], bit3[PWD_MAX_LEN+1];
	char key[PWD_MAX_LEN+1];
    int i, j, d, h1, h2;
    char c[2], c1[2];

	memset( bit1, 0, sizeof( bit1 ) );
	memset( bit2, 0, sizeof( bit2 ) );
	memset( bit3, 0, sizeof( bit3 ) );
	memset( c, 0, sizeof( c ) );
	memset( c1, 0, sizeof( c1 ) );
	memset( key, 0, sizeof( key ) );

	strncpy( key, szkey, 10 );
	strncat( key, "1234567890", 10-strlen(key) );

	for( i = 0; i < 20; i++ )
	{
		d = (szsrc[i] - 'A') & 255;
		for( j = 0; j < 4; j++ )
		{
			if( (d & (1<<(3-j))) > 0 )
				c[0] = '1';
			else
				c[0] = '0';

			if( bit1[0] == 0 )
				bit1[0] = c[0];
			else
				strcat( bit1, c );
		}
	}
	for( i = 0; i < 5; i++ )
	{
		for( j = 0; j < 16; j++ )
		{
			c[0] = bit1[j*5+i];
			if( bit2[0] == 0 )
				bit2[0] = c[0];
			else
				strcat( bit2, c );
		}
	}
	for( i = 0; i < 10; i++ )
	{
		d = 0;
		for( j = 0; j < 8; j ++ )
		{
			if( bit2[i*8+j] == '1' )
				d = d + (1<<(7-j));
		}
		if( i == 0 )
		{
			h1 = 237;
			h2 = 222;
		}
		else 
		{
			if( i == 1)
			{
				h1 = 88;
				h2 = 125;
			}
			else
			{
				h1 = bit3[i-1];
				h2 = bit3[i-2];
			}
		}
		
		d = (d^h1)^h2;
		d = d^key[i];
		
		if( bit3[0] == 0 )
			bit3[0] = d;
		else
		{
			c1[0] = d;
			strcat( bit3, c1 );
		}		
	}
	c1[0] = 0;
	for( i = 0; i < 10; i++ )
	{
		if( (bit3[i] >= '0' && bit3[i] <= '9') 
			|| ((bit3[i] >= 'A' && bit3[i] <= 'Z'))
			|| ((bit3[i] >= 'a' && bit3[i] <= 'z')) )
		{
			if( szresult[0] == 0 )
				szresult[0] = bit3[i];
			else
			{
				c1[0] = bit3[i];
				strcat( szresult, c1 );
			}
		}
	}
	return szresult;
}

/***************************************************************
* �������� : isdigitalex
* ����     : rdc-zhangz
* ����     : �ж��ַ������Ƿ�ȫ�����֣�����(-+)��С����(.)
* ����     : const char* szsrc  - Դ����Ҫ�жϵ��ַ���
* ����ֵ   :
* �޸���ʷ : 
***************************************************************/
bool isdigitalex( const char * szSrc )
{
	for( const char *p = szSrc; *p != 0x00; p++ )
	{
		if( (*p > '9' || *p< '0') && *p != '.' )
		{
			if( p == szSrc && (*p == '-' || *p == '+') )
				continue;
			return false;
		}
	}
	return true;
}

/***************************************************************
* �������� : isdigital
* ����     : rdc-zhangz
* ����     : �ж��ַ������Ƿ�ȫ������
* ����     : const char* szsrc  - Դ����Ҫ�жϵ��ַ���
* ����ֵ   :
* �޸���ʷ : 
***************************************************************/
bool isdigital( const char * szSrc )
{
	for( const char *p = szSrc; *p != 0x00; p++ )
	{
		if( *p > '9' || *p< '0' )
			return false;
	}
	return true;
}

/***************************************************************
* �������� : isengchr
* ����     : rdc-zhangz
* ����     : �ж��ַ������Ƿ�ȫ��Ӣ����ĸ
* ����     : const char* szsrc  - Դ����Ҫ�жϵ��ַ���
* ����ֵ   :
* �޸���ʷ : 
***************************************************************/
bool isengchr( const char * szSrc )
{
	for( const char *p = szSrc; *p != 0x00; p++ )
	{
		if( !(( *p >= 'A' && *p <= 'Z')
			|| (*p >= 'a' && *p <= 'z')) )
			return false;
	}
	return true;
}

/***************************************************************
* �������� : isengchr
* ����     : rdc-zhangz
* ����     : �ж��ַ������Ƿ�ȫ��Ӣ����ĸ������
* ����     : const char* szsrc  - Դ����Ҫ�жϵ��ַ���
* ����ֵ   :
* �޸���ʷ : 
***************************************************************/
bool isengchrordigital( const char * szSrc )
{
	for( const char *p = szSrc; *p != 0x00; p++ )
	{
		if( !( ( *p >= 'A' && *p <= 'Z')
			|| (*p >= 'a' && *p <= 'z')
			|| (*p >= '0' && *p <= '9') ) )
			return false;
	}
	return true;
}

/***************************************************************
* �������� : chrisnull
* ����     : rdc-zhangz
* ����     : �ж��ַ��Ƿ�Ϊ0
* ����     : const char chrsrc  - Դ�ַ�
* ����ֵ   : char �����0, ����0x20�����򷵻�����
* �޸���ʷ : 
***************************************************************/
char chrisnull( char chrsrc )
{
	return (chrsrc==0?0x20:chrsrc);
}

/***************************************************************
* �������� : datediff
* ����     : ���������ڵĲ�ֵ��������
* ����     : unsigned int date1 �������ڣ���ʽ�磺20040301������>19700101
             unsigned int date2 �������ڣ���ʽ�磺20040229������>19700101
             int &ndaydiff      ��������
             ����:date1(20040228) - date2(20040302) = nday(-3)
* ����ֵ   : int 0 ��ȷ�� ��0 �������ڸ�ʽ����
* �޸���ʷ : 
***************************************************************/
int datediff( unsigned int date1, unsigned int date2, int &ndaydiff )
{
    time_t tm0, tm1, tm2;
    struct tm sttm1, sttm2;
    
    sttm1.tm_year = date1/10000-1900;
    if( sttm1.tm_year < 0 )
        return 1;
            
    sttm1.tm_mon=(date1/100)%100-1;
    if( sttm1.tm_mon < 0 || sttm1.tm_mon > 11 )
        return 2;
        
    sttm1.tm_mday = date1%100;
    if( sttm1.tm_mday < 1 || sttm1.tm_mday > 31 )
        return 3;
        
    sttm1.tm_hour = 0;
    sttm1.tm_min  = 0;
    sttm1.tm_sec  = 0;
    sttm1.tm_isdst = -1;
    
    sttm2.tm_year = date2/10000-1900;
    if( sttm2.tm_year < 0 )
        return 1;

    sttm2.tm_mon  = (date2/100)%100 - 1;
    if( sttm2.tm_mon < 0 || sttm2.tm_mon > 11 )
        return 2;

    sttm2.tm_mday = date2%100;
    if( sttm2.tm_mday < 1 || sttm2.tm_mday > 31 )
        return 3;

    sttm2.tm_hour = 0;
    sttm2.tm_min  = 0;
    sttm2.tm_sec  = 0;
    sttm2.tm_isdst = -1;

    tm1 = mktime( &sttm1 );
    if( tm1 == -1 )
        return 4;
    
    tm2 = mktime( &sttm2 );
    if( tm2 == -1 )
        return 5;
        
    tm0 = tm1 - tm2;
    ndaydiff = tm0/(60*60*24);
    return 0;
}

/***************************************************************
* �������� : datediff
* ����     : ���������ڵĲ�ֵ
* ����     : char cDatepart ָ�����ڵ���һ���֣�'Y'��, 'M'��, 'D'��
			 unsigned int date1 �������ڣ���ʽ�磺20040301������>19700101
             unsigned int date2 �������ڣ���ʽ�磺20040229������>19700101
             int &ndaydiff      ��������
             ����:date1(20040228) - date2(20040302) = nday(-3)
* ����ֵ   : int 0 ��ȷ�� ��0 �������ڸ�ʽ����
* �޸���ʷ : 
***************************************************************/
int datediff( unsigned int date1, unsigned int date2, char cDatepart, int &nNum )
{
    time_t tm0, tm1, tm2;
    struct tm sttm1, sttm2;
    
    sttm1.tm_year = date1/10000-1900;
    if( sttm1.tm_year < 0 )
        return 1;
            
    sttm1.tm_mon=(date1/100)%100-1;
    if( sttm1.tm_mon < 0 || sttm1.tm_mon > 11 )
        return 2;
        
    sttm1.tm_mday = date1%100;
    if( sttm1.tm_mday < 1 || sttm1.tm_mday > 31 )
        return 3;
        
    sttm1.tm_hour = 0;
    sttm1.tm_min  = 0;
    sttm1.tm_sec  = 0;
    sttm1.tm_isdst = -1;
    
    sttm2.tm_year = date2/10000-1900;
    if( sttm2.tm_year < 0 )
        return 1;

    sttm2.tm_mon  = (date2/100)%100 - 1;
    if( sttm2.tm_mon < 0 || sttm2.tm_mon > 11 )
        return 2;

    sttm2.tm_mday = date2%100;
    if( sttm2.tm_mday < 1 || sttm2.tm_mday > 31 )
        return 3;

    sttm2.tm_hour = 0;
    sttm2.tm_min  = 0;
    sttm2.tm_sec  = 0;
    sttm2.tm_isdst = -1;

    tm1 = mktime( &sttm1 );
    if( tm1 == -1 )
        return 4;
    
    tm2 = mktime( &sttm2 );
    if( tm2 == -1 )
        return 5;

	if (cDatepart == 'Y')
	{	
		nNum = sttm1.tm_year - sttm2.tm_year;
	}
	else if (cDatepart == 'M')
	{
		nNum = (sttm1.tm_year*12+sttm1.tm_mon) - (sttm2.tm_year*12+sttm2.tm_mon);
	}
	else if (cDatepart == 'D')
	{
		tm0 = tm1 - tm2;
		nNum = tm0/(60*60*24);
	}
    return 0;
}

/***************************************************************
* �������� : dateplus
* ����     : rdc-zhangz
* ����     : ��һ�����ڼ���һ�������ĺ�ֵ�����ڣ�
* ����     : unsigned int date1 �������ڣ�  ��ʽ�磺20040301������>19700101
             int nday           ���ϵ�����
             unsigned int date2 �õ�������  ��ʽ�磺20040301������>19700101
             ����:date1(20040228) + nday(3) = date2(20040302)
* ����ֵ   : int 0 ��ȷ�� ��0 �������ڸ�ʽ����
* �޸���ʷ : 
***************************************************************/
int dateplus( unsigned int date1, int nday, unsigned int &date2 )
{
    time_t tm0, tm1;
    struct tm sttm1, *psttm2;
    
    sttm1.tm_year = date1/10000-1900;
    if( sttm1.tm_year < 0 )
        return 1;
            
    sttm1.tm_mon=(date1/100)%100-1;
    if( sttm1.tm_mon < 0 || sttm1.tm_mon > 11 )
        return 2;
        
    sttm1.tm_mday = date1%100;
    if( sttm1.tm_mday < 1 || sttm1.tm_mday > 31 )
        return 3;
        
    sttm1.tm_hour = 0;
    sttm1.tm_min  = 0;
    sttm1.tm_sec  = 0;
    sttm1.tm_isdst = -1;

    tm1 = mktime( &sttm1 );
    if( tm1 == -1 )
        return 4;

	//00:00:00 ���һ�� so +1
    tm0 = tm1 + nday*24*60*60 + 24*60*60;
    
    psttm2 = gmtime(&tm0);
    if( psttm2 == NULL )
        return 5;

    date2 = (psttm2->tm_year+1900)*10000 + (psttm2->tm_mon+1)*100 + psttm2->tm_mday;    
    return 0;
}

/***************************************************************
* �������� : dateplus
* ����     : yangjianli
* ����     : ��һ�����ڼ���һ�������ĺ�ֵ�����ڣ�
* ����     : unsigned int date1 �������ڣ�  ��ʽ�磺20040301������>19700101
			 char cDatepart		ָ�����ڵ���һ���֣�'Y'��, 'M'��, 'D'��
             int nNum           ���ӵ���ֵ
             unsigned int date2 �õ�������  ��ʽ�磺20040301������>19700101
             ����:date1(20040228) + nday(3) = date2(20040302)
* ����ֵ   : int 0 ��ȷ�� ��0 �������ڸ�ʽ����
* �޸���ʷ : 
***************************************************************/
int dateplus( unsigned int date1, char cDatepart, int nNum, unsigned int &date2 )
{
    struct tm sttm1;
    
    sttm1.tm_year = date1/10000-1900;
    if( sttm1.tm_year < 0 )
        return 1;
            
    sttm1.tm_mon=(date1/100)%100-1;
    if( sttm1.tm_mon < 0 || sttm1.tm_mon > 11 )
        return 2;
        
    sttm1.tm_mday = date1%100;
    if( sttm1.tm_mday < 1 || sttm1.tm_mday > 31 )
        return 3;
        
    sttm1.tm_hour = 0;
    sttm1.tm_min  = 0;
    sttm1.tm_sec  = 0;
    sttm1.tm_isdst = -1;

	if (cDatepart == 'Y')
	{
		sttm1.tm_year += nNum;		
		date2 = (sttm1.tm_year+1900)*10000 + (sttm1.tm_mon+1)*100 + sttm1.tm_mday;    
	}
	else if (cDatepart == 'M')
	{
		int nOld=sttm1.tm_mon+1;
        int nYear=nNum/12;

        if( nOld+nNum > 12 )
                nYear++;
        if( nOld+nNum < 1 )
                nYear--;

        int nNew=(nOld+nNum)%12;
        if( nNew==0 ) nNew=12;
        sttm1.tm_mon=nNew;
        sttm1.tm_year+=nYear;

		date2 = (sttm1.tm_year+1900)*10000 + (sttm1.tm_mon)*100 + sttm1.tm_mday;    
	}
	else if (cDatepart == 'D')
	{
	    time_t tm0, tm1;
		struct tm *psttm2;

		tm1 = mktime( &sttm1 );
		if( tm1 == -1 )
			return 4;

		//00:00:00 ���һ�� so +1
		tm0 = tm1 + nNum*24*60*60 + 24*60*60;
    
		psttm2 = gmtime(&tm0);
		if( psttm2 == NULL )
			return 5;

		date2 = (psttm2->tm_year+1900)*10000 + (psttm2->tm_mon+1)*100 + psttm2->tm_mday;    
	}
	else
		return 6;

    return 0;
}

/***************************************************************
* �������� : strdistinct
* ����     : rdc-zhangz
* ����     : ɾ���ַ������ظ����ַ�
* ����     : char * psrc �����ַ���
             ����:strdistinct( "112233" ) = "123";
* ����ֵ   : no
* �޸���ʷ : 
***************************************************************/
void strdistinct(char * psrc)
{
	char cchr;
	int nsrclen, i, j;
	
	if( psrc == NULL || *psrc == 0x00 )
		return;
	//�ȼ�¼ԭ�ַ������ȣ��ٽ�һ���ַ�����������ͬ���ַ��滻��0x00('\0')
	nsrclen = strlen(psrc);
	for( i = 0; i < nsrclen; i++ )
	{
		if( (cchr = psrc[i]) == 0x00 ) //�Ѿ����滻
			continue;
		for( j = i+1; j < nsrclen; j++ )
		{
			if( cchr == psrc[j] )
			{
				psrc[j] = 0x00;       //�ҵ��ظ��ģ��滻��
			}
		}
	}

	//�ƶ��ַ��������ַ����е�'\0'ȥ��
	int num = 0; //��¼�ж��ٸ�'\0'
	for( i = 0; i < nsrclen; i++ )
	{
		if( psrc[i] == 0x00 ) //�Ѿ����滻
		{
			num++;
		}
		else
		{
			psrc[i-num] = psrc[i];
		}
	}
}

/***************************************************************
* �������� : strdelchr
* ����     : rdc-zhangz
* ����     : ɾ���ַ�����ĳ���ַ�
* ����     : char * src �����ַ���
             ����:strdelchr( "11,22,33",',' ) = "112233";
* ����ֵ   : no
* �޸���ʷ : 
***************************************************************/
void strdelchr(char * str, char cflag )
{
	if( str == NULL || *str == 0x00 || cflag == 0x00 )
		return;

	int i = 0;
	for(i = 0; *str != 0x00; str++ )
	{
		if( *str == cflag )
		{
			i++;
		}
		else
		{
			*(str-i) = *str;
		}
	}

	*(str-i) = 0x00;
}

/***************************************************************
* �������� : strdelstrchr
* ����     : lizb
* ����     : ���ַ���1�� ɾ�� �ַ���2�������ַ�
* ����     : char * operstr �����ַ���
             char * parstr  �����ַ���

             ����: strdelstrchr( "ABCDEF" , 'AE' ) = "BCDF";
				   strdelstrchr( "ABCDEF" , 'ACE' ) = "BDF";

* ����ֵ   : no
* �޸���ʷ : 
***************************************************************/
void strdelstrchr(char * operstr, char * parstr)
{
	if( operstr == NULL || *operstr == 0x00 || parstr == NULL || *parstr == 0x00 )
	{
		return;
	}

	for( unsigned i = 0; i < strlen(parstr); i++ )
	{
		strdelchr(operstr, parstr[i]);
	}

	return;
}

/***************************************************************
* �������� : b2t
* ����     : rdc-zhangz
* ����     : ������ת�����ı�
             ԭ��һ���ַ���ASCII�ֳ�����ʮ�����Ƶ��ַ���ʾ
			 'A'(ASCII: 65(10), 0x41 ) --> '41'
* ����     : void *pvd    -- Դ�����ƴ�
             int   nlen   -- Դ�����ƴ��ĳ���
			 char *pszbuf -- Ŀ���ı�������
			 int   nsize  -- Ŀ���ı���������С��Ӧ�ô��ڵ���2*len+1
* ����ֵ   : 0 �ɹ� �� 0 ʧ��
             -1 -- ������̫С
* ע��     : nsize�Ĵ�СӦ�ú���0x00�ַ�����β��
* �޸���ʷ : 
***************************************************************/
int b2t(void *pvd, int nlen, char *pszbuf, int nsize)
{
	unsigned char chr1, chr2;
	unsigned char *ps;

	ps = (unsigned char*)pvd;
	if( 2*nlen+1 > nsize )
		return -1;

	//0x41 = 'A', 0x30 = '0'
	for( int i = 0; i < nlen; i++ )
	{
		chr1 =  (*(ps+i))>>4;     //�������λ��ֵ
		chr1 += chr1>9?0x41:0x30; 
		chr2 =  (*(ps+i))&0x0F;   //�������λ��ֵ
		chr2 += chr2>9?0x41:0x30;
		*pszbuf = chr1;
		pszbuf++;
		*pszbuf = chr2;
		pszbuf++;
	}
	*pszbuf = 0x00;
	return 0;
}

/***************************************************************
* �������� : t2b
* ����     : rdc-zhangz
* ����     : �ı�ת���ɶ����ƣ�
* ����     : char *pstr  -- Դ�ı���
             void *pvd   -- Ŀ�Ķ����ƻ�����
			 int   nsize -- Ŀ�Ķ����ƻ�������С
* ����ֵ   : 0 �ɹ� �� 0 ʧ��
             -1 -- ������̫С
			 -2 -- Դ�ı�����ʽ����
* �޸���ʷ : 
***************************************************************/
int t2b(char *pstr, void *pbuf, int nsize)
{
	unsigned char chr1, chr2;
	unsigned char *ps;

	ps = (unsigned char*)pbuf;
	if( 2*nsize < (int)strlen(pstr) )
		return -1;

	//0x41 = 'A', 0x30 = '0'
	while( *pstr )
	{
		chr1 =  *pstr;
		chr1 -= chr1>0x39?0x41:0x30;  //��ԭ����λ
		pstr++;
		chr2 =  *pstr;
		chr2 -= chr2>0x39?0x41:0x30;  //��ԭ����λ
		pstr++;
		if( chr1 > 0x0F || chr2 > 0x0F )
			return -2;
		*ps = (chr1<<4)|chr2;         //�ϳ�ԭ�ַ�
		ps++;
	}
	return 0;
}

//Base64��MIME�ʼ��г��õı��뷽ʽ֮һ��������Ҫ˼���ǽ�������ַ��������ݱ����ֻ����{'A'-'Z', 'a'-'z', '0'-'9', '+', '/'}��64���ɴ�ӡ�ַ��Ĵ����ʳ�Ϊ��Base64���� 
//Base64����ķ����ǣ�������������ÿ��ȡ6 bit���ô�6 bit��ֵ(0-63)��Ϊ����ȥ��������Ӧ�ַ���������ÿ3���ֽڽ�����Ϊ4���ַ�(3��8 �� 4��6)������4���ַ�����'='��䡣 
const char EnBase64Tab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
 
int EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen)
{
    unsigned char c1, c2, c3;    // ���뻺��������3���ֽ�
    int nDstLen = 0;             // ������ַ�����
    int nDiv = nSrcLen / 3;      // �������ݳ��ȳ���3�õ��ı���
    int nMod = nSrcLen % 3;      // �������ݳ��ȳ���3�õ�������
 
    // ÿ��ȡ3���ֽڣ������4���ַ�
    for( int i = 0; i < nDiv; i++ )
    {
        // ȡ3���ֽ�
        c1 = *pSrc++;
        c2 = *pSrc++;
        c3 = *pSrc++;
 
        // �����4���ַ�
        *pDst++ = EnBase64Tab[c1 >> 2];
        *pDst++ = EnBase64Tab[((c1 << 4) | (c2 >> 4)) & 0x3f];
        *pDst++ = EnBase64Tab[((c2 << 2) | (c3 >> 6)) & 0x3f];
        *pDst++ = EnBase64Tab[c3 & 0x3f];
        nDstLen += 4; 
    }
 
    // �������µ��ֽ�
    if (nMod == 1)
    {
        c1 = *pSrc++;
        *pDst++ = EnBase64Tab[(c1 & 0xfc) >> 2];
        *pDst++ = EnBase64Tab[((c1 & 0x03) << 4)];
        *pDst++ = '=';
        *pDst++ = '=';
        nDstLen += 4;
    }
    else if (nMod == 2)
    {
        c1 = *pSrc++;
        c2 = *pSrc++;
        *pDst++ = EnBase64Tab[(c1 & 0xfc) >> 2];
        *pDst++ = EnBase64Tab[((c1 & 0x03) << 4) | ((c2 & 0xf0) >> 4)];
        *pDst++ = EnBase64Tab[((c2 & 0x0f) << 2)];
        *pDst++ = '=';
        nDstLen += 4;
    }
 
    // ����Ӹ�������
    *pDst = '\0';
 
    return nDstLen;
}

//Base64���뷽���У���򵥵�Ҳ�ǲ������64���ɴ�ӡ�ַ���ֵ��Ϊ������
//���õ���ֵ����ΧΪ0-63��������������ƴ�ճ��ֽ���ʽ������͵õ��������� 
const char DeBase64Tab[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    62,        // '+'
    0, 0, 0,
    63,        // '/'
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61,        // '0'-'9'
    0, 0, 0, 0, 0, 0, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,        // 'A'-'Z'
    0, 0, 0, 0, 0, 0,
    26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
    39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,        // 'a'-'z'
};
 
int DecodeBase64(const char* pSrc, unsigned char* pDst, int nSrcLen)
{
    int nDstLen;            // ������ַ�����
    int nValue;             // �����õ��ĳ�����
    int i;
 
    i = 0;
    nDstLen = 0;
 
    // ȡ4���ַ������뵽һ�����������پ�����λ�õ�3���ֽ�
    while (i < nSrcLen)
    {
		nValue = DeBase64Tab[*pSrc++] << 18;
		nValue += DeBase64Tab[*pSrc++] << 12;
		*pDst++ = (nValue & 0x00ff0000) >> 16;
		nDstLen++;
		
		if (*pSrc != '=')
		{
			nValue += DeBase64Tab[*pSrc++] << 6;
			*pDst++ = (nValue & 0x0000ff00) >> 8;
			nDstLen++;
			
			if (*pSrc != '=')
			{
				nValue += DeBase64Tab[*pSrc++];
				*pDst++ =nValue & 0x000000ff;
				nDstLen++;
			}
		}
		
		i += 4;
     }
 
    // ����Ӹ�������
    *pDst = '\0';
 
    return nDstLen;
}
int strConvert(char * psrc)
{
	int nsrclen, i;
	
	if( psrc == NULL || *psrc == 0x00 )
		return -1;
	//�ȼ�¼ԭ�ַ������ȣ��ٽ�һ���ַ�����������ͬ���ַ��滻��0x00('\0')
	nsrclen = strlen(psrc);
	for( i = 0; i < nsrclen; i++ )
	{
		if( '0' <= psrc[i] && psrc[i] <= '9' ) //�Ѿ����滻
			continue;
        else
        {
            return -1;
        }
	}

    return 0 ;
}

void fillcomma(char * sStrin)
{
	char  sTmp[2048] = {0};

	strcpy(sTmp, sStrin);
	sprintf(sTmp, ",%s,", sStrin);
	strcpy(sStrin, sTmp);

}

/***************************************************************
* �������� : charreplchar
* ����     : luyp  
* ����     : �滻�ַ����е�ָ���ַ�(Դ�����ı�)
* ����     : char *sSrc   - Դ�ַ�
*            char cTarget - ָ���ַ�
*            char cRepl   - �滻�ַ�
* ����ֵ   : 0    - �ɹ�
*            -1   - ָ�����滻�ַ�Ϊ0x00
*            -2   - Դ������Ϊ��
* �޸���ʷ : 
***************************************************************/
int charreplchar(char *sSrc, char cTarget, char cRepl)
{
	char *ps = NULL;

	if(cTarget == 0 && cRepl == 0)
		return -1;
	if(strlen(sSrc) == 0)
		return -2;
	while((ps = strchr(sSrc, cTarget)) != NULL)
		ps[0] = cRepl;

	return 0;
}

/***************************************************************
* �������� : cutsubbetweenflagstr
* ����     : luyp  
* ����     : ���ݷָ�����ָ���ַ����н�ȡ�����ָ����е��Ӵ�(Դ�����ı�)
* ����     : char *sdest  - ��ȡ�����Ӵ�
*            int nsize    - �Ӵ��Ŀռ��С
*            char *ssrc   - Դ��
*            char cflag   - �ָ���(����Ϊ0x00)
* ����ֵ   : 0   - �ɹ�
*            1   - Դ��Ϊ��(sdest���ÿ�)
*            100 - �Ӵ��ָ�������
*            200 - ��������
*			 300 - �Ӵ��ռ䲻��
* �޸���ʷ : 
***************************************************************/
int cutsubbetweenflagstr(char *sDest, int nSize, char *sSrc, char cStartFlag, char cEndFlag)
{
	char *psStart = NULL;
	char *psEnd = NULL;

	if( nSize < 1 || cStartFlag == 0 || cEndFlag==0)
        return 200;
    
    if( strlen(sSrc) == 0 )
    {
        *sDest = 0x00;
        return 1;
    }

	psStart = strchr( sSrc, cStartFlag);
	psEnd = strchr( sSrc, cEndFlag);

	if((psStart==NULL)||(psEnd==NULL))
	{
		return 100;
	}
	else
	{
		//int ilen = psStart-psEnd;
		if((psEnd-psStart-1)<nSize)
		{
			memcpy(sDest, sSrc+1, psEnd-psStart-1);
			sDest[psEnd-psStart] = 0x00;
			strcpy(sSrc, psEnd+1);
		}
		else
			return 300;
	}
	return 0;
}

/******************************************************
*	FunctionName:Idno15or18
*	Author:		 luyp
*	CDate:		 2006-06-17
*	Version:	 1.0
*	Desc:		 ʮ��λ���֤��ʮ��λ���֤֮���ת����
*				 �������ʮ�ˣ�ת����15�����15ת����
*				 18λ,��ʮ��λת��ʮ�˾���������1900��
*				 1999��֮�䡣
*	Input Params:
	char *sTostr:		����ֵ
	const char *sSstr:	����Դ��
	int nLen:			���봮����,ֻ��Ϊ15��18
	return Value:
	int:	0~�ɹ�; -1~���봮���Ⱥ����봮�ĳ��Ȳ�һ��;
			-2~���봮���ȣ�����15��18;-3~�����зǷ��ַ�
******************************************************/
int Idno15or18(char *sTostr, const char *sSstr, int nLen)
{
	int nKey[18] = {7,9,10,5,8,4,2,1,6,3,7,9,10,5,8,4,2,1};
	char cKey[11] = {'1','0','X','9','8','7','6','5','4','3','2'};
	int n=0, nSum=0, i=0, j=0;
	memset(sTostr, 0x00, strlen(sTostr));
	switch(nLen)
	{
	case 18:
		for(i=16; i>=0; i--)
		{
			j = sSstr[i]-'0';
			if(sSstr[i]>'9' || sSstr[i]<'0')
				return -3;
		}
		if(sSstr[17]!='X' || sSstr[17]>'9' || sSstr[17]<'0')
		if(strlen(sSstr) != 18)
			return -1;
		strncpy(sTostr, sSstr, 15);
		strncpy(sTostr+6, sSstr+8, 9);
		sTostr[15] = '\0';
		break;
	case 15:
		for(i=14; i>=0; i--)
		{
			j = sSstr[i]-'0';
			if(sSstr[i]>'9' || sSstr[i]<'0')
				return -3;
		}
		if(strlen(sSstr) != 15)
			return -1;
		strncpy(sTostr, sSstr, 15);
		strncpy(sTostr+6, "19", 2);
		strncpy(sTostr+8, sSstr+6, 9);
		for(i=16; i>=0; i--)
		{
			n = (sTostr[i]-'0') * nKey[i];
			nSum += n;
		}

		sTostr[17] = cKey[nSum%11];
		sTostr[18] = '\0';
		break;
	default:
		return -2;
	}
	return 0;
}
/***************************************************************
* �������� : strright
* ����     : 
* ����     : ȡ��Nλ�ַ���������Ŀ�괮��
			 ����ַ�������Nλ����ô������'cflag'
* ����     : char *sdest	Ŀ���ַ�����
			 int  nsize		Ŀ���ַ�������
			 char *ssrc		Դ�ַ���
			 int  ncnt      �ұ�Nλ
			 char cflag     ��������ַ�
* ����ֵ   : ��
* �޸���ʷ : 
***************************************************************/
int strright(char *sdest, int nsize, char *ssrc, int ncnt, char cflag)
{
	int nsrclen;

    if( ncnt <= 0 || nsize <= 0 || cflag == 0 )
        return 200;

	if (nsize <= ncnt)
		return 100;
    
    nsrclen = strlen(ssrc);
    
	if (nsrclen >= ncnt)
		strcpy(sdest, ssrc+(nsrclen-ncnt));
	else
	{
        memset( sdest, cflag, ncnt-nsrclen );
        strcpy( sdest+ncnt-nsrclen, ssrc );
	}
	return 0;
}
/***************************************************************
* �������� : getchrnum
* ����     : rdc-zhangz
* ����     : �ַ����滻
* ����     : 
*            char * psrc    - Դ��
*            char   cflag   - ���ҵ��ַ�
* ����ֵ   : int �����ַ��ĸ���
* �޸���ʷ : 
***************************************************************/
int getchrnum(char *ssrc, char cflag)
{
    int num = 0;
    
    while(*ssrc)
    { 
        if(*ssrc==cflag)
            num++;
		ssrc++;
    }
    return num;   
}

/***************************************************************
* �������� : charindextoin
* ����     : rdc-zhangcm
* ����     : ��chindexת��Ϊin
* ����     : 
*            char * ssrc    - Դ��
*            char   sdest   - Ŀ�Ĵ�
*            char   ssprt   - �ָ����
* ����ֵ   : int Ŀ�Ĵ�
* �޸���ʷ : 
***************************************************************/
char *charindextoin(char *ssrc, char *sdest, char ssprt)
{
	char sTmpBuf[16]={0};
	int  nPos=0;
	while(true)
	{
		if(getsubstr( sTmpBuf, sizeof(sTmpBuf), ssrc, nPos, ssprt ))
			break;
		else
		{
			if(nPos!=0)
				sprintf(sdest,"%s,'%s'",sdest,sTmpBuf);				
			else
				sprintf(sdest,"'%s'",sTmpBuf);
		}
		nPos++;
	}

	return sdest;
}

/***************************************************************
* �������� : dayOfWeek
* ����     : rdc-zhangcm
* ����     : ������ת��Ϊ��
* ����     : 
*            int  ndate    - ����
*            
*            
* ����ֵ   : int ����
* �޸���ʷ : 
***************************************************************/
int dayOfWeek(long ndate)   
{       
	int year=0;
	int month=0;
	int day=0;
	int nWeek=0;
	year = ndate / 10000;
	month = ndate / 100 - year*100;
    day = ndate - year*10000 - month*100;
	//�Ϸ����ж�
	if(month > 12 || day > 31 || (month == 2 && day > 29))
		return nWeek;
	
	static   int   _month[12]={31,0,31,30,31,30,31,31,   
		30,31,30,31};   
	if(year%4==0   &&   year%100!=0   ||   year%400==0)   
		_month[1]=29;   
	else   _month[1]=28;   
	int   C=0;   
	for(int   i=0;i<month-1;++i)   
		C+=_month[i];   
	C+=day;   
	int   S=year-1+(year-1)/4-(year-1)/100+(year-1)/400+C;   
	
	nWeek = S%7;
	if(nWeek == 0)
		nWeek =7;

	return nWeek;   
}

/***************************************************************
* �������� : dateplus
* ����     : rdc-cdm
* ����     : ��һ�����ڼ�ȥ��һ�������Ĳ�ֵ�����ڣ�
* ����     : unsigned int date1 ��ʼ���ڣ�  ��ʽ�磺20040302������>19700101
             int nday           ��ȥ������
             unsigned int date2 �õ�������  ��ʽ�磺20040228������>19700101
             ����:date1(20040302) - nday(3) = date2(20040228)
* ����ֵ   : int 0 ��ȷ�� ��0 �������ڸ�ʽ����
* �޸���ʷ : 
***************************************************************/
int datesubtract( int date1, int nday, int &date2 )
{
    time_t tm0, tm1;
    struct tm sttm1, *psttm2;
    
    sttm1.tm_year = date1/10000-1900;
    if( sttm1.tm_year < 0 )
        return 1;
            
    sttm1.tm_mon=(date1/100)%100-1;
    if( sttm1.tm_mon < 0 || sttm1.tm_mon > 11 )
        return 2;
        
    sttm1.tm_mday = date1%100;
    if( sttm1.tm_mday < 1 || sttm1.tm_mday > 31 )
        return 3;
        
    sttm1.tm_hour = 0;
    sttm1.tm_min  = 0;
    sttm1.tm_sec  = 0;
    sttm1.tm_isdst = -1;

    tm1 = mktime( &sttm1 );
    if( tm1 == -1 )
        return 4;

	//00:00:00 ���һ�� so +1
    tm0 = tm1 - nday*24*60*60 + 24*60*60;
    
    psttm2 = gmtime(&tm0);
    if( psttm2 == NULL )
        return 5;

    date2 = (psttm2->tm_year+1900)*10000 + (psttm2->tm_mon+1)*100 + psttm2->tm_mday;    
    return 0;
}
