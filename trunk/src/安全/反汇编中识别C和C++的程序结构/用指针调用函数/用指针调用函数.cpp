//��ָ����ú���ʾ������

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