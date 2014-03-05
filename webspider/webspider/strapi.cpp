/***************************************************************
* 版权所有 : 2003-2010, 深圳金证科技股份公司.
* 文件名   : kdstrapi.cpp
* 作者     : rdc-zhangz  
* 版本     : 1.0.0.0 
* 创建日期 : 20030916
* 描述     : 字符串操作函数集合
* 其他     : 
* 函数列表 : 
     
  ltrimchr                  删除字符串头部的某字符
  rtrimchr                  删除字符串尾部的某字符
  alltrimchr                删除字符串头部的某字符
  ltrim                     删除字符串头部的空格
  rtrim                     删除字符串尾部的空格
  alltrim                   删除字符串头部和尾部的空格
  lfill                     在字符串头部填满指定字符
  rfill                     在字符串尾部填满指定字符
  lfillex                   在字符串头部填满指定字符
  rfillex                   在字符串尾部填满指定字符
  getsubstr                 根据分隔符和序号从指定字符串中取相应子串
  cutsubstr                 根据分隔符从指定字符串中逐个截取子串
  lstrncat                  在字符串头部追加指定字符串的前n个字符
  lstrcat                   在字符串头部追加指定字符串
  rstrncat                  在字符串尾部追加指定字符串的前n个字符,比strncat安全，不易越界
  rstrcat                   在字符串尾部追加指定字符串，比strcat安全，不易越界
  strand                    字符串与操作，提取2个字符串都包含的子串
  stror                     字符串或操作，合并2个字符串，使目标串包含2个字符串中全部子串
  strxor                    字符串异或操作，取出2个字符串中不同的子串
  strnotin                  取出字符串2不在字符串1中的子串
  strchrand                 字符串与操作，提取2个字符串都包含的子串

  getfileinfo               得到文件信息
  getsystime                取得系统日期时间
  strrpl                    字符串替换

  strencrypt0               字符串加密方法0
  strdecrypt0               字符串解密方法0

  chrisnull                 判断字符是否为NULL
  isengchrordigital         判断字符是否为数字或英文字符
  isengchr                  判断字符是否为英文字符
  isdigital                 判断字符是否为数字
  isdigitalex               判断字符串中是否全是数字，正负(-+)，小数点(.)

  datediff                  求两个日期的差值（天数）
  dateplus                  求一个日期加上一定天数的和值（日期）
  datesubtract              求一个日期减去一定天数的差值（日期）

  strdistinct               删除字符串中重复的字符
  strdelchr                 删除字符串中某个字符
  strdelstrchr              在字符串1中 删除 字符串2包含的字符
  t2b                       简单文本转换成二进制
  b2t                       简单二进制转换成文本

  strright						取字符串右边N位字符
  cutsubbetweenflagstr		根据分隔符从指定字符串中截取两个分隔符中的子串(源串被改变)
  Idno15or18				十五位身份证和十八位身份证之间的转化
* 修改历史 :    

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
* 函数名称 : round
* 作者     : rdc-zhangz 
* 描述     : 四舍五入
* 参数     : double dbIn  要四舍五入的浮点数
             int nRatio   在那一位开始四舍五入
			 int nPrec    要求的精度   
* 返回值   : 无
* 修改历史 : 
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
* 函数名称 : ltrimchr
* 作者     : rdc-zhangz  
* 描述     : 删除字符串头部的某字符的串
* 参数     : char *str  要操作的串
             char cflag 要删除的字符
* 返回值   : 无
* 修改历史 : 
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
* 函数名称 : rtrimchr
* 作者     : rdc-zhangz  
* 描述     : 删除字符串尾部的某字符的串
* 参数     : char *str  要操作的串
             char cflag 要删除的字符
* 返回值   : 无
* 修改历史 : 
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
* 函数名称 : allltrimchr
* 作者     : rdc-zhangz  
* 描述     : 删除字符串头尾部的某字符的串
* 参数     : char *str  要操作的串
             char cflag 要删除的字符
* 返回值   : 无
* 修改历史 : 
***************************************************************/
void alltrimchr( char *str, char cflag )
{
	ltrimchr( str, cflag );
	rtrimchr( str, cflag ) ;
	return ;
}

