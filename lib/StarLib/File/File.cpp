#include "stdafx.h"
#include <afx.h>
#include <afxwin.h>
#include <ShlObj.h>
#include <sys/stat.h>
#include <sys/utime.h>
#include "File.h"

/*------------------------------------------------------------------------
[7/24/2009 xiaolin]
˵��:  ���û�ѡ��һ���ļ��л�Ŀ¼,����ѡ���·��.
------------------------------------------------------------------------*/
CString	Star::File::BrowseFolder()
{
	BROWSEINFO bi;

	ZeroMemory(&bi, sizeof(bi));
	bi.hwndOwner = NULL;//m_hwnd
	bi.ulFlags   = BIF_RETURNONLYFSDIRS;

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	BOOL bRet = FALSE;
	TCHAR szFolder[MAX_PATH*2];
	szFolder[0] = _T('\0');

	if (pidl)
	{
		if (SHGetPathFromIDList(pidl, szFolder))  
			bRet = TRUE;
		IMalloc *pMalloc = NULL;
		if (SUCCEEDED(SHGetMalloc(&pMalloc)) && pMalloc)
		{ 
			pMalloc->Free(pidl); 
			pMalloc->Release();
		}
	}
	return szFolder;
}

/************************************************************************
[3/23/2009 xiaolin]
��������DeleteDirectory
������  DirName:Ŀ¼��(��Ҫ�����\);bDeleteSelf:�����Ƿ�����Ŀ¼Ҳɾ��.
����ֵ����Ŀ¼�µ��ļ�����
˵����	ɾ��DirName·���µ������ļ��ʹ��ļ���(ȡ����bDeleteSelf)��
������:	
************************************************************************/
int Star::File::DeleteDirectory(const CString&DirName,BOOL bDeleteSelf/*=FALSE*/) 
{ 
	int i=0;
	CString strDir;
	CString strExistFile;
	CFileFind finder; 

	strDir.Format("%s*.*",DirName);
	BOOL bWorking = finder.FindFile(strDir); 
	while (bWorking){
		bWorking = finder.FindNextFile(); 
		//skip . and ..,skip if IsDirectory
		if (finder.IsDots()){
			continue;
		}else if (finder.IsDirectory()){
			DeleteDirectory(finder.GetFilePath()+"\\",true);
		}else { 
			i++;
			DeleteFile(finder.GetFilePath());
		} 
	}
	finder.Close();
	if (bDeleteSelf){
		RemoveDirectory(DirName);
	}
	return i; 
}

/*------------------------------------------------------------------------
[7/24/2009 xiaolin]
strRarCmd:	rar.exe����ȫ·����,��:C:\Program Files\WinRAR\Rar.exe
rarFile:	����ѹ��.rar����.zip�ļ�
strFilter:	��ѹ����
currentDir:	��ǰ·��,rar���ļ���ѹ�����Ŀ¼��.

˵��: 
WinRAR e *.rar *.doc
�ڵ�ǰ�ļ��У���ȫ���� RAR ѹ���ļ���ѹ���е� *.doc �ļ�����ǰ�ļ��� 

winrar x "D:\����ɱ��.zip" *.html
��D:\����ɱ��.zip�ļ��н�ѹhtml�ļ�����ǰ�ļ�����
------------------------------------------------------------------------*/
void Star::File::RarExtactorFile(CString strRarCmd,CString rarFile,CString strFilter,CString currentDir)
{
	CString strCmdLine="\""+strRarCmd+"\" x -inul -ibck \""+rarFile+"\" "+strFilter;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	si.wShowWindow=SW_HIDE;
	ZeroMemory( &pi, sizeof(pi) );

	// Start the child process. 
	if( !CreateProcess( NULL,		// No module name (use command line). 
		(char*)(LPCTSTR)strCmdLine, // Command line. 
		NULL,						// Process handle not inheritable. 
		NULL,						// Thread handle not inheritable. 
		true,						// Set handle inheritance to FALSE. 
		CREATE_NO_WINDOW   ,        // No creation flags. 
		NULL,						// Use parent's environment block. 
		(char*)(LPCTSTR)currentDir, // Use parent's starting directory. 
		&si,						// Pointer to STARTUPINFO structure.
		&pi )						// Pointer to PROCESS_INFORMATION structure.
		) 
	{
		TRACE( "CreateProcess failed (%d).\n", GetLastError() );
		return;
	}

	// Wait until child process exits.
	WaitForSingleObject( pi.hProcess, INFINITE );

	// Close process and thread handles. 
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );
}

