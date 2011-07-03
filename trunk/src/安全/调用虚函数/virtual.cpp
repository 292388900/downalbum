

#include <stdio.h>

//Base ��Ϊ����,���������麯��,һ����ͨ����
class	Base
{
public :
	virtual	void demo_1()//�麯��1
	{
		printf("Base demo_1\n");
	}

	virtual void demo_2()//�麯��2
	{
		printf("Base demo_2\n");
	}

	void demo_3()//��ͨ����3
	{
		printf("Base demo_3\n");
	}
	~Base()
	{
		printf("Base destroy\n");
	}
};

//Derived ��Ϊ������,���кͻ���Baseͬ���������麯��������һ����ͨ����
class	Derived:public Base
{
public :
	virtual	void demo_1()//�麯��1
	{
		printf("Derived demo_1\n");
	}

	virtual void demo_2()//�麯��2
	{
		printf("Derived demo_2\n");
	}

	void demo_3()//��ͨ����3
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
	p->demo_3();		//���õ��ǻ���ķ���

	delete(p);		//�ڴ�й¶����
}
/*��������������
Base demo_1
Base demo_2
Base demo_3
Derived demo_1
Derived demo_2
Base demo_3
Press any key to continue
*/