/***************************************************************
* 函数名称 : ltrim
* 作者     : rdc-zhangz  
* 描述     : 删除字符串头部的空格
* 参数     : char *str
* 返回值   : 无
* 修改历史 : 
***************************************************************/
void ltrim( char *str )
{
	ltrimchr( str, 0x20 );
}

/***************************************************************
* 函数名称 : rtrim
* 作者     : rdc-zhangz  
* 描述     : 删除字符串尾部的空格
* 参数     : char *str
* 返回值   : 无
* 修改历史 : 
***************************************************************/
void rtrim( char *str )
{
	rtrimchr( str, 0x20 );
}

/***************************************************************
* 函数名称 : alltrim
* 作者     : rdc-zhangz  
* 描述     : 删除字符串头部和尾部的空格
* 参数     : char *str
* 返回值   : 无
* 修改历史 : 
***************************************************************/
void alltrim( char *str )
{
    ltrimchr( str, 0x20 );
    rtrimchr( str, 0x20 );
    return;
}

/***************************************************************
* 函数名称 : lfill
* 作者     : rdc-zhangz  
* 描述     : 在字符串头部填满指定字符
* 参数     : char *sdest  - 被填充的字符串
*            int nsize    - 字符串的空间大小(填充后长度=nsize-1)
*            char cflag   - 填充的字符(不能为0x00)
* 返回值   : 0   - 成功
*            200 - 参数错误
* 修改历史 : 
***************************************************************/
int lfill( char * sdest, int nsize, char cflag )
{
    return lfillex( sdest, nsize, sdest, nsize-1, cflag );
}

/***************************************************************
* 函数名称 : rfill
* 作者     : rdc-zhangz  
* 描述     : 在字符串尾部填满指定字符
* 参数     : char *sdest  - 被填充的字符串
*            int nsize    - 字符串的空间大小(填充后长度=nsize-1)
*            char cflag   - 填充的字符(不能为0x00)
* 返回值   : 0   - 成功
*            200 - 参数错误
* 修改历史 : 
***************************************************************/
int rfill( char * sdest, int nsize, char cflag )
{
    return rfillex( sdest, nsize, sdest, nsize-1, cflag );
}

