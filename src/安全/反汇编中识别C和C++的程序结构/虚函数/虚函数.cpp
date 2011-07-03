#include	<stdio.h>

class	Base
{
public:
	virtual void demo_1()
	{ printf("BASE demo_1\n"); }

	virtual void demo_2()
	{ printf("BASE demo_2\n"); }

	void demo_3()
	{ printf("BASE demo_3\n"); }
};

class	Derived:public Base
{
public:
	virtual	void demo_1()
	{ printf("Derived demo_1\n"); }

	void demo_2()
	{ printf("Derived demo_2\n"); }

	void demo_3()
	{ printf("Derived demo_3\n"); }
};

void	main()
{
	Base *p = new Base();
	p->demo_1();
	p->demo_2();
	p->demo_3();

	p = new Derived();
	p->demo_1();
	p->demo_2();
	p->demo_3();
}