/*------------------------------------------------------------------------
[7/24/2009 xiaolin]
strRarCmd:	rar.exe����ȫ·����,��:C:\Program Files\WinRAR\Rar.exe
rarFile:	.rar����.zip�ļ�
strFileToAdd:Ҫ��ӵ��ļ�
currentDir:	��ǰ·��

˵��: 
WinRAR a help *.hlp
�ӵ�ǰ�ļ������ȫ�� *.hlp �ļ���ѹ���ļ� help.rar ��

winrar a "D:\����ɱ��.zip" "C:\1.htm"
��C:\1.htm��ӵ�D:\����ɱ��.zip��
------------------------------------------------------------------------*/
void Star::File::RarAddFile(CString strRarCmd,CString rarFile,CString strFileToAdd,CString currentDir)
{
	CString strCmdLine="\""+strRarCmd+"\" a \""+rarFile+"\" \""+strFileToAdd+"\"";

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	si.wShowWindow=SW_HIDE;
	ZeroMemory( &pi, sizeof(pi) );

	// Start the child process. 
	if( !CreateProcess( NULL,		// No module name (use command line). 
		(char*)(LPCTSTR)strCmdLine, // Command line. 
		NULL,						// Process handle not inheritable. 
		NULL,						// Thread handle not inheritable. 
		true,						// Set handle inheritance to FALSE. 
		CREATE_NO_WINDOW,			// No creation flags. 
		NULL,						// Use parent's environment block. 
		(char*)(LPCTSTR)currentDir,	// Use parent's starting directory. 
		&si,						// Pointer to STARTUPINFO structure.
		&pi )						// Pointer to PROCESS_INFORMATION structure.
		) 
	{
		TRACE( "CreateProcess failed (%d).\n", GetLastError() );
		return;
	}

	// Wait until child process exits.
	WaitForSingleObject( pi.hProcess, INFINITE );

	// Close process and thread handles. 
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );
}


/*------------------------------------------------------------------------
[7/24/2009 xiaolin]
˵��: 
------------------------------------------------------------------------*/
void Star::File::OpenFile(CString strFileName)
{
/*
	CFileException e;
	CFile file;
	if( file.Open(strFileName,CFile::modeReadWrite,&e) )
	{
		//...
		file.Close();
	}
	else
	{
		//e.ReportError();
	}
*/
}

/*------------------------------------------------------------------------
[7/24/2009 xiaolin]
˵��: 
------------------------------------------------------------------------*/
void Star::File::SearchFile(CString strDirectory)
{
/*
	CFileFind finder;
	BOOL bWorking = finder.FindFile(strDirectory+"*.*"); 
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		//skip . and ..
		if (finder.IsDots()) 
			continue; 
		else if (finder.IsDirectory())
		{
			SearchFile(finder.GetFilePath()+"\\");
		}
		else 
		{ 
			//strFileName=finder.GetFileName();
		}
	}
	finder.Close();*/

}



//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
WORD Star::File::MakeLangID()
{
	return (SUBLANG_ENGLISH_US << 10) | LANG_ENGLISH;
}
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

