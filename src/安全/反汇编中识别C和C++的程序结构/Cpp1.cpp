#include	<stdio.h>

class	Base
{
public:
	void Base_demo_1()
	{ printf("BASE demo_1\n"); }

	void Base_demo_2()
	{ printf("BASE demo_2\n"); }

};

class	Derived:public Base
{
public:
	void Derived_demo_1()
	{ printf("Derived demo_1\n"); }

	void Derived_demo_2()
	{ printf("Derived demo_2\n"); }
};

void	main()
{
	Base *p = new Base();
	p->Base_demo_1();
	p->Base_demo_2();

	Derived *d = new Derived();
	d->Base_demo_1();
	d->Base_demo_2();
	d->Derived_demo_1();
	d->Derived_demo_2();
}

