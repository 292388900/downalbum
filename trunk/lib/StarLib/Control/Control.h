
#pragma once
#include <afx.h>
#include <fstream>
#include <afxwin.h>
#include <afxcmn.h>

namespace Star
{
	namespace Control
	{

/*˵����һ����־��*/


class CEditLog : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CEditLog)

public:
	CEditLog();
	virtual ~CEditLog();

	std::ofstream outfile;
	bool	InitEditLog(RECT&recv,CWnd* pwnd,UINT nID,char *path);		//������
	


	bool write(char *str,COLORREF color=RGB(0,0,0));							
	bool write(int l,COLORREF color=RGB(0,0,0));
	bool write(double f,COLORREF color=RGB(0,0,0));
	bool write(CString& str,COLORREF color=RGB(0,0,0));

	bool EndLine();



	CEditLog& operator<<(char*str);
	CEditLog& operator<<(CString&str);
	CEditLog& operator<<(int i);
	CEditLog& operator<<(double f);
	CEditLog& operator<<(CEditLog& (*a)(CEditLog&));




	bool RecordTimeBool;							//�Ƿ��¼ʱ��
	bool filebool;									//�Ƿ��¼���ļ�
	int LineLen;									//��־��ʾ��������

private:

	bool stratbool;									//�Ƿ��ʼ������־

	char strtime[50];								//GetTime ����ʱ����ַ�

	char *  CEditLog::GetTime();					//��ʱ��ת�ַ�

	bool StartLine;									//�Ƿ���һ�еĿ�ͷ

	DWORD ID;										//ID��

	bool SetEditText(char *str,COLORREF color);		//�����ı�

	bool WriteFile(char *str);						//д���ļ�

	bool WriteHead(COLORREF color);					//д���ļ�ͷ

	COLORREF Color;									//��ɫ



protected:
	DECLARE_MESSAGE_MAP()
};

	CEditLog& endl(CEditLog&);
	
	}
}