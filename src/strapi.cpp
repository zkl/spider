#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strapi.h"


/***************************************************************
* �������� : rtrimchr
* ����     : zhang
* ����     : ɾ���ַ���β����ĳ�ַ��Ĵ�
* ����     : char *str  Ҫ�����Ĵ�
             char cflag Ҫɾ�����ַ�
* ����ֵ   : ��
* �޸���ʷ : 
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
* �������� : ltrimchr
* ����     : rdc-zhangz  
* ����     : ɾ���ַ���ͷ����ĳ�ַ��Ĵ�
* ����     : char *str  Ҫ�����Ĵ�
             char cflag Ҫɾ�����ַ�
* ����ֵ   : ��
* �޸���ʷ : 
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
* �������� : getsubstr
* ����     : rdc-zhangz  
* ����     : ���ݷָ�������Ŵ�ָ���ַ�����ȡ��Ӧ�Ӵ�(Դ������)
* ����     : char *sdest  - ȡ�����Ӵ�
*            int nsize    - �Ӵ��Ŀռ��С
*            char *ssrc   - Դ��
*            int ncnt     - �Ӵ����(��0��ʼ)
*            char cflag   - �ָ���(����Ϊ0x00)
* ����ֵ   : ����Ϊֹ
* �޸���ʷ : 
***************************************************************/
const char * getsubstr( char * sdest, unsigned int nsize, const char * ssrc, char cflag)
{
	const char * ps = ssrc;
	const char * pos = strchr(ssrc, cflag);
	if(pos == 0)
	{
		if(strlen(ssrc) < nsize)
			strcpy(sdest, ssrc);

		return ssrc+strlen(ssrc);
	}

	*sdest = 0;
	if((unsigned int)(pos - ssrc) >= nsize)
		return 0;

	while(ps != pos)
	   *sdest++ = *ps++;

	*sdest = 0;
	return pos+1;
}

