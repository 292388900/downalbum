#include "stdafx.h"
#include "Control.h"


/*------------------------------------------------------------------------
[7/24/2009 xiaolin]
说明:   	
------------------------------------------------------------------------*/
namespace Star
{
	namespace Control
	{

#define  RED	RGB(255,0,0)
#define  BLUE	RGB(0,0,255)
#define  GREEN	RGB(0,255,0)


// CEditLog

IMPLEMENT_DYNAMIC(CEditLog, CRichEditCtrl)
CEditLog::CEditLog()
{
	stratbool=false;
	filebool=false;
	ID=0;
	RecordTimeBool=true;
	LineLen=1000;
	 Color=RGB(0,0,0);
}

CEditLog::~CEditLog()
{
	if(filebool)
	{
		outfile.close();
	}
}


BEGIN_MESSAGE_MAP(CEditLog, CRichEditCtrl)


END_MESSAGE_MAP()



// CEditLog 消息处理程序


bool	CEditLog::InitEditLog(RECT& rect,CWnd *pwnd,UINT nID,char *path)		//创建类
{
	stratbool=true;

	this->CreateEx(NULL, WS_CHILD|WS_VISIBLE|ES_MULTILINE | ES_READONLY | WS_VSCROLL |WS_HSCROLL ,rect,pwnd,nID);

	SetTimer(10,200,NULL);

	if(path!=NULL)
	{
		outfile.open(path,std::ios::app);
		if(outfile)
		{
			filebool=true;

			outfile<<std::endl<<std::endl;
		}
		else
		{
			return false;
		}

	}



	StartLine=TRUE;

	write("开始启动日志",BLUE);
	EndLine();


	return true;
}

char *  CEditLog::GetTime()
{
	SYSTEMTIME t;
	::GetLocalTime(&t);
	sprintf(strtime,"%d-%d-%d %d:%d:%d - %d",t.wYear,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond,t.wMilliseconds);

	return strtime;

}
bool CEditLog::write(char *str,COLORREF color)
{
	if(!stratbool) return false;
	
	if(StartLine)
	{
		WriteHead(color);
	}

	if(filebool)
	{
		WriteFile(str);
	}

	SetEditText(str,color);


	return true;
}

bool CEditLog::write(int l,COLORREF color)
{
	if(!stratbool) return false;

	char str[10]="\0";

	itoa(l,str,10);

	if(StartLine)
	{
		WriteHead(color);
	}

	if(filebool)
	{
		WriteFile(str);
	}

	SetEditText(str,color);

	return true;
}

bool CEditLog::write(double f,COLORREF color)
{

	if(!stratbool) return false;

	char str[50]="\0";

	sprintf(str,"%f",f);

	if(StartLine)
	{
		WriteHead(color);
	}

	if(filebool)
	{
		WriteFile(str);
	}

	SetEditText(str,color);

	return true;
}

bool CEditLog::write(CString& str,COLORREF color)
{
	if(!stratbool) return false;

	if(StartLine)
	{
		WriteHead(color);
	}

	if(filebool)
	{
		WriteFile(str.GetBuffer());
		str.ReleaseBuffer();
	}

	SetEditText(str.GetBuffer(),color);
	str.ReleaseBuffer();
	return true;
} 



bool CEditLog::SetEditText(char *str,COLORREF color)		//设置文本
{    
	int nOldLines = 0, nNewLines = 0, nScroll = 0;
	long nInsertionPoint = 0;
	CHARFORMAT cf;

	// Save number of lines before insertion of new text
	nOldLines		= GetLineCount();

	if(nOldLines>LineLen)
	{
		SetSel(0,GetWindowTextLength()/2);
		ReplaceSel("");
		nOldLines		= GetLineCount();
	}

	// Initialize character format structure
	cf.cbSize		= sizeof(CHARFORMAT);
	cf.dwMask		= CFM_COLOR;
	cf.dwEffects	= 0;	// To disable CFE_AUTOCOLOR
	cf.crTextColor	= color;

	// Set insertion point to end of text
	nInsertionPoint = GetWindowTextLength();
	SetSel(nInsertionPoint, -1);

	//  Set the character format
	SetSelectionCharFormat(cf);

	// Replace selection. Because we have nothing selected, this will simply insert
	// the string at the current caret position.
	ReplaceSel(str);

	// Get new line count
	nNewLines		= GetLineCount();

	// Scroll by the number of lines just inserted
	nScroll			= nNewLines - nOldLines;
	LineScroll(nScroll);
	return true;

}


bool CEditLog::WriteHead(COLORREF color)
{
	char str[100]="\0";
	
	ID++;

	if(RecordTimeBool)
	{
		sprintf(str,"ID=%d Time=%s  Text=",ID,	GetTime());
	}
	else
	{
		sprintf(str,"ID=%d Text=",ID);
	}
	
	if(filebool)
	{
		WriteFile(str);
	}

	SetEditText(str,color);

	StartLine=false;

	return true;
}

bool CEditLog::WriteFile(char *str)
{
	outfile<<str;

	return true;
}

bool CEditLog::EndLine()
{

	if(!stratbool) return false;

	if(filebool)
	{
		outfile<<std::endl;
	}

	Color=RGB(0,0,0);

	SetEditText("\n",Color);

	StartLine=true;

	return true;
}

CEditLog& CEditLog::operator<<(char*str)
{
	try
	{
	
		if(strlen(str)>5)
		{
			if(str[0]=='R' && str[1]=='G' && str[2]=='B')
			{
				int i=4;
				char r[5]="\0",g[5]="\0",b[5]="\0";
				int l=i;
				int count=0;
				while (str[l])
				{

					if(str[l]==',')
					{
						switch(count)
						{
						case 0:
							{
								strncpy(r,&str[i],l-i);
								i=l+1;
							}
							break;
						case 1:
							{
								strncpy(g,&str[i],l-i);
								i=l+1;
							}
							break;
	
						}
						count++;
					}

					if(str[l]==')')
					{				
						strncpy(b,&str[i],l-i);
						i=l+1;		
						l++;
						break;
					}
					l++;


				}

				TRACE("%s,%s,%s",r,g,b);

				int R,G,B;

				R=atoi(r);
				G=atoi(g);
				B=atoi(b);

				Color=RGB(R,G,B);

				if(strlen(&str[l]))
				{
					write(&str[l],Color);						
				}


			}
			else
			{
				write(str,Color);
			}
		}
		else
		{
			write(str,Color);
		}
	}
	catch (...)
	{	EndLine();
		write("分析RGB色出错",RED);
	}
	
	
	return *this;
}

CEditLog& CEditLog::operator<<(CString&str)
{
	operator<<(str.GetBuffer());
	str.ReleaseBuffer();
	return *this;
}

CEditLog& CEditLog::operator<<(int i)
{
	write(i,Color);
	return *this;
}

CEditLog& CEditLog::operator<<(double f)
{
	write(f,Color);
	return *this;
}

CEditLog& endl(CEditLog&a)
{
	a.EndLine();
	return a;
}

CEditLog& CEditLog::operator<<(CEditLog& (*a)(CEditLog&))
{
	return (*a)(*this);
}

//------------------------------------------------------------------------
}
}