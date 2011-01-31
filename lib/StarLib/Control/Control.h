
#pragma once
#include <afx.h>
#include <fstream>
#include <afxwin.h>
#include <afxcmn.h>

namespace Star
{
	namespace Control
	{

/*说明：一个日志类*/


class CEditLog : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CEditLog)

public:
	CEditLog();
	virtual ~CEditLog();

	std::ofstream outfile;
	bool	InitEditLog(RECT&recv,CWnd* pwnd,UINT nID,char *path);		//创建类
	


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




	bool RecordTimeBool;							//是否记录时间
	bool filebool;									//是否记录到文件
	int LineLen;									//日志显示最大的行数

private:

	bool stratbool;									//是否初始化了日志

	char strtime[50];								//GetTime 返回时间的字符

	char *  CEditLog::GetTime();					//把时间转字符

	bool StartLine;									//是否是一行的开头

	DWORD ID;										//ID号

	bool SetEditText(char *str,COLORREF color);		//设置文本

	bool WriteFile(char *str);						//写入文件

	bool WriteHead(COLORREF color);					//写入文件头

	COLORREF Color;									//颜色



protected:
	DECLARE_MESSAGE_MAP()
};

	CEditLog& endl(CEditLog&);
	
	}
}