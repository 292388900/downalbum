void	func();

void	main()
{	
	int	a=0;

	__asm
	{
		LEA		ESI,return_here
		PUSH	ESI
		JMP		func
	}
	a++;		//����ִ��
return_here:
	a++;		//�ص�����ִ��
}

void	func()
{
}
