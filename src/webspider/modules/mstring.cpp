#include <string.h>
#include <stdio.h>
#include <Windows.h>
#include <malloc.h>
#include "mstring.h"


MSTRING mstring_new(char * str, int size)
{
	MSTRING mstr = {0, 0, 0};

	if(size == 0)
		return mstr;

	size = size > 0 ? size:strlen(str);
	mstr.str = (char *)malloc(size+1);
	strncpy(mstr.str, str, size);
	*(mstr.str+size) = 0;

	mstr.size = size;
	mstr.maxsize = size+1;

	return mstr;
}

void mstring_del(MSTRING mstr)
{
	free(mstr.str);
}


/* 字符串转换为大写函数 */
void strtoupper(char *ostr, const char *istr)
{
	const char *ip = istr;
	char *op = ostr;
	while(*ip)
	{
		if(*ip <= 'z' && *ip >= 'a')
			*op++ = *ip++ - 32;
		else
			*op++ = *ip++;
	}
	*op = 0;
}

/* 字符串转换为小写函数 */
void strtolower(char *ostr, const char *istr)
{
	const char *ip = istr;
	char *op = ostr;
	while(*ip)
	{
		if(*ip <= 'Z' && *ip >= 'A')
			*op++ = *ip++ + 32;
		else
			*op++ = *ip++;
	}
	*op = 0;
}

/* 将字符串istr复制给ostr，直到碰到c停止 */
char * strcpy_chr(char *ostr, int size, const char *istr, char c)
{
	char *p = ostr;

	while(*istr && *istr != c && p-ostr < size-1)
		*p++ = *istr++;
	
	*p = 0;
	return ostr;
}

/* 复制len个字符串到dest，不会考虑str的长度，但是会在dest最后加'\0'，所以dest的最小长度为len+1*/
char * strncpy_l2(char *dest, const char *str, int len)
{
	char *bp = dest;
	
	for(;len > 0;len--)
		*bp++ = *str++;

	*bp = 0;
	return dest;
}

char * malloc_str(const char * buf, int size)
{
	if(size == 0)
		return 0;
	char * dest = 0;
	size = size > 0 ? size:strlen(buf);
	dest = (char *)malloc(size+1);
	strncpy(dest, buf, size);
	*(dest+size) = 0;

	return dest;
}

void debug(const char *msg, ...)
{
	char buf[2049];
	va_list ap;

	va_start(ap, msg);
	vsprintf_s(buf, 2048, msg, ap);
	va_end(ap);
	
	OutputDebugString(buf);
	OutputDebugString("\n");
}

/* 获取字符串字段 v1.1 */
char * strgetfiled(char * dest , int size, char const ** endstr, const char * sou, const char *splitchars)
{
	int signal = 0;
	int length= 0;
	char *p = dest;

	if(sou == NULL || *sou == 0)
		return 0;

	while(*sou) // 跳过开始的分隔符
	{
		if(strchr(splitchars, *sou) == NULL)
			break;
		sou++;
	}

	while(*sou && length <= size)
	{
		if(signal == 0)
		{
			if(strchr(splitchars, *sou))
				break;
		}

		if(*sou == '"')
			signal = signal ? 0:1;

		*p++ = *sou++;
		length++;
	}
	if(endstr)
		*endstr = sou;
	*p = 0;

	if(p == dest)
		return 0;

	return dest;
}

/* 将sou中的sstr替换为dstr并存储在dest中 */
char *strreplace(char * dest, int size, const char * sou, const char *sstr, const char *dstr)
{
	int count = 0;

	char  * curset = dest;
	const int dstrlen = strlen(dstr);
	const int sstrlen = strlen(sstr);
	const char * next = sou;
	const char * first= sou;

	while(1)
	{
		next = strstr(first, sstr);
		if(next)
		{
			int len = next-first;

			if(len > 0)
			{
				if(curset+len - dest >= size)
					return 0;
				strncpy(curset, first, len);
			}

			curset += len;
			first   = next+sstrlen;

			if(dstrlen > 0)
			{
				if(curset+len - dest >= size)
					return 0;
				strncpy(curset, dstr, dstrlen);
			}
			curset += dstrlen;
		}
		else
		{
			if(curset+strlen(first)-dest >= size)
				return 0;

			strcpy(curset, first);
			break;
		}
	}
	return dest;
}

/**************************************************************
** 版本:              v 1.0                                   
** 功能: 删除字符串src开始和结束的字符集合chrs
** 返回: dest
** 备注: 
**                                                            
***************************************************************/
char * strdelentry(char *dest, const char *src, char *chrs)
{
	char *bp = dest;
	const char *ptr = src;
	const char *edp = src + strlen(src) - 1;
	while(*ptr && strchr(chrs, *ptr)) /* 跳过开始的字符 */
		ptr++;

	while(edp != src && strchr(chrs, *edp))/* 跳过结束的字符 */
		edp--;

	while(*ptr && ptr != edp+1)/* 复制 */
		*dest++ = *ptr++;

	*dest = 0;

	return bp;
}

/* 删除sou开始的字符c */
char *strheadremovechr(char *dest, const char *sou, char c)
{
	char *bp = dest;
	while(*sou && *sou == c)
		sou++;
	while(*sou)
		*dest++ = *sou++;
	*dest = 0;
	return bp;
}

