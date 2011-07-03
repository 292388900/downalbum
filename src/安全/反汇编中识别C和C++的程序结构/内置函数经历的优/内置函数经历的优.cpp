#include	<stdio.h>

__inline	int max( int a,int b )
{
	if(a>b)
		return a;
	return b;
}

void	main(int argc,char **argv)
{
	printf("%x\n",max(0x666,0x777));
	printf("%x\n",max(0x666,argc));
	printf("%x\n",max(0x666,argc));
}