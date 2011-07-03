//直接函数调用示例代码

void	func();

void	main()
{
	int	a;
	func();
	a=0x666;
	func();
}

void	func()
{
	int	a;
	a++;
}