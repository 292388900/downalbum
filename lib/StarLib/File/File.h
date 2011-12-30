
#pragma once

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
		void ChangeIcon(char* szFileName,char* szIconFile, char* szResName);
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
		bool SetIniString(LPCTSTR fileName,LPCTSTR appName, LPCTSTR keyName, LPCTSTR value);
		UINT GetIniInt(LPCTSTR fileName,LPCTSTR appName, LPCTSTR keyName,int nDefault=0);
		bool SetIniInt(LPCTSTR fileName,LPCTSTR appName, LPCTSTR keyName, UINT value);
		//CString GetRegString(HKEY key, LPCTSTR subKey, LPCTSTR keyName, DWORD maxSize);
		//bool SetRegString(HKEY key, LPCTSTR subKey, LPCTSTR keyName, LPCTSTR value);
		//------------------------------------------------------------------------
		UINT64 GetFileSize(LPCTSTR fileName);
		//------------------------------------------------------------------------
		
		//�Զ�����һ���������������Ƶ�Ŀ¼����Ҫ�����ظ���������ȥ

		//�Զ���λһ���ļ���ѡ��
		BOOL LocateFile(CString strFileName);
	}
}