/*------------------------------------------------------------------------
[7/24/2009 xiaolin]
˵��:API����exe�ļ�ͼ��,��û�м����ļ��Ƿ���PE�ļ�.
ʾ��:ChangeIcon("c:\\test.exe","c:\\test.ico","AyIcon");
------------------------------------------------------------------------*/
void Star::File::ChangeIcon(char* szFileName,char* szIconFile, char* szResName)
{
	int i,FileGrpSize;
	DWORD dwFileSize,dwBytesRead;
	void * filemem,*p;
	PIconResDirGrp FileGrp;
	HANDLE hFile,hUpdateRes;

	//open the icon file 
	hFile=CreateFile(szIconFile,GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN, 0) ; 
	if (hFile==INVALID_HANDLE_VALUE)
	{
		TRACE("Failed open Icon File!");
		return;
	}
	//get the file size
	dwFileSize = ::GetFileSize(hFile,NULL);
	filemem=malloc(dwFileSize);
	//read file to memory
	ReadFile(hFile,filemem, dwFileSize,&dwBytesRead,NULL) ;
	CloseHandle(hFile);

	//assume the TIconResDirGrp STRUCT
	FileGrp=PIconResDirGrp(filemem);
	//get Icon_Header size
	FileGrpSize=sizeof(TIconResDirGrp)+(FileGrp->idHeader.idCount-1)*sizeof(TResDirHeader);
	//begin to change the resource
	hUpdateRes=BeginUpdateResource(szFileName, false);
	//change all frames'resource
	for(i=0;i<FileGrp->idHeader.idCount;i++)
	{
		p=(void *)((DWORD)filemem+FileGrp->idEntries[i].lImageOffset);
		//change every frame
		UpdateResource(hUpdateRes,RT_ICON,MAKEINTRESOURCE(FileGrp->idEntries[i].lImageOffset),
			MakeLangID(), p,    FileGrp->idEntries[i].lBYTEsInRes);  
	}
	//update header information
	UpdateResource(hUpdateRes,RT_GROUP_ICON, szResName,
		MakeLangID(), FileGrp, FileGrpSize);                               
	EndUpdateResource(hUpdateRes, false);
	free(filemem);  
}

DWORD WINAPI Star::File::GetHardDiskVolume()
{
	DWORD nMaxLength = 0;
	DWORD nVolumeNum;
	DWORD nFlags;
	::GetVolumeInformation("C:\\", NULL, 0, 
		&nVolumeNum, &nMaxLength, &nFlags, NULL, 0);

	return nVolumeNum;
}

//------------------------------------------------------------------------
Star::File::CDirDialog::CDirDialog()
{
	m_strWindowTitle = _T("ѡ��Ŀ���ļ���");
}

Star::File::CDirDialog::~CDirDialog()
{
}

// Callback function called by SHBrowseForFolder's browse control
// after initialization and when selection changes
int __stdcall Star::File::CDirDialog::BrowseCtrlCallback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	CDirDialog* pDirDialogObj = (CDirDialog*)lpData;
	if (uMsg == BFFM_INITIALIZED )
	{
		if( ! pDirDialogObj->m_strSelDir.IsEmpty() )
			::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)(LPCTSTR)(pDirDialogObj->m_strSelDir));
	}
	::SendMessage(hwnd, BFFM_ENABLEOK, 0, TRUE);
	return 0;
}

