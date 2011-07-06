
#include "stdafx.h"
#include "Common.h"

/*
输入表处理，一定要注意处理转向的函数，例如kernel32的HeapAlloc实际上转向到ntdll的HeapAlloc
*/
bool ProcessImportTable(PVOID lpNewImgBase)
{
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpNewImgBase;
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)( (ULONG)pDosHeader + pDosHeader->e_lfanew );

	//获取RtlImageDirectoryEntryToData函数地址
	HMODULE hNtDll = LoadLibrary( _T("ntdll.dll") );
	PRTLIMAGEDIRECTORYENTRYTODATA pRtlImageDirectoryEntryToData = (PRTLIMAGEDIRECTORYENTRYTODATA)GetProcAddress( hNtDll, "RtlImageDirectoryEntryToData" );
	if( pRtlImageDirectoryEntryToData == NULL )
	{
		TRACE("获取RtlImageDirectoryEntryToData函数地址失败\n");
		FreeLibrary( hNtDll );
		return false;
	}
	DWORD dwImportSize = 0;
	PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)pRtlImageDirectoryEntryToData( lpNewImgBase, true, IMAGE_DIRECTORY_ENTRY_IMPORT, &dwImportSize );
	if( pImportDescriptor == NULL )
	{
		TRACE("获取输表信息出错\n");
		FreeLibrary( hNtDll );
		return false;
	}

	FreeLibrary( hNtDll );


	while( pImportDescriptor->Name != 0 )
	{
		PIMAGE_THUNK_DATA32 pFirstThunkData = (PIMAGE_THUNK_DATA32)( (ULONG)lpNewImgBase + pImportDescriptor->FirstThunk);
		PIMAGE_THUNK_DATA32 pOriginalThunkData = (PIMAGE_THUNK_DATA32)( (ULONG)lpNewImgBase + pImportDescriptor->OriginalFirstThunk);

		//获取dll名
		char *pDllName = (char*)( (ULONG)lpNewImgBase + pImportDescriptor->Name );
		//TRACE("处理输入表，DLL：%s\n",pDllName);

		//循环处理该dll中的每个输入表函数
		while( (pOriginalThunkData->u1.Ordinal != 0 ) && !( pOriginalThunkData->u1.Ordinal&0x80000000) )
		{
			PIMAGE_IMPORT_BY_NAME pImageImportByName = (PIMAGE_IMPORT_BY_NAME)( (ULONG)lpNewImgBase + (ULONG)(pOriginalThunkData->u1.AddressOfData) );
			char *pFuncName = (char*)(&pImageImportByName->Name);
			if ( stricmp(pDllName,"SHLWAPI.DLL")==0 ){
				int a=0;
			}
			DWORD dwFuncAddr = GetFuncAddrFromModule( pDllName, pFuncName );
			//TRACE("函数名：%s  函数地址：%08X\n",pFuncName,dwFuncAddr);

			if( dwFuncAddr == 0 )
			{
				//TRACE("获取输入函数地址出错\n");
				return false;
			}
			*(PULONG)pFirstThunkData = dwFuncAddr;

			pFirstThunkData++;
			pOriginalThunkData++;
		}
		pImportDescriptor++;
	}

	return true;

}

