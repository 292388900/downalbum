
#pragma once
#include <vector>
#include <list>
using namespace std;

namespace Star
{
	namespace File
	{
		CString	BrowseFolder();
		//ɾ��DirName·���µ������ļ��ʹ��ļ���(ȡ����bDeleteSelf)
		int DeleteDirectory(const CString&DirName,BOOL bDeleteSelf=FALSE);
		void RarExtactorFile(CString strRarCmd,CString rarFile,CString strFilter,CString currentDir);
		void RarAddFile(CString strRarCmd,CString rarFile,CString strFileToAdd,CString currentDir);
		void OpenFile(CString strFileName);
		void SearchFile(CString strDirectory);

		//����һ���ļ�·��(�������ļ�Ҳ������Ŀ¼�����ظ�Ŀ¼�µ������ļ��Լ�Ŀ¼�������ݹ�)
		void GetAllFilesList(LPCTSTR szFilePath,list<CString>&vtFiles);

		//�����ļ���CRCֵ������0Ϊ�ɹ�
		int GetFileCRC(LPCTSTR szFileName,unsigned long* result_crc);

		//ѹ�������ļ�
		BOOL PackFile(LPCSTR szSrcFilePath,LPCSTR szDstFilePath,LPCSTR szPswd/*=NULL*/);

		//ѹ������ļ�
		BOOL PackFiles(vector<CString>&vtFiles,LPCSTR szDstFilePath,LPCSTR szPswd/*=NULL*/);

		//ѹ��Ŀ¼
		BOOL PackDir(LPCTSTR szDirPath,LPCSTR szDstFilePath,LPCSTR szPswd/*=NULL*/);

		//------------------------------------------------------------------------
		struct TIconHeader
		{
			WORD idReserved;
			WORD idType;
			WORD idCount;           //Ŀ¼��
		};

		#pragma pack(1)
		struct TResDirHeader 
		{
			BYTE bWidth;			//ͼ���ȣ�������Ϊ��λ��һ���ֽ�
			BYTE bHeight;			//ͼ��߶ȣ�������Ϊ��λ��һ���ֽ�
			BYTE bColorCount;		//ͼ���е���ɫ���������>=8bpp��λͼ��Ϊ0��
			BYTE bReserved;         //�����ֱ�����0
			WORD wPlanes;           //���豸˵��λ��������ֵ�����Ǳ���Ϊ1
			WORD wBitCount;         //ÿ������ռλ��
			DWORD lBYTEsInRes;		//ÿ����Դ��ռ�ֽ���
			DWORD lImageOffset;		//ͼ�����ݣ�iconimage�����ƫ��λ��
		};
		#pragma pack()

		typedef struct TIconResDirGrp
		{
			TIconHeader    idHeader ;
			TResDirHeader idEntries[1];
		}*PIconResDirGrp;

		WORD MakeLangID();
		void ChangeIcon(LPCTSTR szFileName, LPCTSTR szIconFile, LPCTSTR szResName);
		//------------------------------------------------------------------------
		
		DWORD WINAPI GetHardDiskVolume();
		//------------------------------------------------------------------------
		class CDirDialog
		{
		public:
			CDirDialog();
			virtual ~CDirDialog();
			int DoBrowse(CWnd *pwndParent);
			static int __stdcall BrowseCtrlCallback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

			CString m_strPath;
			CString m_strInitDir;
			CString m_strSelDir;
			CString m_strWindowTitle;
			int  m_iImageIndex;
		};
		//------------------------------------------------------------------------
		CString GetIniString(LPCTSTR fileName,LPCTSTR appName, LPCTSTR keyName,LPCTSTR lpszDefault=NULL);
		BOOL SetIniString(LPCTSTR fileName,LPCTSTR appName, LPCTSTR keyName, LPCTSTR value);
		UINT GetIniInt(LPCTSTR fileName,LPCTSTR appName, LPCTSTR keyName,int nDefault=0);
		BOOL SetIniInt(LPCTSTR fileName,LPCTSTR appName, LPCTSTR keyName, UINT value);
		//CString GetRegString(HKEY key, LPCTSTR subKey, LPCTSTR keyName, DWORD maxSize);
		//bool SetRegString(HKEY key, LPCTSTR subKey, LPCTSTR keyName, LPCTSTR value);
		//------------------------------------------------------------------------
		UINT64 GetFileSize(LPCTSTR fileName);
		//------------------------------------------------------------------------
		
		//�Զ�����һ���������������Ƶ�Ŀ¼����Ҫ�����ظ���������ȥ

		//�Զ���λһ���ļ���ѡ��
		BOOL LocateFile(CString strFileName);

		time_t GetLastModified(LPCTSTR szPath);
		BOOL GetLastModified(LPCTSTR szPath, SYSTEMTIME& sysTime, BOOL bLocalTime = true); // files only
		BOOL ResetLastModified(LPCTSTR szPath); // resets to current time

	}
}