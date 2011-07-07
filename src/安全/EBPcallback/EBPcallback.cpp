#include<stdio.h>
#include<stdlib.h>
#include <conio.h>

void Method_1();
void Method_2();
void Method_3();

void main()
{
	printf("In main\n");
	Method_3();

	getch();
}

void Method_3()
{
	printf("In Method_3\n");
	Method_2();
}
void Method_2()
{
	printf("In Method_2\n");
	Method_1();
}
void Method_1()
{
	int* p;
	int i =0;
	//void(*pFunc[4])
	printf("In Method_1\n");
	p = (int*)&p + 1;

	do{
		printf("Method_%d,the Ret Adrr is:%08X\n",++i,*(p+1));
		p = (int*)*p;
	}
	while(*(p+1));//ret
	//while(*(p+1)>0x400000 && *(p+1)<0x500000);
}

//Êä³ö½á¹û£º
/*
In main
In Method_3
In Method_2
In Method_1
Method_1,the Ret Adrr is:004010FA
Method_2,the Ret Adrr is:004010AA
Method_3,the Ret Adrr is:0040105A
Method_4,the Ret Adrr is:00401319
Method_5,the Ret Adrr is:7C817067
*/






