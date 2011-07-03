//用指针调用函数示例代码

void	func();

void	main()
{
	void(*a)();	
	a=func;
	//(*a)();
	a();
}

void	func()
{
	int	a;
	a++;
}