//��ָ��͸��ӵ�Ŀ���ַ���������ú���

void	func_1();
void	func_2();
void	func_3();

void	main()
{
	int i;
	void (*a[3])()={func_1,func_2,func_3};
	void(*f)();	//ָ������ָ��

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