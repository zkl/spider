#include <stdio.h>
#include "base64.h"

char basecode( int c);
char unbasecode( char c);

void encode( char *ostr, char *istr)
{
    unsigned int i = 0, j = 0;
    unsigned char code[3];
    char *p = istr;
    while(*p)
    {
        for( i = 0; i < 3; i++)
        {
            code[i] = *p;
            if(*p != '\0')p++;
        }
        ostr[j] = code[0] >> 2;
        ostr[j+1] = code[0] << 4;
        ostr[j+1] = ostr[j+1] & 0x30;
        ostr[j+1] = ostr[j+1] | (code[1] >> 4); 
        ostr[j+2] = code[1] << 2;
        ostr[j+2] = ostr[j+2] & 0x3c;
        ostr[j+2] = ostr[j+2] | code[2] >> 6;
        ostr[j+3] = code[2] & 0x3f;
        j += 4;
    }
    for( i = 0; i < j; i++)
    {
        ostr[i] = basecode(ostr[i]);
    }
    if(code[2] == '\0')ostr[i - 1] = '=';
    if(code[1] == '\0')ostr[i - 2] = '=';
    ostr[i] = '\0';
}

void decode( char *ostr, char *istr)
{
    unsigned int i = 0, j = 0;
    unsigned char code[4];
    char *p = istr;
    while(*p)
    {
        for( i = 0; i < 4; i++) //
        {
            if(*p != '=')code[i] = unbasecode(*p);
            else code[i] = 0;   
            if(*p != '\0')p++;
        }
        ostr[j] = code[0] << 2;
        ostr[j] = ostr[j] | (code[1] >> 4);
        ostr[j+1] = code[1] << 4;
        ostr[j+1] = ostr[j+1] | (code[2] >> 2);
        ostr[j+2] = code[2] << 6;
        ostr[j+2] = ostr[j+2] | code[3];
        j += 3;
    } 
    ostr[j] = '\0';
}

char basecode( int c)
{
    char code[64] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                     'I', 'J', 'K', 'L' ,'M', 'N', 'O', 'P', 
                     'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 
                     'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 
                     'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 
                     'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 
                     'w', 'x', 'y', 'z', '0', '1', '2', '3', 
                     '4', '5', '6', '7', '8', '9', '+', '/'};
	if(c < 64 && c >= 0)return code[c];
	else return 0;
}
char unbasecode( char c)
{
    if(c >= 'A' && c <='Z') return c - 'A';
    else if( c >= 'a' && c <= 'z') return c - 'a' + 26;
    else if( c >= '0' && c <= '9') return c - '0' + 52;
    else if( c == '+')return 62;
    else if( c== '/' )return 63;
    else return 0;
}

