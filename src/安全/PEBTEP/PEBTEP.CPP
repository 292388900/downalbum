/*
* -----------------------------------------------------------------------
* Compile : For x86/EWindows XP SP1 & VC 7
*         : cl EnumModule.c /nologo /Os /G6 /W3 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /link /RELEASE
*         :
* Create  : 2003-08-12 11:36
* Modify  : 
* -----------------------------------------------------------------------
*/

/*
* 按加载顺序遍历双向循环链表
*/

#include <stdio.h>
#include <stdlib.h>

#pragma comment( linker, "/INCREMENTAL:NO"    )
#pragma comment( linker, "/subsystem:console" )

int __cdecl main ( int argc, char * argv[] )
{
    void *PEB         = NULL,
         *Ldr         = NULL,
         *Flink       = NULL,
         *p           = NULL,
         *BaseAddress = NULL,
         *FullDllName = NULL;

    __asm
    {
        mov     eax,fs:[0x30]
        mov     PEB,eax
    }
    printf( "PEB   = 0x%08X\n", PEB );
    Ldr   = *( ( void ** )( ( unsigned char * )PEB + 0x0c ) );
    printf( "Ldr   = 0x%08X\n", Ldr );
    Flink = *( ( void ** )( ( unsigned char * )Ldr + 0x0c ) );
    printf( "Flink = 0x%08X\n", Flink );
    p     = Flink;
    do
    {
        BaseAddress = *( ( void ** )( ( unsigned char * )p + 0x18 ) );
		//   ULONG             SizeOfImage;                      // +0x20
        FullDllName = *( ( void ** )( ( unsigned char * )p + 0x28 ) );
		if ( FullDllName==NULL ){
			break;
		}
        printf( "p     = 0x%08X 0x%08X ", p, BaseAddress );
        wprintf( L"%s\n", FullDllName );
        p = *( ( void ** )p );
    }
    while ( Flink != p );

	system("pause");
    return( EXIT_SUCCESS );
}  /* end of main */