/***************************************************************
* 函数名称 : lfillex
* 作者     : rdc-zhangz  
* 描述     : 在字符串头部填满指定字符
* 参数     : char *sdest  - 填充后的串
*            int nsize    - 字符串的空间大小
*            char *ssrc   - 源串
*            int ncnt     - 填充后的字符串的总长度(要小于nsize)
*            char cflag   - 填充的字符(不能为0x00)
* 返回值   : 0   - 成功
*            100 - 目标串空间不足或源串长度大于ncnt
*            200 - 参数错误
* 修改历史 : 
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
* 函数名称 : rfillex
* 作者     : rdc-zhangz  
* 描述     : 在字符串尾部填满指定字符
* 参数     : char *sdest  - 填充后的串
*            int nsize    - 字符串的空间大小
*            char *ssrc   - 源串
*            int ncnt     - 填充后的字符串的总长度(要小于nsize)
*            char cflag   - 填充的字符(不能为0x00)
* 返回值   : 0   - 成功
*            100 - 目标串空间不足或源串长度大于ncnt
*            200 - 参数错误
* 修改历史 : 
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
* 函数名称 : getsubstr
* 作者     : rdc-zhangz  
* 描述     : 根据分隔符和序号从指定字符串中取相应子串(源串不变)
* 参数     : char *sdest  - 取到的子串
*            int nsize    - 子串的空间大小
*            char *ssrc   - 源串
*            int ncnt     - 子串序号(从0开始)
*            char cflag   - 分隔符(不能为0x00)
* 返回值   : 0   - 成功
*            1   - 子串不存在或源串为空
*            100 - 子串空间不足
*            200 - 参数错误
* 修改历史 : 
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
* 函数名称 : _cutsubstr
* 作者     : rdc-zhangz  
* 描述     : 根据分隔符从指定字符串中逐个截取子串(源串不会改变，只是源串指针会移动)
* 参数     : char *sdest  - 截取到的子串
*            int nsize    - 子串的空间大小
*            char *ssrc   - 源串
*            char cflag   - 分隔符(不能为0x00)
* 返回值   : 0   - 成功
*            1   - 源串为空(sdest被置空)
*            100 - 子串空间不足
*            200 - 参数错误
* 修改历史 : 
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
* 函数名称 : cutsubstr
* 作者     : rdc-zhangz  
* 描述     : 根据分隔符从指定字符串中逐个截取子串(源串被改变)
* 参数     : char *sdest  - 截取到的子串
*            int nsize    - 子串的空间大小
*            char *ssrc   - 源串
*            char cflag   - 分隔符(不能为0x00)
* 返回值   : 0   - 成功
*            1   - 源串为空(sdest被置空)
*            100 - 子串空间不足
*            200 - 参数错误
* 修改历史 : 
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
* 函数名称 : lstrncat
* 作者     : chensz
* 描述     : 在字符串头部追加指定字符串的前n个字符
* 参数     : char *sdest  - 目标串
*            int nsize    - 目标串的空间大小
*            char *ssrc   - 源串
*            int ncnt     - 取源串的字符个数
* 返回值   :  0 - 成功
*            -1 - 目标串空间不足
*            -2 - 参数错误
* 修改历史 : 
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
    
    if( nsrclen > ncnt ) //只取前ncnt个字符
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
* 函数名称 : lstrcat
* 作者     : chensz
* 描述     : 在字符串头部追加指定字符串
* 参数     : char *sdest  - 目标串
*            int nsize    - 目标串的空间大小
*            char *ssrc   - 源串
* 返回值   :  0 - 成功
*            -1 - 目标串空间不足
*            -2 - 参数错误
* 修改历史 : 
***************************************************************/
int lstrcat( char * sdest, int nsize, char * ssrc )
{
    return lstrncat(sdest, nsize, ssrc, strlen(ssrc));
}

/***************************************************************
* 函数名称 : rstrncat
* 作者     : chensz
* 描述     : 在字符串尾部追加指定字符串的前n个字符
*            比strncat安全，不容易越界
* 参数     : char *sdest  - 目标串
*            int nsize    - 目标串的空间大小
*            char *ssrc   - 源串
*            int ncnt     - 取源串的字符个数
* 返回值   :  0 - 成功
*            -1 - 目标串空间不足
*            -2 - 参数错误
* 修改历史 : 
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
    
    if( nsrclen > ncnt ) //只取前ncnt个字符
        nsrclen = ncnt;
    
    if( nsize <= nsrclen + ndestlen )
        return -1;
    
    for (i=ndestlen; i<nsrclen+ndestlen; i++)
        *(sdest+i) = *ssrc++;
    *(sdest+nsrclen+ndestlen) = 0x00;
    return 0;
}

/***************************************************************
* 函数名称 : rstrcat
* 作者     : chensz
* 描述     : 在字符串尾部追加指定字符串
*            比strcat安全，不容易越界
* 参数     : char *sdest  - 目标串
*            int nsize    - 目标串的空间大小
*            char *ssrc   - 源串
* 返回值   :  0 - 成功
*            -1 - 目标串空间不足
*            -2 - 参数错误
* 修改历史 : 
***************************************************************/
int rstrcat( char * sdest, int nsize, char * ssrc )
{
    return rstrncat(sdest, nsize, ssrc, strlen(ssrc));
}

