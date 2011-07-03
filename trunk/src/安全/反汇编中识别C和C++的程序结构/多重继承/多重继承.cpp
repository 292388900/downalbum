#include	<stdio.h>

class	A
{
public:
	virtual void f()
	{ printf("A_f\n"); }
};

class	B
{
public:
	virtual	void f()
	{ printf("B_f\n"); }
	virtual	void g()
	{ printf("B_g\n"); }
};

class	C:public A,public B
{
public:
    void f()
	{ printf("C_f\n"); }
};

void	main()
{	
	A*a=new A();
	B*b=new B();
	C*c=new C();
	
	a->f();
	b->f();
	b->g();
	c->f();
	c->g();
	c->A::f();
	c->B::f();
	c->B::g();
}