/*
函数描述：获取指定dll模块导出函数地址
参数：
pDllName:dll模块名
pFuncName:需要获取地址的函数名
返回值：
返回函数地址
*/
DWORD GetFuncAddrFromModule( char *pDllName, char *pFuncName )
{
	//首先获取模块基地址,这里pDllName必须是系统已经加载的模块，如果ReloadAndRun的目标文件含有自己的dll，那么就会失败


	DWORD dwModuleBase = (DWORD)GetModuleHandleA( pDllName );
	if ( dwModuleBase==NULL ){
		dwModuleBase = (DWORD)LoadLibraryA( pDllName );
	}

	//查找到模块基址后，解析其导出表获取指定函数地址
	//再次获取RtlImageDirectoryEntryToData函数地址
	//获取RtlImageDirectoryEntryToData函数地址
	HMODULE hNtDll = LoadLibrary( _T("ntdll.dll") );
	PRTLIMAGEDIRECTORYENTRYTODATA pRtlImageDirectoryEntryToData = (PRTLIMAGEDIRECTORYENTRYTODATA)GetProcAddress( hNtDll, "RtlImageDirectoryEntryToData" );
	if( pRtlImageDirectoryEntryToData == NULL )
	{
		//TRACE("获取RtlImageDirectoryEntryToData函数地址失败\n");
		FreeLibrary( hNtDll );
		return 0;
	}
	DWORD dwExportSize = 0;
	PIMAGE_EXPORT_DIRECTORY pExportDescriptor = (PIMAGE_EXPORT_DIRECTORY)pRtlImageDirectoryEntryToData( (PVOID)dwModuleBase, true, IMAGE_DIRECTORY_ENTRY_EXPORT, &dwExportSize );
	if( pExportDescriptor == NULL )
	{
		//TRACE("获取导出表结构失败\n");
		FreeLibrary( hNtDll );
		return 0;
	}
	FreeLibrary( hNtDll );

	//采用而分查找法查找函数地址
	PULONG pNameTableBase = (PULONG)(dwModuleBase + pExportDescriptor->AddressOfNames);
	PUSHORT pNameOrdinalTableBase = (PUSHORT)(dwModuleBase + pExportDescriptor->AddressOfNameOrdinals);

	DWORD dwLow = 0;
	DWORD dwHigh = pExportDescriptor->NumberOfNames - 1;
	DWORD dwMid = 0;
	while( dwLow <= dwHigh )
	{
		dwMid = (dwLow + dwHigh) >> 1;
		LONG lRes = strcmp( (char*)(dwModuleBase+pNameTableBase[dwMid]), pFuncName );
		if( lRes > 0 )
			dwHigh = dwMid - 1;
		else if(lRes < 0 )
			dwLow = dwMid + 1;
		else
			break;
	}
	if( dwLow > dwHigh )
	{
		//TRACE("查找函数失败\n");
		return 0;
	}
	DWORD dwOridinalName = pNameOrdinalTableBase[dwMid];
	if( dwOridinalName > pExportDescriptor->NumberOfFunctions )
	{
		//TRACE("获取的函数序号错误\n");
		return 0;
	}
	PULONG pAddressTableBase = (PULONG)(dwModuleBase + pExportDescriptor->AddressOfFunctions);
	DWORD dwFuncAddr = dwModuleBase + pAddressTableBase[dwOridinalName];

	//////////////////////////////////////////////////////////////////////////
	//这里简单处理下转向问题
	if ( IsCharAlphaNumeric(*(PCHAR)dwFuncAddr) && IsCharAlphaNumeric(*(PCHAR)(dwFuncAddr+1))
		&& IsCharAlphaNumeric(*(PCHAR)(dwFuncAddr+2)) && IsCharAlphaNumeric(*(PCHAR)(dwFuncAddr+3)) ){
			char szDllName[50]={0};
			char szFuncName[50]={0};
			char*p=strchr((char*)dwFuncAddr,'.');
			if ( p!=NULL ){
				memcpy(szDllName,(char*)dwFuncAddr,(DWORD)p-dwFuncAddr);
				strcat(szDllName,".dll");
				strcpy(szFuncName,p+1);

				dwModuleBase = (DWORD)GetModuleHandle( szDllName );
				if ( dwModuleBase==NULL ){
					dwModuleBase = (DWORD)LoadLibrary( szDllName );
				}
				dwFuncAddr=(DWORD)GetProcAddress((HMODULE)dwModuleBase,szFuncName);
			}
	}
	//////////////////////////////////////////////////////////////////////////
	return dwFuncAddr;

}