/***************************************************************
* 函数名称 : strchrand
* 作者     : rdc-zhangz
* 描述     : 字符串与操作，提取2个字符串都包含的子串
*            ssrc1 = "1234555"
*            ssrc2 = "257"
*            结果为 "2555"
* 参数     : char *sdest  - 目标串
*            int nsize    - 目标串的空间大小
*            char *ssrc1  - 源串1
*            char *ssrc2  - 源串2
* 返回值   : 0 - 成功
*           -1 - 目标串空间不足
*           -2 - 参数错误
* 修改历史 : 
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
* 函数名称 : strand
* 作者     : liuyi
* 描述     : 字符串与操作，提取2个字符串都包含的子串
*            ssrc1 = "001,00002,005,0004"
*            ssrc2 = "002,005,001"
*            结果为 "001,005" 
* 参数     : char *sdest  - 目标串
*            int nsize    - 目标串的空间大小
*            char *ssrc1  - 源串1
*            char *ssrc2  - 源串2
*            char cflag   - 子串分隔符
* 返回值   : 0 - 成功
*           -1 - 目标串空间不足
*           -2 - 参数错误
* 修改历史 : 
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
* 函数名称 : stror
* 作者     : liuyi
* 描述     : 字符串或操作，合并2个字符串，
*            使目标串包含2个字符串中全部子串
*            ssrc1 = "001,00002,005,0004"
*            ssrc2 = "002,005,001"
*            结果为 "001,002,00002,005,0004" 
* 参数     : char *sdest  - 目标串
*            int nsize    - 目标串的空间大小
*            char *ssrc1  - 源串1
*            char *ssrc2  - 源串2
*            char cflag   - 子串分隔符
* 返回值   : 0 - 成功
*           -1 - 目标串空间不足
*           -2 - 参数错误
* 修改历史 : 
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
* 函数名称 : strxor
* 作者     : liuyi
* 描述     : 字符串异或操作，取出2个字符串中不同的子串
*            ssrc1 = "001,00002,005,0004"
*            ssrc2 = "002,005,001"
*            结果为 "002,00002,0004" 
* 参数     : char *sdest  - 目标串
*            int nsize    - 目标串的空间大小
*            char *ssrc1  - 源串1
*            char *ssrc2  - 源串2
*            char cflag   - 子串分隔符
* 返回值   : 0 - 成功
*           -1 - 目标串空间不足
*           -2 - 参数错误
* 修改历史 : 
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
* 函数名称 : strnotin
* 作者     : liuyi
* 描述     : 取出字符串2不在字符串1中的子串
*            ssrc1 = "001,00002,005,0004"
*            ssrc2 = "002,005,001"
*            结果为 "002" 
* 参数     : char *sdest  - 目标串
*            int nsize    - 目标串的空间大小
*            char *ssrc1  - 源串1
*            char *ssrc2  - 源串2
*            char cflag   - 子串分隔符
* 返回值   : 0 - 成功
*           -1 - 目标串空间不足
*           -2 - 参数错误
* 修改历史 : 
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
* 函数名称 : getfileinfo
* 作者     : rdc-zhangz
* 描述     : 取出文件的信息：文件大小，创建日期，创建时间，修改日期
修改时间
*            psfilename = "c:\text.dat"
nfilesize = 789
*            scdate = "1999-12-01"
*            sctime = "13:03:12"
smdate = "2000-03-09"
smtime = "09:03:45"
* 参数     : const char *psfilename  - 文件名称（含路径）
*            int nfilesize    - 文件大小
*            char *scdate  - 创建日期
*            int ncdtsize  - 创建日期目的串的空间大小
*            char *sctime  - 创建时间
*            int nctmsize  - 创建时间目的串的空间大小
*            char *smdate  - 修改日期
*            int nmdtsize  - 修改日期目的串的空间大小
*            char *smtime  - 修改时间
*            int nmtmsize  - 修改时间目的串的空间大小
* 返回值   : 0 - 成功
*           -1 - 文件不存在
*           -2 - 目的串空间不够
*           -3 - 参数错误
* 修改历史 : 
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
* 函数名称 : getsystime
* 作者     : rdc-zhangz
* 描述     : 取出系统日期，系统时间
*            ndate = 20031216
*            ntime = 13451612
* 参数     : int ndate  - 系统日期(整型)
*            int ntime  - 系统时间(整型，精确到百分之一秒)
* 返回值   : void
* 修改历史 : 
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
* 函数名称 : strrpl
* 作者     : rdc-zhangz
* 描述     : 字符串替换
*          ssrc = "abcdef123ijklmn"
*          ssub = "123"
*          srpl = "*******"
*          sdest = "abcdef******ijklmn"
* 参数     : char * sdest  - 目的串
*           int nsize  - 目的串空间的大小
*           char * ssrc - 源串
*           char * ssub - 源串的子串
*           char * srpl - 替换串
* 返回值   : >=0 - 成功替换的子串个数
*           -1 - 参数错误
*           -2 - 目的串空间不够
* 修改历史 : 
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
* 函数名称 : substr
* 作者     : rdc-zhangz
* 描述     : 取得字符串子串
*          ssrc = "abcdef123ijklmn"
*          noffset = 6
*          ncnt = 3
*          sdest = "123"
*          ssrc = "a123"
*          noffset = 1
*          ncnt = 5
*          sdest = "123"
* 参数     : char * sdest  - 目的串
*           unsigned int nsize  - 目的串空间的大小
*           char * ssrc - 源串
*           unsigned int noffset - 从源串中取字符的位置偏移量
*           unsigned int ncnt - 取得字符的个数
* 返回值   : 0 - 成功
*           -1 - 参数错误
*           -2 - 目的串空间不够
* 修改历史 : 
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
* 函数名称 : strencrypt0
* 作者     : rdc-zhangz
* 描述     : 字符串简单加密, 只支持最大10位加密因子和源串
* 参数     : const unsigned char* szsrc  - 源串，要加密的字符串
*           const unsigned char* szkey - KEY串，加密因子串
*           unsigned char * szresult - 生成的加密串，固定长度为20个字节
* 返回值   :
* 修改历史 : 
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
* 函数名称 : strdecrypt0
* 作者     : rdc-zhangz
* 描述     : 字符串解密，对应strencrypt0
* 参数     : const char* szsrc  - 源串，要解密的字符串
*           const char* szkey - KEY串，解密因子串
*           char * szresult - 生成的解密串
* 返回值   :
* 修改历史 : 
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
* 函数名称 : isdigitalex
* 作者     : rdc-zhangz
* 描述     : 判断字符串中是否全是数字，正负(-+)，小数点(.)
* 参数     : const char* szsrc  - 源串，要判断的字符串
* 返回值   :
* 修改历史 : 
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
* 函数名称 : isdigital
* 作者     : rdc-zhangz
* 描述     : 判断字符串中是否全是数字
* 参数     : const char* szsrc  - 源串，要判断的字符串
* 返回值   :
* 修改历史 : 
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
* 函数名称 : isengchr
* 作者     : rdc-zhangz
* 描述     : 判断字符串中是否全是英文字母
* 参数     : const char* szsrc  - 源串，要判断的字符串
* 返回值   :
* 修改历史 : 
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
* 函数名称 : isengchr
* 作者     : rdc-zhangz
* 描述     : 判断字符串中是否全是英文字母和数字
* 参数     : const char* szsrc  - 源串，要判断的字符串
* 返回值   :
* 修改历史 : 
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
* 函数名称 : chrisnull
* 作者     : rdc-zhangz
* 描述     : 判断字符是否为0
* 参数     : const char chrsrc  - 源字符
* 返回值   : char 如果是0, 返回0x20，否则返回自身
* 修改历史 : 
***************************************************************/
char chrisnull( char chrsrc )
{
	return (chrsrc==0?0x20:chrsrc);
}

