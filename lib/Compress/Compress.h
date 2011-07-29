#pragma once

// ѹ�������С.
#define BUFLEN      16384

// ѹ���ļ�����,����WPARAM��ʾ��ѹ������������,LPARAM�����ļ���.
#define WM_COMMPRESS_PROGRESS		(WM_USER + 2004)

// ѹ���ļ����,����WPARAM��ʾѹ���ļ�����,LPARAMΪѹ�����·��.
#define WM_COMMPRESS_COMPLETE		(WM_USER + 2005)

// ѹ���ļ�������Ϣ,����WPARAM��ʾ�ļ�����.
#define WM_COMMPRESS_FILE_COUNT		(WM_USER + 2006)


class CCompress
{
public:
    typedef enum { emGz, emZip, emAuto } outType;

public:
    CCompress(void);
    ~CCompress(void);

public:
    // �õ��ļ�crc.
    int GetFileCrc(const char* filenameinzip, void*buf, unsigned long size_buf, unsigned long* result_crc);
    // ��·�������ļ��к���.
    void ModifyPathSpec(TCHAR* szDst, BOOL bAddSpec);
    BOOL CreateDirectoryNested(LPCTSTR lpszDir);
    BOOL CreateFileNested(LPCTSTR lpszFile, BOOL bOverride = TRUE);

public:
    // ѹ��һ���ļ���gz��ʽ.
    bool CompressGz(const char* inFile, const char* outFile);

    // ѹ��һ���ļ���zip��ʽ.
    bool CompressZip(const char* inFile, const char* outFile);

    // ���������ļ���λ��.
    void SetInPath(CString& strInPath) { m_strInPath = strInPath; }

    // ��������ļ���λ��.
    void SetOutPath(CString& strOutPath) { m_strOutPath = strOutPath; }

    // ����ѹ���ļ��б�.
    void SetFileList(ListXml& listFile) { m_lstFile = listFile; }

    // ѹ����ʽΪzipʱ,������ѹ������.
    void SetZipPassword(std::string& strPwd) { m_strZipPwd = (m_outType == emZip ? strPwd : ""); }

    // ���������ʽ.
    void SetOutType(outType oType) { m_outType = oType; }

public:
    // ��ִ�г���.
    HRESULT Execute(DWORD_PTR dwParam, HANDLE hObject);

private:
    outType m_outType;		// ����ļ���ʽ.
    CString m_strOutPath;	// ����ļ���.
    CString m_strInPath;	// �����ļ���.
    std::string m_strZipPwd;// zipѹ����ʽʱ������.	
    ListXml m_lstFile;		// ��Ҫѹ�����ļ�.
    DWORD m_dwFileCount;	// �ļ�����.
    HWND m_hMsgWnd;			// ��Ϣ֪ͨ���ھ��.
};
