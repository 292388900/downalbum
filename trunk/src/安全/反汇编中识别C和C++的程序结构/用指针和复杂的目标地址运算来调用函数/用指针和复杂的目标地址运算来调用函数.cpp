//用指针和复杂的目标地址运算来调用函数

void	func_1();
void	func_2();
void	func_3();

void	main()
{
	int i;
	void (*a[3])()={func_1,func_2,func_3};
	void(*f)();	//指向函数的指针

	for(i=0;i<3;i++)
	{
		f=a[i];
		f();
	}
}

void	func_1()
{
}
void	func_2()
{
}
void	func_3()
{
}