/***************************************************************
* 函数名称 : datediff
* 描述     : 求两个日期的差值（天数）
* 参数     : unsigned int date1 被减日期，格式如：20040301，必须>19700101
             unsigned int date2 减的日期，格式如：20040229，必须>19700101
             int &ndaydiff      相差的天数
             例如:date1(20040228) - date2(20040302) = nday(-3)
* 返回值   : int 0 正确， 非0 错误，日期格式不对
* 修改历史 : 
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
* 函数名称 : datediff
* 描述     : 求两个日期的差值
* 参数     : char cDatepart 指明日期的哪一部分，'Y'年, 'M'月, 'D'日
			 unsigned int date1 被减日期，格式如：20040301，必须>19700101
             unsigned int date2 减的日期，格式如：20040229，必须>19700101
             int &ndaydiff      相差的数字
             例如:date1(20040228) - date2(20040302) = nday(-3)
* 返回值   : int 0 正确， 非0 错误，日期格式不对
* 修改历史 : 
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
* 函数名称 : dateplus
* 作者     : rdc-zhangz
* 描述     : 求一个日期加上一定天数的和值（日期）
* 参数     : unsigned int date1 被加日期，  格式如：20040301，必须>19700101
             int nday           加上的天数
             unsigned int date2 得到的日期  格式如：20040301，必须>19700101
             例如:date1(20040228) + nday(3) = date2(20040302)
* 返回值   : int 0 正确， 非0 错误，日期格式不对
* 修改历史 : 
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

	//00:00:00 相差一天 so +1
    tm0 = tm1 + nday*24*60*60 + 24*60*60;
    
    psttm2 = gmtime(&tm0);
    if( psttm2 == NULL )
        return 5;

    date2 = (psttm2->tm_year+1900)*10000 + (psttm2->tm_mon+1)*100 + psttm2->tm_mday;    
    return 0;
}

/***************************************************************
* 函数名称 : dateplus
* 作者     : yangjianli
* 描述     : 求一个日期加上一定天数的和值（日期）
* 参数     : unsigned int date1 被加日期，  格式如：20040301，必须>19700101
			 char cDatepart		指明日期的哪一部分，'Y'年, 'M'月, 'D'日
             int nNum           增加的数值
             unsigned int date2 得到的日期  格式如：20040301，必须>19700101
             例如:date1(20040228) + nday(3) = date2(20040302)
* 返回值   : int 0 正确， 非0 错误，日期格式不对
* 修改历史 : 
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

		//00:00:00 相差一天 so +1
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
* 函数名称 : strdistinct
* 作者     : rdc-zhangz
* 描述     : 删除字符串中重复的字符
* 参数     : char * psrc 操作字符串
             例如:strdistinct( "112233" ) = "123";
* 返回值   : no
* 修改历史 : 
***************************************************************/
void strdistinct(char * psrc)
{
	char cchr;
	int nsrclen, i, j;
	
	if( psrc == NULL || *psrc == 0x00 )
		return;
	//先记录原字符串长度，再将一个字符后面与它相同的字符替换成0x00('\0')
	nsrclen = strlen(psrc);
	for( i = 0; i < nsrclen; i++ )
	{
		if( (cchr = psrc[i]) == 0x00 ) //已经被替换
			continue;
		for( j = i+1; j < nsrclen; j++ )
		{
			if( cchr == psrc[j] )
			{
				psrc[j] = 0x00;       //找到重复的，替换它
			}
		}
	}

	//移动字符串，将字符串中的'\0'去掉
	int num = 0; //记录有多少个'\0'
	for( i = 0; i < nsrclen; i++ )
	{
		if( psrc[i] == 0x00 ) //已经被替换
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
* 函数名称 : strdelchr
* 作者     : rdc-zhangz
* 描述     : 删除字符串中某个字符
* 参数     : char * src 操作字符串
             例如:strdelchr( "11,22,33",',' ) = "112233";
* 返回值   : no
* 修改历史 : 
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
* 函数名称 : strdelstrchr
* 作者     : lizb
* 描述     : 在字符串1中 删除 字符串2包含的字符
* 参数     : char * operstr 操作字符串
             char * parstr  参照字符串

             例如: strdelstrchr( "ABCDEF" , 'AE' ) = "BCDF";
				   strdelstrchr( "ABCDEF" , 'ACE' ) = "BDF";

* 返回值   : no
* 修改历史 : 
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
* 函数名称 : b2t
* 作者     : rdc-zhangz
* 描述     : 二进制转换成文本
             原理将一个字符的ASCII分成两个十六进制的字符表示
			 'A'(ASCII: 65(10), 0x41 ) --> '41'
* 参数     : void *pvd    -- 源二进制串
             int   nlen   -- 源二进制串的长度
			 char *pszbuf -- 目的文本缓冲区
			 int   nsize  -- 目的文本缓冲区大小，应该大于等于2*len+1
* 返回值   : 0 成功 非 0 失败
             -1 -- 缓冲区太小
* 注意     : nsize的大小应该含有0x00字符串结尾符
* 修改历史 : 
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
		chr1 =  (*(ps+i))>>4;     //分离高四位的值
		chr1 += chr1>9?0x41:0x30; 
		chr2 =  (*(ps+i))&0x0F;   //分离低四位的值
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
* 函数名称 : t2b
* 作者     : rdc-zhangz
* 描述     : 文本转换成二进制，
* 参数     : char *pstr  -- 源文本串
             void *pvd   -- 目的二进制缓冲区
			 int   nsize -- 目的二进制缓冲区大小
* 返回值   : 0 成功 非 0 失败
             -1 -- 缓冲区太小
			 -2 -- 源文本串格式不对
* 修改历史 : 
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
		chr1 -= chr1>0x39?0x41:0x30;  //还原高四位
		pstr++;
		chr2 =  *pstr;
		chr2 -= chr2>0x39?0x41:0x30;  //还原低四位
		pstr++;
		if( chr1 > 0x0F || chr2 > 0x0F )
			return -2;
		*ps = (chr1<<4)|chr2;         //合成原字符
		ps++;
	}
	return 0;
}

