#include <string.h>

void * mkargv(void * args, const void * ptr, int size)
{
	memcpy(args, ptr, size);
	return (char *)args + size;
}

void * getarg(void * ptr, const void * args, int size)
{
	memcpy(ptr, args, size);
	return (char *)args+size;
}

int getline(char *buf, int bufsize, /*std_bufot cbot,*/ void * parm)
{
	char c = '\0';
	int count = 0;

	while(c != '\n')
	{
		//if(cbot(&c, 1, parm) < 1)
		//	return count;

		*(buf+count) = c;

		if(count == bufsize)
			return count;

		count++;
	}

	return count-1;
}