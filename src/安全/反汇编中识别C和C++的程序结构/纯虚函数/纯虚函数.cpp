#include	<stdio.h>

class	Base
{
public:
	virtual void demo()=0;
	virtual void demo_2()
	{ printf("Base demo_2\n"); }
};

class	Derived:public Base
{
public:
	virtual	void demo()
	{ printf("Derived demo\n"); }
};

void	main()
{	
	Base *p = new Derived();
	p->demo();
/*	__asm
	{
		mov	eax,p
		mov	eax,[eax]
		add	eax,8
		call [eax]
	}	
*/
}