BOOL Star::File::CDirDialog::DoBrowse(CWnd *pwndParent/*=NULL*/)
{

	if( ! m_strSelDir.IsEmpty() )
	{
		m_strSelDir.TrimRight();
		if( m_strSelDir.Right(1) == "\\" || m_strSelDir.Right(1) == "//" )
			m_strSelDir = m_strSelDir.Left(m_strSelDir.GetLength() - 1);
	}

	LPMALLOC pMalloc;
	if (SHGetMalloc (&pMalloc)!= NOERROR)
		return FALSE;

	BROWSEINFO bInfo;
	LPITEMIDLIST pidl;
	ZeroMemory ( (PVOID) &bInfo,sizeof (BROWSEINFO));

	if (!m_strInitDir.IsEmpty ())
	{
		OLECHAR       olePath[MAX_PATH];
		ULONG         chEaten;
		ULONG         dwAttributes;
		HRESULT       hr;
		LPSHELLFOLDER pDesktopFolder;
		//
		// Get a pointer to the Desktop's IShellFolder interface.
		//
		if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
		{
			//
			// IShellFolder::ParseDisplayName requires the file name be in Unicode.
			//
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_strInitDir.GetBuffer(MAX_PATH), -1,
				olePath, MAX_PATH);

			m_strInitDir.ReleaseBuffer (-1);
			//
			// Convert the path to an ITEMIDLIST.
			//
			hr = pDesktopFolder->ParseDisplayName(NULL,
				NULL,
				olePath,
				&chEaten,
				&pidl,
				&dwAttributes);
			if (FAILED(hr))
			{
				pMalloc ->Free (pidl);
				pMalloc ->Release ();
				return FALSE;
			}
			bInfo.pidlRoot = pidl;

		}
	}
	bInfo.hwndOwner = pwndParent == NULL ? NULL : pwndParent->GetSafeHwnd();
	bInfo.pszDisplayName = m_strPath.GetBuffer (MAX_PATH);
	bInfo.lpszTitle = m_strWindowTitle;
	bInfo.ulFlags = BIF_RETURNFSANCESTORS
		| BIF_RETURNONLYFSDIRS
		| (FALSE/*m_bStatus*/ ? BIF_STATUSTEXT : 0);

	bInfo.lpfn = BrowseCtrlCallback;  // address of callback function
	bInfo.lParam = (LPARAM)this;      // pass address of object to callback function

	if ((pidl = ::SHBrowseForFolder(&bInfo)) == NULL)
	{
		return FALSE;
	}
	m_strPath.ReleaseBuffer();
	m_iImageIndex = bInfo.iImage;

	if (::SHGetPathFromIDList(pidl, m_strPath.GetBuffer(MAX_PATH)) == FALSE)
	{
		pMalloc ->Free(pidl);
		pMalloc ->Release();
		return FALSE;
	}

	m_strPath.ReleaseBuffer();

	pMalloc ->Free(pidl);
	pMalloc ->Release();

	return TRUE;
}
//------------------------------------------------------------------------
CString Star::File::GetIniString(LPCTSTR fileName,LPCTSTR appName, LPCTSTR keyName,LPCTSTR lpszDefault)
{
	TCHAR szTemp[MAX_PATH*2];
	DWORD size = GetPrivateProfileString(appName, keyName, lpszDefault, szTemp, sizeof(szTemp), fileName);
	return szTemp;
}

bool Star::File::SetIniString(LPCTSTR fileName,LPCTSTR appName, LPCTSTR keyName, LPCTSTR value)
{
	return WritePrivateProfileString(appName, keyName, value, fileName);
}

UINT Star::File::GetIniInt(LPCTSTR fileName,LPCTSTR appName, LPCTSTR keyName,int nDefault)
{
	return GetPrivateProfileInt(appName, keyName, nDefault, fileName);
}

bool Star::File::SetIniInt(LPCTSTR fileName,LPCTSTR appName, LPCTSTR keyName, UINT value)
{
	CString strValue;
	strValue.Format("%d", value);
	return WritePrivateProfileString(appName, keyName, strValue, fileName);
}