/*
函数描述：将读出的文件内容按内存对齐格式对齐
参数：
pFileBuffer:文件内容缓冲
返回值：内存对齐后的文件缓冲
*/
PVOID AlignFileToMem(PVOID pImageBuffer,DWORD dwImageSize,PVOID pFileBuffer,DWORD dwFileSize)
{
	//这里有两种方法可以计算对齐后的文件大小：保存在_IMAGE_NT_HEADERS结构中，然后遍历每个节头，根据节头中的节内存偏移拷贝文件内容
	//但有时节表中的节偏移不准确，所以我们这里根据PE文件格式自己计算

	//填充映像内存
	memset( pImageBuffer, 0, dwImageSize );

	//复制PE头信息
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)( (ULONG)pFileBuffer + ((PIMAGE_DOS_HEADER)pFileBuffer)->e_lfanew );
	PIMAGE_SECTION_HEADER pSectionHeader = (PIMAGE_SECTION_HEADER)( (ULONG)pNtHeaders + sizeof(IMAGE_NT_HEADERS) );
	DWORD dwCpySize = pNtHeaders->OptionalHeader.SizeOfHeaders;
	for( DWORD dwIndex = 0; dwIndex < pNtHeaders->FileHeader.NumberOfSections; dwIndex++ )
	{
		if( (pSectionHeader[dwIndex].PointerToRawData) != 0 && (pSectionHeader[dwIndex].PointerToRawData<dwCpySize) )
			dwCpySize = pSectionHeader[dwIndex].PointerToRawData;
	}
	memcpy( pImageBuffer, pFileBuffer, dwCpySize );
	PVOID pt = (PVOID)((ULONG)pImageBuffer + GetAlignSize( pNtHeaders->OptionalHeader.SizeOfHeaders, pNtHeaders->OptionalHeader.SectionAlignment ));
	for( DWORD dwIndex = 0; dwIndex < pNtHeaders->FileHeader.NumberOfSections; dwIndex++ )
	{
		if( pSectionHeader[dwIndex].VirtualAddress != 0 )
			pt = (PVOID)( (DWORD)pImageBuffer + pSectionHeader[dwIndex].VirtualAddress);
		if( pSectionHeader[dwIndex].SizeOfRawData != 0 )
		{
			memcpy( pt, (PVOID)( (DWORD)pFileBuffer + pSectionHeader[dwIndex].PointerToRawData), pSectionHeader[dwIndex].SizeOfRawData );
			if( pSectionHeader[dwIndex].SizeOfRawData > pSectionHeader[dwIndex].Misc.VirtualSize )
				pt = (PVOID)( (ULONG)pt + GetAlignSize(pSectionHeader[dwIndex].SizeOfRawData, pNtHeaders->OptionalHeader.SectionAlignment ) );
			else
				pt = (PVOID)( (ULONG)pt + GetAlignSize(pSectionHeader[dwIndex].Misc.VirtualSize, pNtHeaders->OptionalHeader.SectionAlignment ) );

		}
		else
			pt = (PVOID)( (ULONG)pt + GetAlignSize(pSectionHeader[dwIndex].Misc.VirtualSize, pNtHeaders->OptionalHeader.SectionAlignment ) );
	}
	return pImageBuffer;
}

/*
函数说明：计算文件内存对齐后的大小,首先计算文件头对齐值大小，再计算每节的对齐值大小：如果节表中的VirtualAddress非空，VirtualSize非0，则按VirtualSize值计算对齐值
//如果为0，则按SizeOfRawData计算对齐值；反之，如果VirtualAddress为空，谁大就按谁计算对齐值
参数：
pFileBuffer:文件内容起始地址
返回值：返回文件内容按内存对齐后的大小
*/
DWORD GetTotalImageSize( PVOID pFileBuffer, DWORD dwFileSize )
{
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pFileBuffer;
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)( (ULONG)pFileBuffer + pDosHeader->e_lfanew );

	DWORD dwTotalSize = GetAlignSize( pNtHeaders->OptionalHeader.SizeOfHeaders, pNtHeaders->OptionalHeader.SectionAlignment );

	PIMAGE_SECTION_HEADER pSectionHeader = (PIMAGE_SECTION_HEADER)( (ULONG)pNtHeaders + sizeof(IMAGE_NT_HEADERS) );

	DWORD dwSectionCnt = pNtHeaders->FileHeader.NumberOfSections;
	for( DWORD dwIndex = 0; dwIndex < dwSectionCnt; dwIndex++ )
	{
		if( (pSectionHeader[dwIndex].PointerToRawData + pSectionHeader[dwIndex].SizeOfRawData) > dwFileSize )
		{
			TRACE("查找函数失败\n");
			return 0;
		}
		else if( pSectionHeader[dwIndex].VirtualAddress != NULL )
		{
			if( pSectionHeader[dwIndex].Misc.VirtualSize != 0 )
				dwTotalSize += GetAlignSize( pSectionHeader[dwIndex].Misc.VirtualSize, pNtHeaders->OptionalHeader.SectionAlignment );
			else
				dwTotalSize += GetAlignSize( pSectionHeader[dwIndex].SizeOfRawData, pNtHeaders->OptionalHeader.SectionAlignment );
		}
		else 
		{
			if( pSectionHeader[dwIndex].SizeOfRawData > pSectionHeader[dwIndex].Misc.VirtualSize )
				dwTotalSize += GetAlignSize( pSectionHeader[dwIndex].SizeOfRawData, pNtHeaders->OptionalHeader.SectionAlignment );
			else
				dwTotalSize += GetAlignSize( pSectionHeader[dwIndex].Misc.VirtualSize, pNtHeaders->OptionalHeader.SectionAlignment );
		}
	}
	return dwTotalSize;
}