//Base64是MIME邮件中常用的编码方式之一。它的主要思想是将输入的字符串或数据编码成只含有{'A'-'Z', 'a'-'z', '0'-'9', '+', '/'}这64个可打印字符的串，故称为“Base64”。 
//Base64编码的方法是，将输入数据流每次取6 bit，用此6 bit的值(0-63)作为索引去查表，输出相应字符。这样，每3个字节将编码为4个字符(3×8 → 4×6)；不满4个字符的以'='填充。 
const char EnBase64Tab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
 
int EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen)
{
    unsigned char c1, c2, c3;    // 输入缓冲区读出3个字节
    int nDstLen = 0;             // 输出的字符计数
    int nDiv = nSrcLen / 3;      // 输入数据长度除以3得到的倍数
    int nMod = nSrcLen % 3;      // 输入数据长度除以3得到的余数
 
    // 每次取3个字节，编码成4个字符
    for( int i = 0; i < nDiv; i++ )
    {
        // 取3个字节
        c1 = *pSrc++;
        c2 = *pSrc++;
        c3 = *pSrc++;
 
        // 编码成4个字符
        *pDst++ = EnBase64Tab[c1 >> 2];
        *pDst++ = EnBase64Tab[((c1 << 4) | (c2 >> 4)) & 0x3f];
        *pDst++ = EnBase64Tab[((c2 << 2) | (c3 >> 6)) & 0x3f];
        *pDst++ = EnBase64Tab[c3 & 0x3f];
        nDstLen += 4; 
    }
 
    // 编码余下的字节
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
 
    // 输出加个结束符
    *pDst = '\0';
 
    return nDstLen;
}

