// Loader.cpp : Defines the entry point for the console application.
//

/*
ʱ�䣺2011��2��12��
���ߣ�angelkiss
�������ܣ�Reload And Run����Ŀ¼�µ�test.exe����
*/

#include "stdafx.h"
#include <afx.h>
#include <iostream>
#include <tchar.h>
#include <afxwin.h>
#include <Psapi.h>
#include <shlwapi.h>

//#include "ntimage.h"
#include "Loader.h"

using namespace std;


#pragma comment(lib,"Psapi.lib")
#pragma comment(lib,"shlwapi.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString GetStartPath()
{
	TCHAR szTemp[MAX_PATH*2];
	GetModuleFileName(NULL,szTemp,sizeof(szTemp)/sizeof(TCHAR));
	StrRChr(szTemp,NULL,'\\')[1]=0;
	return szTemp;
}

int main(int argc, char* argv[])
{
	CString strStartPath=GetStartPath();
	//CString strTargetFile=_T("C:\\windows\\notepad.exe");
	CString strTargetFile=strStartPath+_T("notepad.exe");

	//��test.exe�ļ�������ȡ�ļ�����
	TCHAR szDllFile[MAX_PATH] = {0};
	GetModuleFileName(NULL,szDllFile,_countof(szDllFile));
#ifdef _DEBUG
	lstrcpy(&StrRChr(szDllFile,NULL,'\\')[1],_T("dllexe.dll"));
#else
	lstrcpy(&StrRChr(szDllFile,NULL,'\\')[1],_T("dllexe.dll"));
#endif


	//���Ȼ�ȡĿ��exe��OEP
	//////////////////////////////////////////////////////////////////////////

	HANDLE hFile=CreateFile(strTargetFile,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if ( hFile==INVALID_HANDLE_VALUE ){
		AfxMessageBox(_T("���ļ�ʧ��"));
		return -1;
	}

	DWORD size_low,size_high;
	size_low= GetFileSize(hFile,&size_high); 

	HANDLE hMap=CreateFileMapping(hFile,NULL,PAGE_READONLY,size_high,size_low,NULL);
	if ( hMap==NULL ){
		AfxMessageBox(_T("CreateFileMappingʧ��"));
		CloseHandle(hFile);
		return -1;
	}

	PBYTE pFile=(PBYTE)MapViewOfFile(hMap,FILE_MAP_READ,0,0,0);
	if ( pFile==NULL ){
		AfxMessageBox(_T("MapViewOfFileʧ��"));
		CloseHandle(hMap);
		CloseHandle(hFile);
		return -1;
	}

	PIMAGE_DOS_HEADER pDosHdr=(PIMAGE_DOS_HEADER)pFile;
	PIMAGE_NT_HEADERS pNtHdr=(PIMAGE_NT_HEADERS)((PBYTE)pDosHdr+pDosHdr->e_lfanew);
	DWORD dwImageSize=pNtHdr->OptionalHeader.SizeOfImage;
	DWORD dwImageBase=pNtHdr->OptionalHeader.ImageBase;
	DWORD dwEP=pNtHdr->OptionalHeader.AddressOfEntryPoint+dwImageBase;

	UnmapViewOfFile(pFile);
	CloseHandle(hMap);
	CloseHandle(hFile);

	//�Ե��Եķ�ʽ����notepad����
	//////////////////////////////////////////////////////////////////////////
	STARTUPINFO si={sizeof(si)};
	PROCESS_INFORMATION pi;
	//ʹ��DETACHED_PROCESS��־��Ϊ�˲����ӽ���ӵ�и����̵Ŀ���̨����
	CreateProcess(strTargetFile,NULL,NULL,NULL,FALSE,DEBUG_PROCESS|DEBUG_ONLY_THIS_PROCESS|DETACHED_PROCESS,NULL,NULL,&si,&pi);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	CString strText;
	BOOL bPatched=FALSE;
	DEBUG_EVENT stDbgevent;
	CONTEXT stThreadContext;
	DWORD dwProcessId;
	LPVOID lpImageBase;
	HANDLE hProcess;
	HANDLE hThread;
	LPVOID lpEntryPoint;
	DWORD dwReadWrite;
	while ( TRUE ){
		if ( WaitForDebugEvent(&stDbgevent,INFINITE)==0 ){
			//failed
			break;
		}

		if ( stDbgevent.dwDebugEventCode==CREATE_PROCESS_DEBUG_EVENT ){			//�����Խ��̸ձ�����

			//��ȡһЩ������Ϣ
			dwProcessId=stDbgevent.dwProcessId;
			lpImageBase=stDbgevent.u.CreateProcessInfo.lpBaseOfImage;
			lpEntryPoint=stDbgevent.u.CreateProcessInfo.lpStartAddress;
			hProcess=stDbgevent.u.CreateProcessInfo.hProcess;
			hThread=stDbgevent.u.CreateProcessInfo.hThread;

			//����ڴ�д��int3�ϵ�
			BYTE bInt3='\xCC';
			WriteProcessMemory(hProcess,lpEntryPoint,&bInt3,sizeof(BYTE),&dwReadWrite);
			FlushInstructionCache(hProcess,lpEntryPoint,1);	//������д���ڴ���cpuִ��
			ContinueDebugEvent(stDbgevent.dwProcessId, stDbgevent.dwThreadId, DBG_EXCEPTION_NOT_HANDLED ); 

		}else if ( stDbgevent.dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT ){	//���Խ���
			break;
		}else if ( stDbgevent.dwDebugEventCode == EXCEPTION_DEBUG_EVENT ){
			if ( stDbgevent.u.Exception.ExceptionRecord.ExceptionCode==EXCEPTION_BREAKPOINT 
				&& stDbgevent.u.Exception.ExceptionRecord.ExceptionAddress==lpEntryPoint ){
				//���쳣��������ڴ�ʱ���������µ�int3�ϵ㣬����patch������

				stThreadContext.ContextFlags=CONTEXT_CONTROL;
				GetThreadContext(hThread,&stThreadContext);

				//patch���룬�����µ�EIP
				stThreadContext.Eip=PatchCode(hProcess,lpImageBase,dwImageSize,szDllFile);
				SetThreadContext(hThread,&stThreadContext);

				//���ǵ�Ŀ�����ˣ�detach��������������
				ContinueDebugEvent(stDbgevent.dwProcessId, stDbgevent.dwThreadId, DBG_CONTINUE); 
				DebugActiveProcessStop(stDbgevent.dwProcessId);
				break;
			}else{
				//��DBG_CONTINUE��־�������е���
				ContinueDebugEvent(stDbgevent.dwProcessId, stDbgevent.dwThreadId, DBG_CONTINUE); 
			}

		}else{	//�������������
			ContinueDebugEvent(stDbgevent.dwProcessId, stDbgevent.dwThreadId, DBG_EXCEPTION_NOT_HANDLED); 
		}

	}

	system("pause");
	return 0;
}

DWORD PatchCode(HANDLE hTargetProcess,LPVOID lpImageBase,DWORD dwImageSize,LPCTSTR lpszFilePath)
{
	DWORD dwNewEip=0;

	//�޸�ҳ��������
	DWORD dwOldProtect=0;
	if ( !VirtualProtectEx(hTargetProcess,lpImageBase, dwImageSize, PAGE_EXECUTE_READWRITE, &dwOldProtect) ){
		AfxMessageBox(_T("VirtualProtectʧ��"));
		return dwNewEip;
	}
	
	HANDLE hFile = CreateFile(lpszFilePath,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	DWORD dwErr = GetLastError();
	if( hFile == INVALID_HANDLE_VALUE ){
		AfxMessageBox(_T("���ļ�ʧ��"));
		return dwNewEip;
	}

	DWORD dwFileSize = GetFileSize( hFile, NULL );
	if( dwFileSize == INVALID_FILE_SIZE ){
		AfxMessageBox(_T("��ȡ�ļ���Сʧ��"));
		CloseHandle(hFile);
		return dwNewEip;
	}

	//�����㹻��Ŀռ䣬���ļ�����
	PVOID pBuffer = (PVOID)new BYTE[dwFileSize];
	if( pBuffer == NULL ){
		AfxMessageBox(_T("�������ļ����ݿռ�ʧ��"));
		CloseHandle( hFile );
		return dwNewEip;
	}

	DWORD dwReadWrite = 0;
	if( ReadFile(hFile,pBuffer,dwFileSize,&dwReadWrite,NULL)==FALSE ){
		AfxMessageBox(_T("��ȡ�ļ�����ʧ��"));
		delete []pBuffer;
		CloseHandle( hFile );
		return dwNewEip;
	}

	//�����ļ��ڴ�����Ĵ�С
	DWORD dwSrcImgSize = GetTotalImageSize(pBuffer,dwFileSize);
	if ( dwSrcImgSize==0 ){
		AfxMessageBox(_T("GetTotalImageSizeʧ��"));
		delete []pBuffer;
		CloseHandle( hFile );
		return dwNewEip;
	}

	//����Ŀ���ַ
	PVOID lpNewImgBase=NULL;
	if ( dwSrcImgSize<=dwImageSize ){	//˵������ֱ��patch����
		lpNewImgBase=lpImageBase;
	}else{	//�ռ䲻�������ڴ�
		lpNewImgBase=VirtualAllocEx(hTargetProcess,NULL,dwSrcImgSize,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
		if ( lpNewImgBase==NULL ){
			AfxMessageBox(_T("VirtualAllocExʧ��"));
			delete []pBuffer;
			CloseHandle( hFile );
			return dwNewEip;
		}
	}

	//��ӳ�䵽��ǰ�����У����ֱ��д��Ŀ�����
	PVOID pImageBuffer=VirtualAllocEx(::GetCurrentProcess(),NULL,dwSrcImgSize,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	if ( lpNewImgBase==NULL ){
		AfxMessageBox(_T("VirtualAllocExʧ��"));
		delete []pBuffer;
		CloseHandle( hFile );
		return dwNewEip;
	}

	//���Ƚ��ڰ��ڴ����ֵ����
	AlignFileToMem(pImageBuffer,dwSrcImgSize,pBuffer,dwFileSize);
	if( pImageBuffer == NULL ){
		AfxMessageBox(_T("�����ļ�����ʧ��"));
		delete []pBuffer;
		CloseHandle( hFile );
		return dwNewEip;
	}

	//�ͷ��ڴ棬�ر��ļ�
	delete []pBuffer;
	CloseHandle( hFile );

	//�����ļ��ɹ������ȴ����������Ϣ
	if( !ProcessImportTable(pImageBuffer,lpNewImgBase) ){
		AfxMessageBox(_T("���������ʧ��"));
		//VirtualFreeEx(getcurpImageBuffer
		return dwNewEip;

	}

	//���������ض�λ��
	if( !ProcessRelocTable(pImageBuffer,lpNewImgBase) ){
		AfxMessageBox(_T("�����ض�λ��ʧ��"));
		//VirtualFreeEx pImageBuffer
		return dwNewEip;
	}

	//patch����
	WriteProcessMemory(hTargetProcess,lpNewImgBase,pImageBuffer,dwSrcImgSize,&dwReadWrite);
	
	//VirtualFreeEx pImageBuffer

	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pImageBuffer;
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)( (ULONG)pDosHeader + pDosHeader->e_lfanew );
	dwNewEip=pNtHeaders->OptionalHeader.AddressOfEntryPoint+(DWORD)lpNewImgBase;
	return dwNewEip;
}

/*
�������һ��Ҫע�⴦��ת��ĺ���������kernel32��HeapAllocʵ����ת��ntdll��HeapAlloc
*/
bool ProcessImportTable( PVOID pBaseAddr, PVOID lpNewImgBase )
{
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pBaseAddr;
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)( (ULONG)pDosHeader + pDosHeader->e_lfanew );

	//��ȡRtlImageDirectoryEntryToData������ַ
	HMODULE hNtDll = LoadLibrary( _T("ntdll.dll") );
	PRTLIMAGEDIRECTORYENTRYTODATA pRtlImageDirectoryEntryToData = (PRTLIMAGEDIRECTORYENTRYTODATA)GetProcAddress( hNtDll, "RtlImageDirectoryEntryToData" );
	if( pRtlImageDirectoryEntryToData == NULL )
	{
		cout<<"��ȡRtlImageDirectoryEntryToData������ַʧ��"<<endl;
		FreeLibrary( hNtDll );
		return false;
	}
	DWORD dwImportSize = 0;
	PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)pRtlImageDirectoryEntryToData( pBaseAddr, true, IMAGE_DIRECTORY_ENTRY_IMPORT, &dwImportSize );
	if( pImportDescriptor == NULL )
	{
		cout<<"��ȡ�������Ϣ����"<<endl;
		FreeLibrary( hNtDll );
		return false;
	}

	FreeLibrary( hNtDll );


	while( pImportDescriptor->Name != 0 )
	{
		PIMAGE_THUNK_DATA32 pFirstThunkData = (PIMAGE_THUNK_DATA32)( (ULONG)pBaseAddr + pImportDescriptor->FirstThunk);
		PIMAGE_THUNK_DATA32 pOriginalThunkData = (PIMAGE_THUNK_DATA32)( (ULONG)pBaseAddr + pImportDescriptor->OriginalFirstThunk);

		//��ȡdll��
		char *pDllName = (char*)( (ULONG)pBaseAddr + pImportDescriptor->Name );
		TRACE("���������DLL��%s\n",pDllName);

		//ѭ�������dll�е�ÿ���������
		while( (pOriginalThunkData->u1.Ordinal != 0 ) && !( pOriginalThunkData->u1.Ordinal&0x80000000) )
		{
			PIMAGE_IMPORT_BY_NAME pImageImportByName = (PIMAGE_IMPORT_BY_NAME)( (ULONG)pBaseAddr + (ULONG)(pOriginalThunkData->u1.AddressOfData) );
			char *pFuncName = (char*)(&pImageImportByName->Name);
			if ( stricmp(pDllName,"SHLWAPI.DLL")==0 ){
				int a=0;
			}
			DWORD dwFuncAddr = GetFuncAddrFromModule( pDllName, pFuncName );
			TRACE("��������%s  ������ַ��%08X\n",pFuncName,dwFuncAddr);

			if( dwFuncAddr == 0 )
			{
				cout<<"��ȡ���뺯����ַ����"<<endl;
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
		cout<<"��ȡRtlImageDirectoryEntryToData������ַʧ��"<<endl;
		FreeLibrary( hNtDll );
		return 0;
	}
	DWORD dwExportSize = 0;
	PIMAGE_EXPORT_DIRECTORY pExportDescriptor = (PIMAGE_EXPORT_DIRECTORY)pRtlImageDirectoryEntryToData( (PVOID)dwModuleBase, true, IMAGE_DIRECTORY_ENTRY_EXPORT, &dwExportSize );
	if( pExportDescriptor == NULL )
	{
		cout<<"��ȡ������ṹʧ��"<<endl;
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
		cout<<"���Һ���ʧ��"<<endl;
		return 0;
	}
	DWORD dwOridinalName = pNameOrdinalTableBase[dwMid];
	if( dwOridinalName > pExportDescriptor->NumberOfFunctions )
	{
		cout<<"��ȡ�ĺ�����Ŵ���"<<endl;
		return 0;
	}
	PULONG pAddressTableBase = (PULONG)(dwModuleBase + pExportDescriptor->AddressOfFunctions);
	DWORD dwFuncAddr = dwModuleBase + pAddressTableBase[dwOridinalName];

	//////////////////////////////////////////////////////////////////////////
	//����򵥴�����ת������
	if ( IsCharAlphaNumeric(*(PCHAR)dwFuncAddr) && IsCharAlphaNumeric(*(PCHAR)(dwFuncAddr+1))
		&& IsCharAlphaNumeric(*(PCHAR)(dwFuncAddr+2)) && IsCharAlphaNumeric(*(PCHAR)(dwFuncAddr+3)) ){
		CString strText=(LPCTSTR)dwFuncAddr;
		int nPos1=strText.Find('.');
		if ( nPos1!=-1 ){
			CString strDllName=strText.Left(nPos1)+".dll";
			CString strFuncName=strText.Mid(nPos1+1);
			dwFuncAddr=(DWORD)GetProcAddress(GetModuleHandleA(strDllName),strFuncName);
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
			cout<<"�ļ��ڱ���Ϣ������"<<endl;
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
		cout<<"��ȡRtlImageDirectoryEntryToData������ַʧ��"<<endl;
		FreeLibrary( hNtDll );
		return false;
	}
	DWORD dwRelocSize = 0; 
	PIMAGE_BASE_RELOCATION pRelocDescriptor = (PIMAGE_BASE_RELOCATION)pRtlImageDirectoryEntryToData( pImageBuffer, true, IMAGE_DIRECTORY_ENTRY_BASERELOC, &dwRelocSize );
	if( pRelocDescriptor == NULL )
	{
		FreeLibrary( hNtDll );
		cout<<"���ض�λ��"<<endl;
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