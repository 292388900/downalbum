#include "StdAfx.h"
#include "Compress.h"

#include <md5.h>


CCompress::CCompress(void)
: m_outType(emAuto)
{
}

CCompress::~CCompress(void)
{
}

void CCompress::ModifyPathSpec(TCHAR* szDst, BOOL bAddSpec)
{  
    int nLen = lstrlen(szDst);  
    TCHAR ch = szDst[nLen - 1]; 

    if( (ch == _T('\\')) || (ch == _T('/')))  
    {  
        if(!bAddSpec)  
        {  
            szDst[nLen - 1] = _T('\0');  
        }  
    }  
    else  
    {  
        if(bAddSpec)  
        {  
            szDst[nLen] = _T('\\');  
            szDst[nLen + 1] = _T('\0');  
        }  
    }  
}  

BOOL CCompress::CreateDirectoryNested(LPCTSTR lpszDir)
{
    // �滻·���е�б��.
    CString strPath = lpszDir;

    strPath.Replace(_T('/'), _T('\\'));

    if(::PathIsDirectory(strPath))
        return TRUE;

    TCHAR szPreDir[MAX_PATH];
    lstrcpy(szPreDir, strPath);

    // ȷ��·��ĩβû�з�б��.
    ModifyPathSpec(szPreDir, FALSE);  

    // ��ȡ�ϼ�Ŀ¼.
    BOOL bGetPreDir = ::PathRemoveFileSpec(szPreDir);
    if(!bGetPreDir)
        return FALSE;

    // ����ϼ�Ŀ¼������,��ݹ鴴���ϼ�Ŀ¼.
    if(!::PathIsDirectory(szPreDir))
    {
        CreateDirectoryNested(szPreDir);
    }

    // �����ļ���.
    return ::CreateDirectory(strPath, NULL);
}

BOOL CCompress::CreateFileNested(LPCTSTR lpszFile, BOOL bOverride/* = TRUE*/)
{  
    if(::PathFileExists(lpszFile))
        return TRUE;  

    TCHAR szDir[MAX_PATH];  
    _tcscpy_s(szDir, lpszFile);

    //��ȡ�ļ�Ŀ¼  
    BOOL bGetDir = ::PathRemoveFileSpec(szDir);  
    if(!bGetDir)
        return FALSE;  

    //�����ļ�Ŀ¼  
    if(!CreateDirectoryNested(szDir))
        return FALSE;  

    // �����ļ�.
    DWORD dwCreate = bOverride ? CREATE_ALWAYS : OPEN_ALWAYS;  
    HANDLE hFile = ::CreateFile(lpszFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, 
        dwCreate, FILE_FLAG_BACKUP_SEMANTICS, NULL);

    if(hFile == INVALID_HANDLE_VALUE)
        return FALSE;

    ::CloseHandle(hFile);

    return TRUE;
}

// ѹ��һ���ļ���gz��ʽ.
bool CCompress::CompressGz(const char* inFile, const char* outFile)
{
#ifdef UNICODE
    CreateFileNested(CA2W(outFile));
#else
    CreateFileNested(outFile);
#endif // UNICODE

    FILE * in = fopen(inFile, "rb");
    if (in)
    {
        gzFile out;
        char outmode[20];

        strcpy(outmode, "wb6f");
        out = gzopen(outFile, outmode);
        if (out)
        {
            char buf[BUFLEN];
            int len;

            do 
            {
                len = (int)fread(buf, 1, sizeof(buf), in);
                if (ferror(in))
                    return false;

                if (len == 0)
                    break;

                if (gzwrite(out, buf, (unsigned)len) != len)
                    break;

            } while (1);

            fclose(in);
            if (gzclose(out) != Z_OK)
                return false;

            return true;
        }
    }
    return false;
}