//Base64解码方法中，最简单的也是查表法：将64个可打印字符的值作为索引，
//查表得到的值（范围为0-63）依次连起来，拼凑成字节形式输出，就得到解码结果。 
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
    int nDstLen;            // 输出的字符计数
    int nValue;             // 解码用到的长整数
    int i;
 
    i = 0;
    nDstLen = 0;
 
    // 取4个字符，解码到一个长整数，再经过移位得到3个字节
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
 
    // 输出加个结束符
    *pDst = '\0';
 
    return nDstLen;
}
int strConvert(char * psrc)
{
	int nsrclen, i;
	
	if( psrc == NULL || *psrc == 0x00 )
		return -1;
	//先记录原字符串长度，再将一个字符后面与它相同的字符替换成0x00('\0')
	nsrclen = strlen(psrc);
	for( i = 0; i < nsrclen; i++ )
	{
		if( '0' <= psrc[i] && psrc[i] <= '9' ) //已经被替换
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
* 函数名称 : charreplchar
* 作者     : luyp  
* 描述     : 替换字符串中的指定字符(源串被改变)
* 参数     : char *sSrc   - 源字符
*            char cTarget - 指定字符
*            char cRepl   - 替换字符
* 返回值   : 0    - 成功
*            -1   - 指定或替换字符为0x00
*            -2   - 源串不能为空
* 修改历史 : 
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
* 函数名称 : cutsubbetweenflagstr
* 作者     : luyp  
* 描述     : 根据分隔符从指定字符串中截取两个分隔符中的子串(源串被改变)
* 参数     : char *sdest  - 截取到的子串
*            int nsize    - 子串的空间大小
*            char *ssrc   - 源串
*            char cflag   - 分隔符(不能为0x00)
* 返回值   : 0   - 成功
*            1   - 源串为空(sdest被置空)
*            100 - 子串分隔符不足
*            200 - 参数错误
*			 300 - 子串空间不足
* 修改历史 : 
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
*	Desc:		 十五位身份证和十八位身份证之间的转化，
*				 如果输入十八，转换成15，如果15转换成
*				 18位,但十五位转换十八局现年龄在1900到
*				 1999年之间。
*	Input Params:
	char *sTostr:		返回值
	const char *sSstr:	送入源串
	int nLen:			送入串长度,只能为15或18
	return Value:
	int:	0~成功; -1~输入串长度和输入串的长度不一致;
			-2~送入串长度，不是15或18;-3~串中有非法字符
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
* 函数名称 : strright
* 作者     : 
* 描述     : 取右N位字符串，放入目标串；
			 如果字符串不够N位，那么左边填充'cflag'
* 参数     : char *sdest	目标字符串串
			 int  nsize		目标字符串长度
			 char *ssrc		源字符串
			 int  ncnt      右边N位
			 char cflag     不够填充字符
* 返回值   : 无
* 修改历史 : 
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
* 函数名称 : getchrnum
* 作者     : rdc-zhangz
* 描述     : 字符串替换
* 参数     : 
*            char * psrc    - 源串
*            char   cflag   - 查找的字符
* 返回值   : int 包含字符的个数
* 修改历史 : 
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
* 函数名称 : charindextoin
* 作者     : rdc-zhangcm
* 描述     : 将chindex转换为in
* 参数     : 
*            char * ssrc    - 源串
*            char   sdest   - 目的串
*            char   ssprt   - 分割符号
* 返回值   : int 目的串
* 修改历史 : 
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
* 函数名称 : dayOfWeek
* 作者     : rdc-zhangcm
* 描述     : 将日期转换为周
* 参数     : 
*            int  ndate    - 日期
*            
*            
* 返回值   : int 星期
* 修改历史 : 
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
	//合法性判断
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
* 函数名称 : dateplus
* 作者     : rdc-cdm
* 描述     : 求一个日期减去上一定天数的差值（日期）
* 参数     : unsigned int date1 初始日期，  格式如：20040302，必须>19700101
             int nday           减去的天数
             unsigned int date2 得到的日期  格式如：20040228，必须>19700101
             例如:date1(20040302) - nday(3) = date2(20040228)
* 返回值   : int 0 正确， 非0 错误，日期格式不对
* 修改历史 : 
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

	//00:00:00 相差一天 so +1
    tm0 = tm1 - nday*24*60*60 + 24*60*60;
    
    psttm2 = gmtime(&tm0);
    if( psttm2 == NULL )
        return 5;

    date2 = (psttm2->tm_year+1900)*10000 + (psttm2->tm_mon+1)*100 + psttm2->tm_mday;    
    return 0;
}
