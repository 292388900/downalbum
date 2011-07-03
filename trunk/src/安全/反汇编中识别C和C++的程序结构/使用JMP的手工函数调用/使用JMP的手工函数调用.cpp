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
	a++;		//不被执行
return_here:
	a++;		//回到这里执行
}

void	func()
{
}