int CCompress::GetFileCrc(const char* filenameinzip, void*buf, unsigned long size_buf, unsigned long* result_crc)
{
    unsigned long calculate_crc=0;
    int err=ZIP_OK;
    FILE * fin = fopen(filenameinzip, "rb");
    unsigned long size_read = 0;
    unsigned long total_read = 0;
    if (fin==NULL)
    {
        err = ZIP_ERRNO;
    }

    if (err == ZIP_OK)
        do
        {
            err = ZIP_OK;
            size_read = (int)fread(buf,1,size_buf,fin);
            if (size_read < size_buf)
                if (feof(fin)==0)
                {
                    ATLTRACE("error in reading %s\n", filenameinzip);
                    err = ZIP_ERRNO;
                }

                if (size_read>0)
                    calculate_crc = crc32(calculate_crc,(const Bytef *)buf,size_read);
                total_read += size_read;

        } while ((err == ZIP_OK) && (size_read>0));

        if (fin)
            fclose(fin);

        *result_crc=calculate_crc;
        ATLTRACE("file %s crc %x\n",filenameinzip,calculate_crc);
        return err;
}

// ѹ��һ���ļ���zip��ʽ.
bool CCompress::CompressZip(const char* inFile, const char* outFile)
{
    zipFile zf;
    int errclose;
    zip_fileinfo zi = {0};

#ifdef UNICODE
    CreateFileNested(CA2W(outFile));
#else
    CreateFileNested(outFile);
#endif // UNICODE

    zf = zipOpen(outFile, 2);
    if (!zf)
        return false;

    // �����ļ�ʱ��.
    FILETIME ftLocal;
    HANDLE hFind;
    WIN32_FIND_DATA ff32;
#ifdef UNICODE
    hFind = FindFirstFile(CA2W(inFile), &ff32);
#else
    hFind = FindFirstFile(inFile, &ff32);
#endif // UNICODE
    if (hFind != INVALID_HANDLE_VALUE)
    {
        FileTimeToLocalFileTime(&(ff32.ftLastWriteTime),&ftLocal);
        FileTimeToDosDateTime(&ftLocal,((LPWORD)&zi.dosDate)+1,((LPWORD)&zi.dosDate)+0);
        FindClose(hFind);
    }

    int err = 0;
    unsigned long crcFile = 0;
    int size_buf = BUFLEN;
    int size_read = 0;
    void* buf = (void*)malloc(size_buf);
    const char* password = NULL;
    FILE* fin = NULL;

    // ��������.
    if (m_strZipPwd != "")
    {
        err = GetFileCrc(inFile, buf, size_buf, &crcFile);
        password = m_strZipPwd.c_str();
    }

    err = zipOpenNewFileInZip3(zf, inFile, &zi,
        NULL,0,NULL,0,NULL /* comment*/,
        Z_DEFLATED,
        Z_DEFAULT_COMPRESSION,0,
        /* -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY, */
        -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
        password, crcFile);
    if (err != ZIP_OK)
    {
        zipClose(zf,NULL);
        return false;
    }
    else
    {
        fin = fopen(inFile, "rb");
        if (!fin)
        {
            zipClose(zf,NULL);
            return false;
        }
    }

    if (err == ZIP_OK)
    {
        do
        {
            err = ZIP_OK;
            size_read = (int)fread(buf, 1, size_buf, fin);
            if (size_read < size_buf)
                if (feof(fin)==0)
                {
                    ATLTRACE("error in reading %s\n", inFile);
                    err = ZIP_ERRNO;
                }

                if (size_read>0)
                {
                    err = zipWriteInFileInZip (zf,buf,size_read);
                    if (err<0)
                    {
                        ATLTRACE("error in writing %s in the zipfile\n",
                            inFile);
                    }
                }
        } while ((err == ZIP_OK) && (size_read>0));

        if (fin)
            fclose(fin);

        if (err<0)
            err=ZIP_ERRNO;
        else
        {
            err = zipCloseFileInZip(zf);
            if (err!=ZIP_OK)
                ATLTRACE("error in closing %s in the zipfile\n",
                inFile);
        }
        errclose = zipClose(zf,NULL);
        if (errclose != ZIP_OK)
        {
            ATLTRACE("error in closing %s\n", inFile);
            return false;
        }

        return true;
    }

    return false;
}

