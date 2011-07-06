
#include "stdafx.h"
#include "Common.h"

/*
�������һ��Ҫע�⴦��ת��ĺ���������kernel32��HeapAllocʵ����ת��ntdll��HeapAlloc
*/
bool ProcessImportTable(PVOID lpNewImgBase)
{
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpNewImgBase;
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)( (ULONG)pDosHeader + pDosHeader->e_lfanew );

	//��ȡRtlImageDirectoryEntryToData������ַ
	HMODULE hNtDll = LoadLibrary( _T("ntdll.dll") );
	PRTLIMAGEDIRECTORYENTRYTODATA pRtlImageDirectoryEntryToData = (PRTLIMAGEDIRECTORYENTRYTODATA)GetProcAddress( hNtDll, "RtlImageDirectoryEntryToData" );
	if( pRtlImageDirectoryEntryToData == NULL )
	{
		TRACE("��ȡRtlImageDirectoryEntryToData������ַʧ��\n");
		FreeLibrary( hNtDll );
		return false;
	}
	DWORD dwImportSize = 0;
	PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)pRtlImageDirectoryEntryToData( lpNewImgBase, true, IMAGE_DIRECTORY_ENTRY_IMPORT, &dwImportSize );
	if( pImportDescriptor == NULL )
	{
		TRACE("��ȡ�����Ϣ����\n");
		FreeLibrary( hNtDll );
		return false;
	}

	FreeLibrary( hNtDll );


	while( pImportDescriptor->Name != 0 )
	{
		PIMAGE_THUNK_DATA32 pFirstThunkData = (PIMAGE_THUNK_DATA32)( (ULONG)lpNewImgBase + pImportDescriptor->FirstThunk);
		PIMAGE_THUNK_DATA32 pOriginalThunkData = (PIMAGE_THUNK_DATA32)( (ULONG)lpNewImgBase + pImportDescriptor->OriginalFirstThunk);

		//��ȡdll��
		char *pDllName = (char*)( (ULONG)lpNewImgBase + pImportDescriptor->Name );
		//TRACE("���������DLL��%s\n",pDllName);

		//ѭ�������dll�е�ÿ���������
		while( (pOriginalThunkData->u1.Ordinal != 0 ) && !( pOriginalThunkData->u1.Ordinal&0x80000000) )
		{
			PIMAGE_IMPORT_BY_NAME pImageImportByName = (PIMAGE_IMPORT_BY_NAME)( (ULONG)lpNewImgBase + (ULONG)(pOriginalThunkData->u1.AddressOfData) );
			char *pFuncName = (char*)(&pImageImportByName->Name);
			if ( stricmp(pDllName,"SHLWAPI.DLL")==0 ){
				int a=0;
			}
			DWORD dwFuncAddr = GetFuncAddrFromModule( pDllName, pFuncName );
			//TRACE("��������%s  ������ַ��%08X\n",pFuncName,dwFuncAddr);

			if( dwFuncAddr == 0 )
			{
				//TRACE("��ȡ���뺯����ַ����\n");
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
������������ȡָ��dllģ�鵼��������ַ
������
pDllName:dllģ����
pFuncName:��Ҫ��ȡ��ַ�ĺ�����
����ֵ��
���غ�����ַ
*/
DWORD GetFuncAddrFromModule( char *pDllName, char *pFuncName )
{
	//���Ȼ�ȡģ�����ַ,����pDllName������ϵͳ�Ѿ����ص�ģ�飬���ReloadAndRun��Ŀ���ļ������Լ���dll����ô�ͻ�ʧ��


	DWORD dwModuleBase = (DWORD)GetModuleHandleA( pDllName );
	if ( dwModuleBase==NULL ){
		dwModuleBase = (DWORD)LoadLibraryA( pDllName );
	}

	//���ҵ�ģ���ַ�󣬽����䵼�����ȡָ��������ַ
	//�ٴλ�ȡRtlImageDirectoryEntryToData������ַ
	//��ȡRtlImageDirectoryEntryToData������ַ
	HMODULE hNtDll = LoadLibrary( _T("ntdll.dll") );
	PRTLIMAGEDIRECTORYENTRYTODATA pRtlImageDirectoryEntryToData = (PRTLIMAGEDIRECTORYENTRYTODATA)GetProcAddress( hNtDll, "RtlImageDirectoryEntryToData" );
	if( pRtlImageDirectoryEntryToData == NULL )
	{
		//TRACE("��ȡRtlImageDirectoryEntryToData������ַʧ��\n");
		FreeLibrary( hNtDll );
		return 0;
	}
	DWORD dwExportSize = 0;
	PIMAGE_EXPORT_DIRECTORY pExportDescriptor = (PIMAGE_EXPORT_DIRECTORY)pRtlImageDirectoryEntryToData( (PVOID)dwModuleBase, true, IMAGE_DIRECTORY_ENTRY_EXPORT, &dwExportSize );
	if( pExportDescriptor == NULL )
	{
		//TRACE("��ȡ������ṹʧ��\n");
		FreeLibrary( hNtDll );
		return 0;
	}
	FreeLibrary( hNtDll );

	//���ö��ֲ��ҷ����Һ�����ַ
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
		//TRACE("���Һ���ʧ��\n");
		return 0;
	}
	DWORD dwOridinalName = pNameOrdinalTableBase[dwMid];
	if( dwOridinalName > pExportDescriptor->NumberOfFunctions )
	{
		//TRACE("��ȡ�ĺ�����Ŵ���\n");
		return 0;
	}
	PULONG pAddressTableBase = (PULONG)(dwModuleBase + pExportDescriptor->AddressOfFunctions);
	DWORD dwFuncAddr = dwModuleBase + pAddressTableBase[dwOridinalName];

	//////////////////////////////////////////////////////////////////////////
	//����򵥴�����ת������
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
�������������������ļ����ݰ��ڴ�����ʽ����
������
pFileBuffer:�ļ����ݻ���
����ֵ���ڴ�������ļ�����
*/
PVOID AlignFileToMem(PVOID pImageBuffer,DWORD dwImageSize,PVOID pFileBuffer,DWORD dwFileSize)
{
	//���������ַ������Լ���������ļ���С��������_IMAGE_NT_HEADERS�ṹ�У�Ȼ�����ÿ����ͷ�����ݽ�ͷ�еĽ��ڴ�ƫ�ƿ����ļ�����
	//����ʱ�ڱ��еĽ�ƫ�Ʋ�׼ȷ�����������������PE�ļ���ʽ�Լ�����

	//���ӳ���ڴ�
	memset( pImageBuffer, 0, dwImageSize );

	//����PEͷ��Ϣ
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
����˵���������ļ��ڴ�����Ĵ�С,���ȼ����ļ�ͷ����ֵ��С���ټ���ÿ�ڵĶ���ֵ��С������ڱ��е�VirtualAddress�ǿգ�VirtualSize��0����VirtualSizeֵ�������ֵ
//���Ϊ0����SizeOfRawData�������ֵ����֮�����VirtualAddressΪ�գ�˭��Ͱ�˭�������ֵ
������
pFileBuffer:�ļ�������ʼ��ַ
����ֵ�������ļ����ݰ��ڴ�����Ĵ�С
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
			TRACE("���Һ���ʧ��\n");
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
����˵����������ڴ������ļ����ض�λ��
������
pImageBuffer:�������ļ���ַ
*/
bool ProcessRelocTable( PVOID pImageBuffer, PVOID lpNewImgBase )
{

	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pImageBuffer;
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)( (ULONG)pDosHeader + pDosHeader->e_lfanew );

	//��ȡRtlImageDirectoryEntryToData������ַ
	HMODULE hNtDll = LoadLibrary( _T("ntdll.dll") );
	PRTLIMAGEDIRECTORYENTRYTODATA pRtlImageDirectoryEntryToData = (PRTLIMAGEDIRECTORYENTRYTODATA)GetProcAddress( hNtDll, "RtlImageDirectoryEntryToData" );
	if( pRtlImageDirectoryEntryToData == NULL )
	{
		TRACE("��ȡRtlImageDirectoryEntryToData������ַʧ��\n");
		FreeLibrary( hNtDll );
		return false;
	}
	DWORD dwRelocSize = 0; 
	PIMAGE_BASE_RELOCATION pRelocDescriptor = (PIMAGE_BASE_RELOCATION)pRtlImageDirectoryEntryToData( pImageBuffer, true, IMAGE_DIRECTORY_ENTRY_BASERELOC, &dwRelocSize );
	if( pRelocDescriptor == NULL )
	{
		FreeLibrary( hNtDll );
		TRACE("���ض�λ��\n");
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