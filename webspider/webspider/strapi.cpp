#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strapi.h"


/***************************************************************
* 函数名称 : rtrimchr
* 作者     : zhang
* 描述     : 删除字符串尾部的某字符的串
* 参数     : char *str  要操作的串
             char cflag 要删除的字符
* 返回值   : 无
* 修改历史 : 
***************************************************************/
char * rtrimchr( char *str, char cflag )
{
	int i;
	int len;

	len = (int)strlen(str) - 1;

	for( i = len; i >= 0 && str[i] == cflag; i-- )
		str[i] = 0;

	return str;
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
char * ltrimchr( char *str, char cflag )
{
	int i ;
	int len;
	
	len = (int)strlen( str );

	for( i = 0 ; i < len && str[i] == cflag; i++ );
	if( i > 0 )
		strcpy( str , str + i );
	
	return str;
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
* 返回值   : 结束为止
* 修改历史 : 
***************************************************************/
const char * getsubstr( char * sdest, int nsize, const char * ssrc, char cflag)
{
	const char *ps = ssrc;
	const char * pos = strchr(ssrc, cflag);
	if(pos == 0)
		return 0;

	*sdest = 0;
	if(pos - ssrc >= nsize)
		return 0;

	while(ps != pos)
	   *sdest++ = *ps++;

	*sdest = 0;
	return pos+1;
}