// CString Star::File::GetRegString(HKEY key, LPCTSTR subKey, LPCTSTR keyName, DWORD maxSize)
// {
// 	HKEY hKey;
// 	DWORD nSize = maxSize+1;
// 	TCHAR *pStr = new TCHAR(nSize);
// 	ScopeGuard sg = MakeArrayGuard(pStr);
// 	if(ERROR_SUCCESS != RegOpenKeyEx(key, subKey, 0, KEY_READ, &hKey))
// 		return _T("");
// 	if(ERROR_SUCCESS != RegQueryValueEx(hKey, keyName, NULL, NULL, (LPBYTE)pStr, &nSize))
// 	{
// 		RegCloseKey(hKey);
// 		return _T("");
// 	}
// 	RegCloseKey(hKey);
// 	return TString(pStr);
// }
// 
// bool Star::File::SetRegString(HKEY key, LPCTSTR subKey, LPCTSTR keyName, LPCTSTR value)
// {
// 	HKEY hKey;
// 	if(ERROR_SUCCESS != RegOpenKeyEx(key, subKey, 0, KEY_WRITE, &hKey))
// 		return false;
// 
// 	if(ERROR_SUCCESS != RegSetValueEx(hKey, keyName, NULL, REG_SZ, (LPBYTE)value, _tcslen(value)))
// 	{
// 		RegCloseKey(hKey);
// 		return false;
// 	}
// 
// 	RegCloseKey(hKey);
// 	return true;
// }
//------------------------------------------------------------------------
UINT64 Star::File::GetFileSize(LPCTSTR fileName)
{
	HANDLE hFile;
	LARGE_INTEGER size;
	hFile = CreateFile(fileName, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == hFile)
		return -1;
	size.LowPart = ::GetFileSize(hFile, (LPDWORD)&size.HighPart);
	CloseHandle(hFile);
	if(INVALID_FILE_SIZE == size.LowPart)
		return -1;
	return size.QuadPart;
}
//------------------------------------------------------------------------


//�Զ���λһ���ļ���ѡ��
BOOL Star::File::LocateFile(CString strFileName)
{
	BOOL bSuccess=FALSE;

	if ( GetFileAttributes(strFileName)==-1 ){	//�ļ������ڴ������ڵ�Ŀ¼
		CString strFolder="";
		int nPos=strFileName.ReverseFind('\\');
		if ( nPos!=-1 ){
			strFolder=strFileName.Left(nPos);
		}

		if ( GetFileAttributes(strFolder)==-1 ){
			//AfxMessageBox("�����ļ������ڣ������Ѿ���ɾ���ˣ�\n"+strFileName);
		}else{
			ShellExecute(NULL,"open","explorer.exe",strFolder,NULL,SW_NORMAL);
		}
	}else{
		CString strCmdLine;
		strCmdLine.Format( "/select, \"%s\"", strFileName );
		ShellExecute(NULL,"open","explorer.exe",strCmdLine,NULL,SW_NORMAL);
	}

	return bSuccess;
}


time_t Star::File::GetLastModified(LPCTSTR szPath)
{
	struct _stat st;

	if (!szPath || _tstat64i32(szPath, &st) != 0)
		return 0;

	// files only
	if ((st.st_mode & _S_IFDIR) == _S_IFDIR)
		return 0;

	return st.st_mtime;
}

BOOL Star::File::GetLastModified(LPCTSTR szPath, SYSTEMTIME& sysTime, BOOL bLocalTime)
{
	ZeroMemory(&sysTime, sizeof(SYSTEMTIME));

	DWORD dwAttr = ::GetFileAttributes(szPath);

	// files only
	if (dwAttr == 0xFFFFFFFF)
		return false;

	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile((LPTSTR)szPath, &findFileData);

	if (hFind == INVALID_HANDLE_VALUE)
		return FALSE;

	FindClose(hFind);

	FILETIME ft = findFileData.ftLastWriteTime;

	if (bLocalTime)
		FileTimeToLocalFileTime(&findFileData.ftLastWriteTime, &ft);

	FileTimeToSystemTime(&ft, &sysTime);
	return true;
}

BOOL Star::File::ResetLastModified(LPCTSTR szPath)
{
	::SetFileAttributes(szPath, FILE_ATTRIBUTE_NORMAL);

	return (_tutime(szPath, NULL) == 0);
}