// ��ִ�г���.
HRESULT CCompress::Execute(DWORD_PTR dwParam, HANDLE hObject)
{
    // ��ɺ����˯��,���ٹ���.
    m_hMsgWnd = (HWND)dwParam;

    // �õ���ǰ·��.
    CString strCurDirectory;
    CString strBackupDirectory;
    CString strFileDir;
    CString strFileName;

    strCurDirectory.Preallocate(MAX_PATH);
    ::GetCurrentDirectory(MAX_PATH, strCurDirectory.GetBuffer(0));
    strCurDirectory.ReleaseBuffer();
    strBackupDirectory = strCurDirectory;

    std::string inPath, outPath;
    // if (m_strInPath != _T("") && m_strOutPath != _T("") && m_lstFile.size() != 0)
    {
        // ���'\'
        // 			if (m_strInPath.Right(1) != _T('\\'))
        // 				m_strInPath = m_strInPath + _T('\\');
        if (m_strOutPath.Right(1) != _T('\\'))
            m_strOutPath = m_strOutPath + _T('\\');
#ifdef UNICODE
        //			inPath = CW2A(m_strInPath.GetBuffer(0), CP_ACP);
        outPath = CW2A(m_strOutPath.GetBuffer(0), CP_ACP);
#else
        //			inPath = m_strInPath.GetBuffer(0);
        outPath = m_strOutPath.GetBuffer(0);
#endif // UNICODE

        std::string inFile, outFile, strInName, strOutName;
        int nPos = 0;

        m_dwFileCount = m_lstFile.size();
        ::PostMessage(m_hMsgWnd, WM_COMMPRESS_FILE_COUNT, m_dwFileCount, 0);
        m_dwFileCount = 0;

        for (ListXml::iterator it = m_lstFile.begin(); it != m_lstFile.end(); it++)
        {
            strFileDir = it->strFullPath; // m_strInPath + it->strName;			// �õ������ļ���(·��+�ļ���).
            nPos = strFileDir.ReverseFind(_T('\\'));		// �õ����һ����б��λ��.
            strCurDirectory = strFileDir.Left(nPos + 1);	// ���һ����б��λ��ǰ����ַ���ʵ���Ͼ�������·��.
            strFileName = strFileDir.Right(strFileDir.GetLength() - (nPos + 1)); // ���һ����б��λ�ú�����ַ���ʵ���Ͼ����ļ���.
            ::SetCurrentDirectory(strCurDirectory.GetBuffer(0));	// ����Ϊ��ǰ�ļ���.
#ifdef UNICODE
            strInName = CW2A(strFileName.GetBuffer(0), CP_ACP);
            strOutName = CW2A(it->strName.GetBuffer(0), CP_ACP);
#else
            strInName = strFileName.GetBuffer(0);
            strOutName = it->strName.GetBuffer(0);
#endif // UNICODE

            inFile = strInName;
            outFile = outPath + strOutName;

            if (m_outType == emAuto)	// �Զ����,���б���ָ�������.
            {
                if (it->strCompress == _T("zip"))		// ʹ��zipѹ��.
                {
                    outFile += ".zip";
                    CompressZip(inFile.c_str(), outFile.c_str());
                }
                else if (it->strCompress == _T("gz"))	// ʹ��gzѹ��.
                {
                    outFile += ".gz";
                    CompressGz(inFile.c_str(), outFile.c_str());
                }
                else		// ��ʹ��ѹ��ʱ,ֱ�Ӹ��ƹ�ȥ.
                {
                    CString strNewName = m_strOutPath + it->strName;
                    CreateFileNested(strNewName);
                    CopyFile(strFileName, strNewName, FALSE);
                }					
            }
            else if (m_outType == emGz)	// ָ�����ΪGZ��ʽ.
            {
                outFile += ".gz";
                CompressGz(inFile.c_str(), outFile.c_str());
            }
            else if (m_outType == emZip) // ָ�����ΪZIP��ʽ.
            {
                outFile += ".zip";
                CompressZip(inFile.c_str(), outFile.c_str());
            }

            m_dwFileCount++;
            ::PostMessage(m_hMsgWnd, WM_COMMPRESS_PROGRESS, (WPARAM)it->strName.GetBuffer(0), m_dwFileCount);
        }
    }

    ::SetCurrentDirectory(strBackupDirectory.GetBuffer(0));
    //  ����ѹ�������Ϣ.
    ::PostMessage(m_hMsgWnd, WM_COMMPRESS_COMPLETE, (WPARAM)m_dwFileCount, (LPARAM)m_strOutPath.GetBuffer(0));

    return S_FALSE;
}
