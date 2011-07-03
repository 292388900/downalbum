

#include <stdio.h>

//Base 作为基类,含有两个虚函数,一个普通函数
class	Base
{
public :
	virtual	void demo_1()//虚函数1
	{
		printf("Base demo_1\n");
	}

	virtual void demo_2()//虚函数2
	{
		printf("Base demo_2\n");
	}

	void demo_3()//普通函数3
	{
		printf("Base demo_3\n");
	}
	~Base()
	{
		printf("Base destroy\n");
	}
};

//Derived 作为派生类,含有和基类Base同名的两个虚函数函数和一个普通函数
class	Derived:public Base
{
public :
	virtual	void demo_1()//虚函数1
	{
		printf("Derived demo_1\n");
	}

	virtual void demo_2()//虚函数2
	{
		printf("Derived demo_2\n");
	}

	void demo_3()//普通函数3
	{
		printf("Derived demo_3\n");
	}

	~Derived()
	{
		printf("Derived destroy\n");
	}
};

void main()
{
	Base *p = new Base();
	p->demo_1();
	p->demo_2();
	p->demo_3();

	delete(p);

	p = new Derived();
	p->demo_1();
	p->demo_2();
	p->demo_3();		//调用的是基类的方法

	delete(p);		//内存泄露……
}
/*程序运行输出结果
Base demo_1
Base demo_2
Base demo_3
Derived demo_1
Derived demo_2
Base demo_3
Press any key to continue
*/