/* 删除sou结尾的的字符c */
char *strlastremovechr(char *dest, const char *sou, char c)
{
	char *bp = dest;
	const char *lst = strlen(sou)+sou-1;
	while(lst != sou && *lst == c)
		lst--;

	while(sou != lst+1)
		*dest++ = *sou++;
	*dest = 0;
	return bp;
}

char * utf8tomult(char * mulstr, int size, const char * utf8str)
{
	const char *endstr = utf8str;
	char * mp = mulstr;
	wchar_t unicode;
	char data[10]  = {0};
	int len = (int)strlen(utf8str);

	while(endstr-utf8str < len)
	{
		if(utf8toucs2(&unicode, &endstr, endstr) == 0)
			break;

		memset(data, 0, 10);
		WideCharToMultiByte(0, 0, &unicode, 1, data, 10, 0, 0);

		if(mp+strlen(data)-mulstr < size)
		{
			strcpy(mp, data);
			mp += strlen(data);
		}
	}
	return mulstr;
}

/**************************************************************
** 版本:              v 1.0                                   
** 功能: 将UTF-8编码转换为UNICODE编码
** 备注: 
**                                                            
***************************************************************/
int utf8toucs2(wchar_t *wchr, const char **endstr, const char *utf8)
{
	char unicode[4];
	unsigned char utf8cde[10];

	memset(unicode, 0, 4);
	memset(utf8cde, 0, 9);
	if((*utf8 & 0x80) == 0)  // 只有一位时
	{
		if(endstr)
			*endstr = utf8+1;

		unicode[3] = *utf8;
	}
	else
	{
		unsigned char st;
		unsigned int count = 0;
		for(st = 0x40; st > 0x00 && (*utf8 & st); st /= 2, count++)
		{}

		if(count < 1 || strlen(utf8) < count+1)
			return 0;

		for(unsigned int i=0; i<count; i++)
		{
			if((utf8[i+1] & 0xC0) != 0x80)
				return 0;
		}

		memccpy(utf8cde, utf8, 1, count+2);

		utf8cde[0] <<= count+1;
		utf8cde[0] >>= count+1; // 清空第一个字节的无效的位

		for(unsigned int i=0; i<count+1; i++)  // 清空其他字节无效的位
			utf8cde[i+1] &= 0x3F;

		unsigned int set = 3;
		unsigned int mvst= 6;
		for(int i=count; i>=0 && (i || set); i--)
		{
			unicode[set] = utf8cde[i] >> (6-mvst);
			if(i > 0)
				unicode[set] |= utf8cde[i-1] << mvst;

			set = set > 0 ? set-1 : 0;
			mvst= mvst> 0 ? mvst-2: 6;
		}

		if(endstr)
			*endstr = utf8 + count + 1;
	}

	/* 附加操作，交换处理 */
	char c;
	c = unicode[0];
	unicode[0] = unicode[1];
	unicode[1] = c;

	c = unicode[2];
	unicode[2] = unicode[3];
	unicode[3] = c;

	*wchr = *((wchar_t*)(unicode+2));
	return 1;
}


/* 逻辑分析 */
/*switch(count)
{
case 1:
	{
		unicode[3] = (utf8cde[1] >> 0) | (utf8cde[0] << 6);
		unicode[2] = (utf8cde[0] >> 2);
		break;
	}
case 2:
	{
		unicode[3] = (utf8cde[2] >> 0) | (utf8cde[1] << 6);
		unicode[2] = (utf8cde[1] >> 2) | (utf8cde[0] << 4);
		unicode[1] = (utf8cde[0] >> 4);
		break;
	}
case 3:
	{
		unicode[3] = (utf8cde[3] >> 0) | (utf8cde[2] << 6);
		unicode[2] = (utf8cde[2] >> 2) | (utf8cde[1] << 4);
		unicode[1] = (utf8cde[1] >> 4) | (utf8cde[0] << 2);
		unicode[0] = (utf8cde[0] >> 6);
		break;
	}
case 4:
	{
		unicode[3] = (utf8cde[4] >> 0) | (utf8cde[3] << 6);
		unicode[2] = (utf8cde[3] >> 2) | (utf8cde[2] << 4);
		unicode[1] = (utf8cde[2] >> 4) | (utf8cde[1] << 2);
		unicode[0] = (utf8cde[1] >> 6) | (utf8cde[0] << 0); // 结束
		unicode[0] = (utf8cde[0] >> 0);                     // 错误
		break;
	}
case 5:
	{
		unicode[3] = (utf8cde[5] >> 0) | (utf8cde[4] << 6);
		unicode[2] = (utf8cde[4] >> 2) | (utf8cde[3] << 4);
		unicode[1] = (utf8cde[3] >> 4) | (utf8cde[2] << 2);
		unicode[0] = (utf8cde[2] >> 6) | (utf8cde[1] << 0);
		unicode[0] = (utf8cde[1] >> 0) | (utf8cde[0] << 6); // 结束
		unicode[0] = (utf8cde[0] >> 2);                     // 错误
		break;
	}
default:
	break;
}*/