DWORD GetAlignSize( DWORD dwSize, DWORD dwAlignSize )
{
	return (dwSize+dwAlignSize-1)/dwAlignSize*dwAlignSize;
}

/*
函数说明：处理节内存对齐后文件的重定位表
参数：
pImageBuffer:对齐后的文件基址
*/
bool ProcessRelocTable( PVOID pImageBuffer, PVOID lpNewImgBase )
{

	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pImageBuffer;
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)( (ULONG)pDosHeader + pDosHeader->e_lfanew );

	//获取RtlImageDirectoryEntryToData函数地址
	HMODULE hNtDll = LoadLibrary( _T("ntdll.dll") );
	PRTLIMAGEDIRECTORYENTRYTODATA pRtlImageDirectoryEntryToData = (PRTLIMAGEDIRECTORYENTRYTODATA)GetProcAddress( hNtDll, "RtlImageDirectoryEntryToData" );
	if( pRtlImageDirectoryEntryToData == NULL )
	{
		TRACE("获取RtlImageDirectoryEntryToData函数地址失败\n");
		FreeLibrary( hNtDll );
		return false;
	}
	DWORD dwRelocSize = 0; 
	PIMAGE_BASE_RELOCATION pRelocDescriptor = (PIMAGE_BASE_RELOCATION)pRtlImageDirectoryEntryToData( pImageBuffer, true, IMAGE_DIRECTORY_ENTRY_BASERELOC, &dwRelocSize );
	if( pRelocDescriptor == NULL )
	{
		FreeLibrary( hNtDll );
		TRACE("无重定位表\n");
		return true;
	}
	FreeLibrary( hNtDll );

	DWORD dwRelocaSize = pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size;
	DWORD dwDelta = (DWORD)lpNewImgBase - pNtHeaders->OptionalHeader.ImageBase;
	while( dwRelocaSize > 0 )
	{
		PUSHORT pFixup = (PUSHORT)((ULONG)pRelocDescriptor + sizeof(IMAGE_BASE_RELOCATION));
		for( DWORD dwIndex = 0; dwIndex< ( (pRelocDescriptor->SizeOfBlock-sizeof(IMAGE_BASE_RELOCATION))/2 ); dwIndex++ )
		{

			if( (pFixup[dwIndex]>>12) == IMAGE_REL_BASED_HIGHLOW )
			{
				DWORD dwAddr = (DWORD)pImageBuffer + pRelocDescriptor->VirtualAddress + (pFixup[dwIndex]&0xfff);
				*(PULONG)dwAddr += dwDelta;
			}

			//DWORD dwAddr = (DWORD)pImageBuffer + pRelocDescriptor->VirtualAddress + (pFixup[dwIndex]&0xfff);
			//	*(PULONG)dwAddr += dwDelta;
		}
		dwRelocaSize -= pRelocDescriptor->SizeOfBlock;
		pRelocDescriptor = (PIMAGE_BASE_RELOCATION)((ULONG)pRelocDescriptor + pRelocDescriptor->SizeOfBlock);
	}
	return true;

}