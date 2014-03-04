#include <string.h>
#include "webspider.h"

unsigned long webspider_setflag(unsigned long flag, char * str)
{
	if(!strcmp(str, "img"))
		flag |= FLAG_IMG;
	else if(!strcmp(str, "/img"))
		flag &= ~FLAG_IMG;
	else if(!strcmp(str, "a"))
		flag &= FLAG_A;
	else if(!strcmp(str, "/a"))
		flag &= ~FLAG_A;

	return flag;

}